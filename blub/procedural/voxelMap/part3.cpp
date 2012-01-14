#include "part3.hpp"
#include "blub/core/mutexLocker.hpp"
#include "blub/core/log.hpp"
#include "blub/procedural/voxelMap/transVoxel.hpp"

using namespace blub;
using namespace blub::procedural;
using namespace blub::procedural::voxelMap;


part3::part3(int32 xStart, int32 yStart, int32 zStart, uint32 xSize, uint32 ySize, uint32 zSize, real scale, bool calcTransvoxel)
    : m_map(1, 1, 1, xSize+3, ySize+3, zSize+3)
    , m_mapTransvoxel(0)
    , m_startX(xStart)
    , m_startY(yStart)
    , m_startZ(zStart)
    , m_sizeX(xSize)
    , m_sizeY(ySize)
    , m_sizeZ(zSize)
    , m_hullVerticesRelease(0)
    , m_hullNormalsRelease(0)
    , m_scale(scale)
    , m_calcTransvoxel(calcTransvoxel)
{
    m_hullTrianglesRelease[0] = 0;

    if (m_calcTransvoxel)
    {   // init transvoxel maps
        m_mapTransvoxel = new map3[6];
        m_mapTransvoxel[0] = map3(1, 1, 0, xSize*2+1+3, ySize*2+1+3, 1);
        m_mapTransvoxel[1] = map3(1, 1, -zSize*2, xSize*2+1+3, ySize*2+1+3, 1);

        m_mapTransvoxel[2] = map3(1, 0, 1, xSize*2+1+3, 1, zSize*2+1+3);
        m_mapTransvoxel[3] = map3(1, -ySize*2, 1, xSize*2+1+3, 1, zSize*2+1+3);

        m_mapTransvoxel[4] = map3(0, 1, 1, 1, ySize*2+1+3, zSize*2+1+3);
        m_mapTransvoxel[5] = map3(-xSize*2, 1, 1, 1, ySize*2+1+3, zSize*2+1+3);
    }
}

part3::~part3()
{
    // BOUT("~part3()");
    if (m_mapTransvoxel)
        delete [] m_mapTransvoxel;
}

