#include "manualObject.hpp"
#include "blub/core/log.hpp"
#include "blub/graphic/material.hpp"

#include "QVector"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreSubMesh.h>
#include <OGRE/OgreCommon.h>
#include <OGRE/OgreHardwareBuffer.h>
#include <OGRE/OgreHardwareBufferManager.h>


using namespace blub::graphic;
using namespace blub;

uint32 manualObject::g_idCounter = 0;


manualObject::manualObject(graphic::scene *sc, uint16 numLod)
    : m_scene(sc)
//    , m_begin(false)
    , m_id(g_idCounter++)
    , m_mesh(0)
    , m_numLod(numLod)
    , m_staticVertexBuffer(true)
{
//    BOUT("manualObject::manualObject(graphic::scene *sc, uint16 numLod)");
    m_lods = new lod[numLod];
}

manualObject::~manualObject()
{
    deleteAllGeometry();
    if (m_mesh)
        delete m_mesh;
    if (!m_mainMesh.isNull())
    {
        Ogre::MeshManager::getSingletonPtr()->remove(string("mO"+string::number(m_id)));
    }
    delete [] m_lods;
}

void manualObject::initialiseMesh(Ogre::MeshPtr mesh)
{
    (void)mesh;
    /*mesh->_setBounds(axisAlignedBox(-1000,-1000,-1000,1000,1000,1000));
    mesh->_setBoundingSphereRadius(Ogre::Math::Sqrt(3*1000*1000));*/
}

graphic::mesh* manualObject::mesh()
{
    return m_mesh;
}

void manualObject::deleteTriangles(uint16 id)
{
    if (m_lods[id].triangles != 0)
        delete [] m_lods[id].triangles;
    m_lods[id].triangles=0;
}

void manualObject::aabb(blub::axisAlignedBox box)
{
    m_aabb = box;
    // setPremadeAabb(box.minimum(), box.maximum());
}

/*vector<vector3> *manualObject::position(uint16 lod)
{
    return &m_vertices[lod];
}
*/
void manualObject::position(const vector3 *pos, const uint32 num, uint16 lod)
{
    memcpy(m_lods[lod].vertices, pos, num*sizeof(vector3));
}

void manualObject::position(const vector<vector3> &pos, uint16 lod)
{
    uint32 counter(0);
    for (vector<vector3>::const_iterator it = pos.constBegin(); it != pos.constEnd(); ++it, ++counter)
    {
        m_lods[lod].vertices[counter] = *it;
    }
}

void manualObject::normal(vector3 *norms, uint32 num, uint16 lod)
{
    memcpy(m_lods[lod].normals, norms, num*sizeof(vector3));
}

void manualObject::normal(vector<vector3> normal, uint16 lod)
{
    uint32 counter(0);
    for (vector<vector3>::const_iterator it = normal.constBegin(); it != normal.constEnd(); ++it, ++counter)
    {
        m_lods[lod].normals[counter] = *it;
    }
}

void manualObject::triangle(blub::triangle *tris, uint32 num, uint16 lod)
{
    memcpy(m_lods[lod].triangles, tris, num*sizeof(blub::triangle));
}

void manualObject::triangle(const vector<blub::triangle> &tris, uint16 lod)
{
    uint32 counter(0);
    for (vector<blub::triangle>::const_iterator it = tris.constBegin(); it != tris.constEnd(); ++it, ++counter)
    {
        m_lods[lod].triangles[counter] = *it;
    }
}

void manualObject::triangle(const vector<uint32> &tris, uint16 lod)
{
    uint32 counter(0);
    for (vector<blub::uint32>::const_iterator it = tris.constBegin(); it < tris.constEnd(); it+=3, ++counter)
    {
        m_lods[lod].triangles[counter].ind[0] = *(it+0);
        m_lods[lod].triangles[counter].ind[1] = *(it+1);
        m_lods[lod].triangles[counter].ind[2] = *(it+2);
    }
}

void manualObject::deleteAllGeometry()
{
    deleteAllTriangles();
    deleteAllVertices();
}

void manualObject::deleteAllTriangles()
{
    for (uint16 ind = 0; ind < m_numLod; ++ind)
    {
        deleteTriangles(ind);
    }
}

