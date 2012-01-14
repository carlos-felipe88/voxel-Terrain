#ifndef PART3_HPP
#define PART3_HPP

#include "blub/core/axisAlignedBox.hpp"
#include "blub/core/triangle.hpp"
#include "blub/core/hashMap.hpp"
#include "blub/core/mutex.hpp"
#include "blub/core/sharedPointer.hpp"

#include "blub/procedural/voxelMap/map3.hpp"
#include "blub/procedural/voxelMap/edit.hpp"
#include "blub/procedural/voxel.hpp"

#include <boost/signals2.hpp>

namespace blub
{

namespace procedural
{

namespace voxelMap
{

    // class not threadsafe
    class part3 : public mutex
    {
    public:
        part3(int32 xStart, int32 yStart, int32 zStart, uint32 xSize, uint32 ySize, uint32 zSize, real scale, bool calcTransvoxel);
        ~part3();

        // lock class part3 before
        void calculateGeometry(void);

        vector3* hullVertices(void);
        vector3* hullNormals(void);
        // first is lod 0; the rest 6 are the transvoxel-lod
        triangle* hullTriangles(uint16 ind);
        uint32 numVertices() const;
        // first is lod 0; the rest 6 are the transvoxel-lod
        uint32 numTriangles(uint16 ind) const;


        // deletes all vertices and triangles; for memorysaves; to recalculate geometry call calculateGeometry();
        void clearGeometry(void);

        typedef boost::signals2::signal<void ()> t_signalPartChanged;
        void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
        {m_sigPartChanged.connect(subscriber);}

        axisAlignedBox aabb();

        // class part3 has to be locked
        void edit (sharedPointer<voxelMap::edit> ed);

        // class part3 has to be locked
        map3* map();
    private:
        struct vertex
        {
            vertex(vector3 _pos, vector3 _normal)
                : pos(_pos)
                , normal(_normal)
            {;}
            vertex(vector3 _pos)
                : pos(_pos)
                , normal(0)
            {;}
            vertex()
                : pos(0)
                , normal(0)
            {;}
            vector3 pos;
            vector3 normal;
        };

        void copyCalcToRelease();
        static blub::vector3 calculateIntersection(uint32 nX, uint32 nY, uint32 nZ, uint16 corner0, uint16 corner1, map3 *mp);
        blub::vector3 calculateIntersectionTransvoxel(uint32 nX, uint32 nY, uint32 nZ, uint16 corner0, uint16 corner1, map3 *mp, const int32 voxel[9][3]);
        static blub::vector3 calculateCorner(vector3 offset, uint16 corner);
        static blub::vector3 calculateCornerTransvoxel(vector3 offset, const uint16 corner, const int32 voxel[9][3]);
        void calculateMarchingCubes(int32 startX, int32 startY, int32 startZ,
                                    int32 endX, int32 endY, int32 endZ,
                                    map3 *mp,
                                    hashMap<uint32, vertex>* verticesCalc,
                                    vector<triangle>* triangleCalc);
        void calculateTransvoxel(int32 startX, int32 startY, int32 startZ,
                                 int32 endX, int32 endY, int32 endZ,
                                 map3 *mp,
                                 map3 *mpTrans,
                                 hashMap<uint32, vertex> *verticesCalc,
                                 vector<triangle>* triangleCalc, bool invertTriangles, int32 coord);

        static vector3 interpolate(real fX1, real fY1, real fZ1, real fX2, real fY2, real fZ2, real tVal1, real tVal2);
        // blub::real voxelScalar(uint32 x, uint32 y, uint32 z);
        static blub::real voxelScalar(uint32 x, uint32 y, uint32 z, map3 *mp);
        static blub::real voxelScalarTrans(uint32 x, uint32 y, uint32 z, map3 *mp);
        uint32 edgeID(uint32 nX, uint32 nY, uint32 nZ, uint16 edgeId);
        uint32 vertexID(uint32 nX, uint32 nY, uint32 nZ);

        map3 m_map;
        map3 *m_mapTransvoxel;

        int32 m_startX;
        int32 m_startY;
        int32 m_startZ;
        uint32 m_sizeX;
        uint32 m_sizeY;
        uint32 m_sizeZ;

        vector3* m_hullVerticesRelease;
        vector3* m_hullNormalsRelease;
        // one mainpart, 6 with lod 0 and 6 with lod 1
        triangle* m_hullTrianglesRelease[1+6];
        uint32 m_numVertices;
        uint32 m_numTriangles[1+6];

        typedef hashMap<uint32, vertex> t_hullVerticesCalc;
        // one mainpart, 6 with lod 0 and 6 with lod 1
        t_hullVerticesCalc m_hullVerticesCalc;
        // one mainpart, 6 with lod 0 and 6 with lod 1
        vector<triangle> m_hullTrianglesCalc[1+6];

        t_signalPartChanged m_sigPartChanged;

        static const uint32 m_edgeTable[256];
        static const int32 m_triTable[256][16];

        real m_scale;
        typedef sharedPointer<voxelMap::edit> t_edit;
        vector<t_edit> m_edits;

        bool m_calcTransvoxel;
    };

}
}
}

#endif // PART3_HPP