void part3::calculateGeometry(void)
{
    // cleanUp
    clearGeometry();

    m_map.begin();
    if (m_calcTransvoxel)
    {
        for (uint16 ind = 0; ind < 6; ++ind)
            m_mapTransvoxel[ind].begin();
    }
    //axisAlignedBox delta;
    //delta.setNull();
    for (vector<t_edit>::iterator it = m_edits.begin(); it != m_edits.end(); ++it)
    {
        voxelMap::edit *ed = (*it).data();
        ed->createVoxel(&m_map,
                        m_startX-1, m_startY-1, m_startZ-1,
                        m_startX+m_sizeX+2, m_startY+m_sizeY+2, m_startZ+m_sizeZ+2,
                        m_scale); // voxel get created, slower but saves !lots! of memory
        //delta.merge(ed->aab(1.));
        if (m_calcTransvoxel)
        {
            m_startX*=2;m_startY*=2;m_startZ*=2;
            ed->createVoxel(&m_mapTransvoxel[0],
                            m_startX-1, m_startY-1, m_startZ-0,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+2, m_startZ+1,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[1],
                            m_startX-1, m_startY-1, m_startZ+m_sizeZ*2-0,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+1,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[2],
                            m_startX-1, m_startY+0, m_startZ-1,
                            m_startX+m_sizeX*2+2, m_startY+1, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[3],
                            m_startX-1, m_startY+m_sizeY*2-0, m_startZ-1,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+1, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[4],
                            m_startX-0, m_startY-1, m_startZ-1,
                            m_startX+1, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[5],
                            m_startX+m_sizeX*2-0, m_startY-1, m_startZ-1,
                            m_startX+m_sizeX*2+1, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            m_startX/=2;m_startY/=2;m_startZ/=2;
        }
    }
    //delta.move(-vector3(m_startX, m_startY, m_startZ));
    //delta.scale(vector3(1./m_scale));
    // Generate isosurface.
    // axisAlignedBox delta = m_map.delta();
    int32 startX = /*delta.getMinimum().x-1; if (startX < 0) startX = */0;
    int32 startY = /*delta.getMinimum().y-1; if (startY < 0) startY = */0;
    int32 startZ = /*delta.getMinimum().z-1; if (startZ < 0) startZ = */0;

    // +2 because of "<" in later for
    int32 endX = /*delta.getMaximum().x+3; if (endX > (signed)m_sizeX+2) endX = */m_sizeX+2;
    int32 endY = /*delta.getMaximum().y+3; if (endY > (signed)m_sizeY+2) endY = */m_sizeY+2;
    int32 endZ = /*delta.getMaximum().z+3; if (endZ > (signed)m_sizeZ+2) endZ = */m_sizeZ+2;

    calculateMarchingCubes(startX+1, startY+1, startZ+1, endX-1, endY-1, endZ-1, &m_map, &m_hullVerticesCalc, &m_hullTrianglesCalc[0]);

    const int32 offsetsNormalCorrection[][6] = {
                                {startX, startY, startZ, endX, endY, startZ+1},
                                {startX, startY, endZ-1, endX, endY, endZ},
                                {startX, startY, startZ, endX, startY+1, endZ},
                                {startX, endY-1, startZ, endX, endY, endZ},
                                {startX, startY, startZ, startX+1, endY, endZ},
                                {endX-1, startY, startZ, endX, endY, endZ},
                               };
    t_hullVerticesCalc verticesBuffer;
    for (uint16 ind = 0; ind < 6; ++ind)
    {
        calculateMarchingCubes(offsetsNormalCorrection[ind][0],
                               offsetsNormalCorrection[ind][1],
                               offsetsNormalCorrection[ind][2],
                               offsetsNormalCorrection[ind][3],
                               offsetsNormalCorrection[ind][4],
                               offsetsNormalCorrection[ind][5],
                               &m_map, &verticesBuffer, 0);
    }
    for (t_hullVerticesCalc::iterator it = verticesBuffer.begin(); it != verticesBuffer.end(); ++it)
    {
        t_hullVerticesCalc::iterator it2 = m_hullVerticesCalc.find(it.key());
        if (it2 != m_hullVerticesCalc.end())
            it2.value().normal += it.value().normal;
    }
    if (m_calcTransvoxel)
    {
        calculateTransvoxel(startX+1, startY+1, 1, endX-1, endY-1, 2, &m_map, &m_mapTransvoxel[0], &m_hullVerticesCalc, &m_hullTrianglesCalc[1], false, 1);
        calculateTransvoxel(startX+1, startY+1, m_sizeZ+1, endX-1, endY-1, m_sizeZ+2, &m_map, &m_mapTransvoxel[1], &m_hullVerticesCalc, &m_hullTrianglesCalc[2], true, 1);

        calculateTransvoxel(startX+1, 1, startZ+1, endX-1, 2, endZ-1, &m_map, &m_mapTransvoxel[2], &m_hullVerticesCalc, &m_hullTrianglesCalc[3], true, 2);
        calculateTransvoxel(startX+1, m_sizeY+1, startZ+1, endX-1, m_sizeY+2, endZ-1, &m_map, &m_mapTransvoxel[3], &m_hullVerticesCalc, &m_hullTrianglesCalc[4], false, 2);

        calculateTransvoxel(1, startY+1, startZ+1, 2, endY-1, endZ-1, &m_map, &m_mapTransvoxel[4], &m_hullVerticesCalc, &m_hullTrianglesCalc[5], false, 0);
        calculateTransvoxel(m_sizeX+1, startY+1, startZ+1, m_sizeX+2, endY-1, endZ-1, &m_map, &m_mapTransvoxel[5], &m_hullVerticesCalc, &m_hullTrianglesCalc[6], true, 0);

    }

    m_map.end(); // voxels get deleted - save lots of memory
    if (m_calcTransvoxel)
    {
        for (uint16 ind = 0; ind < 6; ++ind)
            m_mapTransvoxel[ind].end();
    }

    copyCalcToRelease();

    m_sigPartChanged();

    //    BOUT("calculateGeometry(bool reduce) done num Triangles: " + blub::string::number(m_hullTriangles.size()));
}

axisAlignedBox part3::aabb()
{
    return axisAlignedBox(m_startX*m_scale,
                          m_startY*m_scale,
                          m_startZ*m_scale,
                          m_startX*m_scale + m_sizeX*m_scale,
                          m_startY*m_scale + m_sizeY*m_scale,
                          m_startZ*m_scale + m_sizeZ*m_scale);
}

void part3::copyCalcToRelease()
{
    m_numVertices = 0;
    for (uint16 ind = 0; ind < 1+6; ++ind)
        m_numTriangles[ind] = 0;

    uint32 numVertices = m_hullVerticesCalc.size();
    // no vertices --> no indices
    if (numVertices == 0)
        return;
    m_hullVerticesRelease = new vector3[numVertices];
    m_hullNormalsRelease = new vector3[numVertices];

    hashMap<uint32, uint32> vertexIndices;
    vertexIndices.reserve(numVertices);
    for (t_hullVerticesCalc::iterator it = m_hullVerticesCalc.begin(); it != m_hullVerticesCalc.end(); ++it)
    {
        vector3 pos = it.value().pos-vector3(1.)*m_scale;
        vector3 normal = it.value().normal;
        m_hullVerticesRelease[m_numVertices] = pos;
        m_hullNormalsRelease[m_numVertices] = normal.normalisedCopy();
        vertexIndices.insert(it.key(), m_numVertices);
        ++m_numVertices;
    }

    // calc triangles
    for (uint16 ind = 0; ind < 1+6; ++ind)
    {
        uint32 numTriangles = m_hullTrianglesCalc[ind].size();
        m_hullTrianglesRelease[ind] = new triangle[numTriangles];
        for (vector<triangle>::iterator it = m_hullTrianglesCalc[ind].begin(); it != m_hullTrianglesCalc[ind].end(); ++it)
        {
            for (uint16 ind2 = 0; ind2 < 3; ++ind2)
            {
                uint32 newId = vertexIndices.find((*it).ind[ind2]).value();
                m_hullTrianglesRelease[ind][m_numTriangles[ind]].ind[ind2] = newId;
            }
            ++m_numTriangles[ind];
        }
    }
    // clean up
    m_hullVerticesCalc.clear();
    for (uint16 ind = 0; ind < 1+6; ++ind)
    {
        m_hullTrianglesCalc[ind].clear();
    }
}

vector3 part3::calculateIntersection(uint32 nX, uint32 nY, uint32 nZ, uint16 corner0, uint16 corner1, map3 *mp)
{
    vector3 offset(nX, nY, nZ);
    vector3 corn0 = calculateCorner(offset, corner0);
    vector3 corn1 = calculateCorner(offset, corner1);
    real val0 = voxelScalar(corn0.x, corn0.y, corn0.z, mp);
    real val1 = voxelScalar(corn1.x, corn1.y, corn1.z, mp);
    vector3 result = interpolate(corn0.x, corn0.y, corn0.z, corn1.x, corn1.y, corn1.z, val0, val1);
    return result;
}

vector3 part3::calculateIntersectionTransvoxel(uint32 nX, uint32 nY, uint32 nZ, uint16 corner0, uint16 corner1, map3 *mp, const int32 voxel[9][3])
{
    vector3 offset(nX, nY, nZ);
    vector3 corn0 = calculateCornerTransvoxel(offset, corner0, voxel);
    vector3 corn1 = calculateCornerTransvoxel(offset, corner1, voxel);
    real val0 = voxelScalarTrans(corn0.x*2.-1, corn0.y*2.-1, corn0.z*2.-1, mp);
    real val1 = voxelScalarTrans(corn1.x*2.-1, corn1.y*2.-1, corn1.z*2.-1, mp);
    vector3 result = interpolate(corn0.x, corn0.y, corn0.z, corn1.x, corn1.y, corn1.z, val0, val1);
    return result;
}

vector3 part3::calculateCorner(vector3 offset, uint16 corner)
{
    switch (corner)
    {
    case 0:
        return offset;
    case 1:
        return offset + vector3(1.,0.,0.);
    case 2:
        return offset + vector3(0.,0.,1.);
    case 3:
        return offset + vector3(1.,0.,1.);
    case 4:
        return offset + vector3(0.,1.,0.);
    case 5:
        return offset + vector3(1.,1.,0.);
    case 6:
        return offset + vector3(0.,1.,1.);
    case 7:
        return offset + vector3(1.,1.,1.);
    default:
        // fatal
        assert(0);
    }
    // never reached
    assert(0);
    return vector3(0);
}

vector3 part3::calculateCornerTransvoxel(vector3 offset, const uint16 corner, const int32 voxel[9][3])
{
    vector3 result;
    switch (corner)
    {
    case 0x0:
        result = vector3(0.);
        break;
    case 0x1:
        result = vector3(voxel[1][0], voxel[1][1], voxel[1][2]);
        break;
    case 0x2:
        result = vector3(voxel[2][0], voxel[2][1], voxel[2][2]);
        break;
    case 0x3:
        result = vector3(voxel[7][0], voxel[7][1], voxel[7][2]);
        break;
    case 0x4:
        result = vector3(voxel[8][0], voxel[8][1], voxel[8][2]);
        break;
    case 0x5:
        result = vector3(voxel[3][0], voxel[3][1], voxel[3][2]);
        break;
    case 0x6:
        result = vector3(voxel[6][0], voxel[6][1], voxel[6][2]);
        break;
    case 0x7:
        result = vector3(voxel[5][0], voxel[5][1], voxel[5][2]);
        break;
    case 0x8:
        result = vector3(voxel[4][0], voxel[4][1], voxel[4][2]);
        break;
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
        break;
    default:
        assert(0);
    }
    return offset + result / 2.;
}

void part3::calculateMarchingCubes(int32 startX, int32 startY, int32 startZ, int32 endX, int32 endY, int32 endZ, map3 *mp, hashMap<uint32, vertex> *verticesCalc, vector<triangle> *triangleCalc)
{
    /*if (m_calcTransvoxel)
        return;
*/
    const blub::real isoLevel(0.);
    for (uint32 z = startZ; z < (unsigned)endZ; ++z)
    {
        for (uint32 y = startY; y < (unsigned)endY; ++y)
        {
            for (uint32 x = startX; x < (unsigned)endX; ++x)
            {
                uint32 tableIndex = 0;
                if (voxelScalar(x, y, z, mp) < isoLevel)
                    tableIndex |= 1;
                if (voxelScalar(x+1, y, z, mp) < isoLevel)
                    tableIndex |= 2;
                if (voxelScalar(x, y, z+1, mp) < isoLevel)
                    tableIndex |= 4;
                if (voxelScalar(x+1, y, z+1, mp) < isoLevel)
                    tableIndex |= 8;
                if (voxelScalar(x, y+1, z, mp) < isoLevel)
                    tableIndex |= 16;
                if (voxelScalar(x+1, y+1, z, mp) < isoLevel)
                    tableIndex |= 32;
                if (voxelScalar(x, y+1, z+1, mp) < isoLevel)
                    tableIndex |= 64;
                if (voxelScalar(x+1, y+1, z+1, mp) < isoLevel)
                    tableIndex |= 128;

                // Now create a triangulation of the isosurface in this
                // cell.
                if (tableIndex != 0 && tableIndex != 255)
                {
                    const RegularCellData *data = &regularCellData[regularCellClass[tableIndex]];
                    uint32 ids[12];
                    for (uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        uint16 data2 = regularVertexData[tableIndex][ind];
                        uint16 corner0 = data2 & 0x0F;
                        uint16 corner1 = (data2 & 0xF0) >> 4;
                        uint32 id = edgeID(x, y, z, data2 >> 8);
                        ids[ind]=id;
                        // uint16 owner = (data2 >> 8) & 0x0F;
                        if (/*owner == 8 || owner == 9) && */verticesCalc->find(id) == verticesCalc->end())
                        {
                            vector3 pt = calculateIntersection(x, y, z, corner0, corner1, mp);
                            // pt = (calculateCorner(vector3(x, y, z), corner0) + calculateCorner(vector3(x, y, z), corner1)) / 2.;
                            pt*=m_scale;
                            verticesCalc->insert(id, pt);
                        }
                    }
                    for (uint16 ind = 0; ind < data->GetTriangleCount()*3; ind+=3)
                    {
                        // calc normal
                        vertex *pos0(&(*verticesCalc)[ids[data->vertexIndex[ind+0]]]);
                        vertex *pos1(&(*verticesCalc)[ids[data->vertexIndex[ind+1]]]);
                        vertex *pos2(&(*verticesCalc)[ids[data->vertexIndex[ind+2]]]);
                        if (pos0->pos == pos1->pos || pos1->pos == pos2->pos || pos0->pos == pos2->pos)
                            continue; // triangle with 0 space
                        vector3 addNormal = (pos1->pos - pos0->pos).crossProduct(pos2->pos - pos0->pos);
                        {
                            pos0->normal += addNormal;
                            pos1->normal += addNormal;
                            pos2->normal += addNormal;
                        }
                        if (triangleCalc)
                        {
                            // insert new triangle
                            triangle tri(ids[data->vertexIndex[ind+0]],
                                         ids[data->vertexIndex[ind+1]],
                                         ids[data->vertexIndex[ind+2]]);
                            triangleCalc->push_back(tri);
                        }
                    }
                }
            }
        }
    }
}

void part3::calculateTransvoxel(int32 startX, int32 startY, int32 startZ,
                                int32 endX, int32 endY, int32 endZ,
                                map3 *mp, map3 *mpTrans,
                                hashMap<uint32, vertex> *verticesCalc, vector<triangle> *triangleCalc,
                                bool invertTriangles, int32 coord)
{
    (void)mp;
    const int32 voxel[][9][3] = {
                        {{0, 0, 0},{0, 1, 0},{0, 2, 0},{0, 2, 1},{0, 2, 2},{0, 1, 2},{0, 0, 2},{0, 0, 1},{0, 1, 1}},
                        {{0, 0, 0},{1, 0, 0},{2, 0, 0},{2, 1, 0},{2, 2, 0},{1, 2, 0},{0, 2, 0},{0, 1, 0},{1, 1, 0}},
                        {{0, 0, 0},{1, 0, 0},{2, 0, 0},{2, 0, 1},{2, 0, 2},{1, 0, 2},{0, 0, 2},{0, 0, 1},{1, 0, 1}},
                        };
    const blub::real isoLevel(0.);
    for (uint32 z = startZ; z < (unsigned)endZ; ++z)
    {
        for (uint32 y = startY; y < (unsigned)endY; ++y)
        {
            for (uint32 x = startX; x < (unsigned)endX; ++x)
            {
                {
                    x*=2;y*=2;z*=2;
                    x-=1;y-=1;z-=1;
                    uint32 tableIndex = 0;
                    uint32 add = 1;
                    for (uint16 ind = 0; ind < 9; ++ind)
                    {
                        if (voxelScalarTrans(x+voxel[coord][ind][0], y+voxel[coord][ind][1], z+voxel[coord][ind][2], mpTrans) < isoLevel)
                            tableIndex |= add;
                        add*=2;
                    }

                    // litte "small voxel" versus "big voxel" check if same for y
/*#ifdef BLUB_DEBUG
                    bool first = (voxelScalarTrans(x,y,z, mpTrans) < isoLevel) == (voxelScalar((x+1)/2,(y+1)/2,(z+1)/2,mp) < isoLevel);
                    bool snd = (voxelScalarTrans(x+2,y,z, mpTrans) < isoLevel) == (voxelScalar((x+1)/2 +1 ,(y+1)/2,(z+1)/2,mp) < isoLevel);
                    bool third = (voxelScalarTrans(x,y,z+2, mpTrans) < isoLevel) == (voxelScalar((x+1)/2,(y+1)/2,(z+1)/2 +1 ,mp) < isoLevel);
                    bool fourth = (voxelScalarTrans(x+2,y,z+2, mpTrans) < isoLevel) == (voxelScalar((x+1)/2 +1 ,(y+1)/2,(z+1)/2 +1 ,mp) < isoLevel);
                    if (!first || !snd || !third || !fourth)
                        BERROR("!first || !snd || !third || !fourth");
#endif*/

                    x+=1;y+=1;z+=1;
                    x/=2;y/=2;z/=2;

                    uint32 classIndex = transitionCellClass[tableIndex];
                    const TransitionCellData *data = &transitionCellData[classIndex & 0x7F]; // only the last 7 bit count
                    uint32 ids[12];
                    vector3 normal(0,0,0);

                    for (uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        uint16 data2 = transitionVertexData[tableIndex][ind];
                        uint16 edge = data2 >> 8;
                        uint16 edgeId = edge & 0x0F;
                        if (edgeId == 0x9 || edgeId == 0x8)
                        {
                            uint32 id;
                            if (coord == 1)
                                edgeId-=6;
                            if (coord == 2)
                            {
                                if (edgeId == 9)
                                    edgeId = 1;
                                else
                                    edgeId = 2;
                            }
                            if (coord == 0)
                            {
                                if (edgeId == 9)
                                    edgeId = 1;
                                else
                                    edgeId = 3;
                            }

                            uint16 owner = (edge & 0xF0) >> 4;
                            int16 diffX = owner%2;
                            int16 diffY = (owner >> 1)%2;
                            int16 diffZ = 1;

                            if (coord == 0)
                            {
                                diffX = 1;
                                diffY = owner%2;
                                diffZ = (owner >> 1)%2;
                            }
                            if (coord == 2)
                            {
                                diffY = 1;
                                diffZ = (owner >> 1)%2;
                            }

                            id = vertexID(x - diffX, y - diffY, z - diffZ) + (edgeId-1);
                            // id = vertexID(x, y, z) + 20 * (std::pow(19, 3) + 1) + ind;
                            ids[ind]=id;
                            t_hullVerticesCalc::iterator it = verticesCalc->find(id);
                            if (it != verticesCalc->end())
                                normal = it.value().normal;
                            else
                                BERROR("!!!!");
                        }
                    }
                    for (uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        uint16 data2 = transitionVertexData[tableIndex][ind];
                        uint16 edge = data2 >> 8;
                        uint16 edgeId = edge & 0x0F;
                        if (!(edgeId == 0x9 || edgeId == 0x8))
                        {
                            uint32 id;
                            uint16 corner0 = data2 & 0x0F;
                            uint16 corner1 = (data2 & 0xF0) >> 4;
                            // vector3 pt = (calculateCornerTransvoxel(vector3(x, y, z), corner0) + calculateCornerTransvoxel(vector3(x, y, z), corner1)) / 2.;
                            vector3 pt = calculateIntersectionTransvoxel(x, y, z, corner0, corner1, mpTrans, voxel[coord]);
                            pt*=m_scale;

                            uint16 owner = (edge & 0xF0) >> 4;
                            uint16 diffX = owner%2;
                            uint16 diffY = (owner >> 1)%2;
                            uint16 diffZ = 1;
                            if (coord == 0)
                            {
                                diffX = 1;
                                diffY = owner%2;
                                diffZ = (owner >> 1)%2;
                            }
                            if (coord == 2)
                            {
                                diffY = 1;
                                diffZ = (owner >> 1)%2;
                            }

                            id = vertexID(x - diffX, y - diffY, z - diffZ) + (edgeId-1) + 20 * (coord+1) * (m_sizeX+3)*(m_sizeY+3)*(m_sizeZ+3);
                            if (owner == 4) // wont get reused
                                id = vertexID(x, y, z) + 200 * (coord+1) * (m_sizeX+3)*(m_sizeY+3)*(m_sizeZ+3) + ind;
                            verticesCalc->insert(id, vertex(pt, normal));
                            ids[ind]=id;
                        }
                    }
                    for (uint16 ind = 0; ind < data->GetTriangleCount()*3; ind+=3)
                    {
                        if (normal == vector3(0))
                        {
                            // calc normal
                            vertex *pos0(&(*verticesCalc)[ids[data->vertexIndex[ind+0]]]);
                            vertex *pos1(&(*verticesCalc)[ids[data->vertexIndex[ind+1]]]);
                            vertex *pos2(&(*verticesCalc)[ids[data->vertexIndex[ind+2]]]);
                            if (pos0->pos == pos1->pos || pos1->pos == pos2->pos || pos0->pos == pos2->pos)
                                continue; // triangle with 0 space
                            vector3 addNormal = (pos1->pos - pos0->pos).crossProduct(pos2->pos - pos0->pos);
                            {
                                pos0->normal += addNormal;
                                pos1->normal += addNormal;
                                pos2->normal += addNormal;
                            }
                        }

                        triangle tri;
                        uint16 invert = 1;
                        if (invertTriangles)
                            invert = 0;
                        if ((classIndex >> 7) % 2 == invert)
                        {
                            tri = triangle(ids[data->vertexIndex[ind+0]],
                                           ids[data->vertexIndex[ind+1]],
                                           ids[data->vertexIndex[ind+2]]);
                        }
                        else
                        {
                            tri = triangle(ids[data->vertexIndex[ind+1]],
                                           ids[data->vertexIndex[ind+0]],
                                           ids[data->vertexIndex[ind+2]]);
                        }
                        triangleCalc->push_back(tri);
                    }
                }
            }
        }
    }
}

vector3 part3::interpolate(real fX1, real fY1, real fZ1, real fX2, real fY2, real fZ2, real tVal1, real tVal2)
{
    vector3 interpolation;
    real mu;

    mu = ((0. - tVal1))/(tVal2 - tVal1);
    interpolation.x = fX1 + mu*(fX2 - fX1);
    interpolation.y = fY1 + mu*(fY2 - fY1);
    interpolation.z = fZ1 + mu*(fZ2 - fZ1);

    return interpolation;
}
/*blub::real part3::voxelScalar(uint32 x, uint32 y, uint32 z)
{
    voxel *vox = m_map.getVoxel(((int32)x)-1, ((int32)y)-1, ((int32)z)-1);
    return vox->scalar;
}*/

real part3::voxelScalar(uint32 x, uint32 y, uint32 z, map3 *mp)
{
    voxel *vox = mp->getVoxel(((int32)x)-1, ((int32)y)-1, ((int32)z)-1);
    return vox->scalar;
}

real part3::voxelScalarTrans(uint32 x, uint32 y, uint32 z, map3 *mp)
{
    voxel *vox = mp->getVoxel(((int32)x)-1, ((int32)y)-1, ((int32)z)-1);
    return vox->scalar;
}
uint32 part3::edgeID(uint32 nX, uint32 nY, uint32 nZ, uint16 edgeId)
{
    uint16 edge = edgeId & 0x0F;
    uint16 owner = (edgeId & 0xF0) >> 4;
    uint32 result(-1);
    uint16 diffX = owner%2;
    uint16 diffY = (owner >> 2)%2;
    uint16 diffZ = (owner >> 1)%2;
    result = vertexID(nX - diffX, nY - diffY, nZ - diffZ) + (edge-1);
    return result;
}
uint32 part3::vertexID(uint32 nX, uint32 nY, uint32 nZ)
{
    return 20*(nZ*(m_sizeY + 3)*(m_sizeX + 3) + nY*(m_sizeX + 3) + nX);
}

map3* part3::map()
{
    return &m_map;
}

void part3::clearGeometry(void)
{
    if (m_hullVerticesRelease != 0)
        delete [] m_hullVerticesRelease; m_hullVerticesRelease = 0;
    if (m_hullNormalsRelease != 0)
        delete [] m_hullNormalsRelease; m_hullNormalsRelease = 0;
    if (m_hullTrianglesRelease[0] != 0)
        delete [] m_hullTrianglesRelease[0]; m_hullTrianglesRelease[0] = 0;
}

void part3::edit(sharedPointer<voxelMap::edit> ed)
{
    m_edits.push_back(ed);
}

vector3 *part3::hullVertices(void)
{
    return m_hullVerticesRelease;
}

vector3 *part3::hullNormals()
{
    return m_hullNormalsRelease;
}

triangle *part3::hullTriangles(uint16 ind)
{
    return m_hullTrianglesRelease[ind];
}

uint32 part3::numVertices() const
{
    return m_numVertices;
}

uint32 part3::numTriangles(uint16 ind) const
{
    return m_numTriangles[ind];
}