void manualObject::deleteAllVertices()
{
    for (uint16 ind = 0; ind < m_numLod; ++ind)
    {
        if (m_lods[ind].vertices != 0)
        {
            delete [] m_lods[ind].vertices; m_lods[ind].vertices=0;
        }
        if (m_lods[ind].normals != 0)
        {
            delete [] m_lods[ind].normals; m_lods[ind].normals=0;
        }
    }
}

void manualObject::staticVertexBuffer(bool set)
{
    m_staticVertexBuffer = set;
}

/*void manualObject::updateVertices()
{
    Ogre::MeshPtr todo = m_mainMesh;
    updateVerticesSubmesh(todo->getSubMesh(0));
    for (uint16 ind = 0; ind < m_numLod-1; ++ind)
    {
        todo = m_lodMesh[ind];
        updateVerticesSubmesh(todo->getSubMesh(0));
    }
}
*/

/*void manualObject::updateVerticesSubmesh(Ogre::SubMesh* mesh)
{
    Ogre::HardwareVertexBufferSharedPtr vBuffer = mesh->vertexData->vertexBufferBinding->getBuffer(0);
    if (vBuffer->getNumVertices() != (unsigned)m_vertices->size())
    {
        BERROR("vBuffer->getNumVertices() != m_vertices->size()");
        return;
    }
    void* buffer = vBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
    memcpy(buffer, m_vertices->toVector().data(), vBuffer->getSizeInBytes());
    vBuffer->unlock();
}*/

void manualObject::createSubmesh(Ogre::MeshPtr mesh, uint16 _lod, string materialName)
{
    lod *work = &m_lods[_lod];
    if (work->numTriangles == 0)
        return;
    /*if (mesh->getNumSubMeshes() > 0)
        mesh->destroySubMesh(0);*/
    Ogre::SubMesh* sub = mesh->createSubMesh();
    sub->setBuildEdgesEnabled(false);
    sub->useSharedVertices = true;
    /*sub->vertexData = new Ogre::VertexData();
    createVertexData(sub->vertexData, work);*/

    // indexBuffer
    uint16 numIndices(work->numTriangles*3);
    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().
            createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
                numIndices,
            Ogre::HardwareBuffer::HBU_STATIC);

    // Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), work->triangles, true);

    // Set parameters of the submesh
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = numIndices;
    sub->indexData->indexStart = 0;

    sub->setMaterialName(materialName); // "triPlanarMaterial1"
}

void manualObject::createSharedVertexData(Ogre::MeshPtr mesh)
{
    (void) mesh;
}

void manualObject::createVertexData(Ogre::VertexData *data, const lod *work)
{
    data->vertexCount = work->numVertices;
    // Vertex declaration
    Ogre::VertexDeclaration* decl = data->vertexDeclaration;
    size_t offset = 0;
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    // add vertices
    Ogre::HardwareVertexBufferSharedPtr vbuf;
    if (m_staticVertexBuffer)
    {
        vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                                    offset, data->vertexCount, Ogre::HardwareBuffer::HBU_STATIC);
    }
    else
    {
        vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                                    offset, data->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
    }
    // Upload the vertex data to the card

    vbuf->writeData(0, vbuf->getSizeInBytes(), work->vertices, true);
    // bind vertexBuffer to mesh
    Ogre::VertexBufferBinding* bind = data->vertexBufferBinding;
    bind->setBinding(0, vbuf);

    // same for normalbuffer
    if (work->normals != 0)
    {
        offset = 0;
        decl->addElement(1, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                                    offset, data->vertexCount, Ogre::HardwareBuffer::HBU_STATIC);
        vbuf->writeData(0, vbuf->getSizeInBytes(), work->normals, true);

        // bind to positionbuffer
        bind->setBinding(1, vbuf);
    }
}

void manualObject::reInitGraphic(material* mat)
{
//    BOUT("reInitGraphic()");
    string materialName = mat->_material()->getName();

    if (m_mainMesh.isNull())
    {
        // create meshes
        m_mainMesh = Ogre::MeshManager::getSingleton().createManual(string("mO"+string::number(m_id)), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        m_mesh = new graphic::mesh(m_mainMesh);
        initialiseMesh(m_mainMesh);
/*        for (uint16 ind = 0; ind < m_numLod-1; ++ind)
        {
            m_lodMesh[ind] = Ogre::MeshManager::getSingleton().createManual(string("mO"+string::number(m_id)+"_"+string::number(ind)), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            initialiseMesh(m_lodMesh[ind]);
            m_mainMesh->createManualLodLevel((ind+1)*40.0, m_lodMesh[ind]->getName());
        }
*/    }
    m_mainMesh->sharedVertexData = new Ogre::VertexData();
    createVertexData(m_mainMesh->sharedVertexData, &m_lods[0]);
    for (uint16 ind = 0; ind < m_numLod; ++ind)
    {
        createSubmesh(m_mainMesh, ind, materialName);
    }
    m_mainMesh->_setBounds(m_aabb);
    m_mainMesh->_setBoundingSphereRadius(m_aabb.getSize().length()/2.);
    m_mainMesh->load(true);
    /*for (uint16 ind = 0; ind < m_numLod-1; ++ind)
    {
        createSubmesh(m_lodMesh[ind], ind+1, materialName);
        m_lodMesh[ind]->_setBounds();
        // m_lodMesh[ind]->_setBoundingSphereRadius(m_aabb.getSize().x);
        m_lodMesh[ind]->load(true);
    }*/

//    BOUT("reInitGraphic() done");
}

/*void manualObject::getLockedVertexIndexBase(unsigned char **vertexbase, int& numverts,PHY_ScalarType& type, int& stride,unsigned char **indexbase,int & indexstride,int& numfaces,PHY_ScalarType& indicestype,int subpart)
{
    (void) vertexbase;
    (void) numverts;
    (void) type;
    (void) stride;
    (void) indexbase;
    (void) indexstride;
    (void) numfaces;
    (void) indicestype;
    (void) subpart;
    BERROR("getLockedVertexIndexBase " + string::number(m_id));
    // getLockedVertexIndexBase(vertexbase, numverts, type, stride, indexbase, indexstride, numfaces, indicestype, subpart);
}
void manualObject::getLockedReadOnlyVertexIndexBase(const unsigned char **vertexbase, int& numverts,PHY_ScalarType& type, int& stride,const unsigned char **indexbase,int & indexstride,int& numfaces,PHY_ScalarType& indicestype,int subpart) const
{
    BOUT("getLockedReadOnlyVertexIndexBase " + string::number(m_id));
    (void)subpart;
    QVector<vector3> *uglyVert = new QVector<vector3>(m_vertices->toVector());
    *vertexbase = (unsigned char*)uglyVert->data();
    numverts = m_vertices->size();
    type = PHY_FLOAT;
    stride = sizeof(vector3);

    QVector<blub::triangle> *uglyTriangle = new QVector<blub::triangle>(m_indices->toVector());
    *indexbase = (unsigned char*)uglyTriangle->data();
    indexstride = sizeof(::blub::triangle);
    numfaces = m_indices->size();
    indicestype = PHY_INTEGER;
}
void manualObject::unLockVertexBase(int subpart)
{
    (void)subpart;
}
void manualObject::unLockReadOnlyVertexBase(int subpart) const
{
    (void)subpart;
}
int manualObject::getNumSubParts() const
{
    return 1;
}
void manualObject::preallocateVertices(int numverts)
{
    (void)numverts;
}

void manualObject::preallocateIndices(int numindices)
{
    (void)numindices;
}

*/

void manualObject::numVertices(uint32 num, uint16 _lod)
{
    deleteAllVertices();
    m_lods[_lod].numVertices = num;
    m_lods[_lod].vertices = new vector3[num];
    m_lods[_lod].normals = new vector3[num];
}

void manualObject::numTriangles(uint32 num, uint16 _lod)
{
    deleteTriangles(_lod);
    m_lods[_lod].numTriangles = num;
    m_lods[_lod].triangles = new blub::triangle[num];
}


manualObject::lod *manualObject::lods()
{
    return m_lods;
}


