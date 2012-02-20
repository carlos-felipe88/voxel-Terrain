#ifndef MANUALOBJECT_HPP
#define MANUALOBJECT_HPP

#include "blub/core/vector3.hpp"
#include "blub/core/vector2.hpp"
#include "blub/core/colour.hpp"
#include "blub/core/vector.hpp"
#include "blub/core/triangle.hpp"
#include "blub/core/axisAlignedBox.hpp"
#include "blub/graphic/graphic.hpp"
#include "blub/graphic/mesh.hpp"
#include "blub/graphic/scene.hpp"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreMesh.h>
// #include <bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.h>

#include <QVector>

namespace blub
{
namespace graphic
{

class material;

class manualObject// : public btStridingMeshInterface
{
public:
    struct lod {
        lod()
            : triangles(0)
            , vertices(0)
            , textures(0)
            , normals(0)
            , colours(0)
            , numVertices(0)
            , numTriangles(0)
        {
            ;
        }

        blub::triangle* triangles;
        vector3* vertices;
        vector2* textures;
        vector3* normals;
        blub::colour* colours;

        uint32 numVertices;
        uint32 numTriangles;
    };



    manualObject(graphic::scene *sc, uint16 numLod);
    ~manualObject();

    void numVertices(uint32 num, uint16 _lod = 0);
    void numTriangles(uint32 num, uint16 _lod = 0);
    // vector<vector3> *position(uint16 lod = 0);
    void position(const vector3* pos, const uint32 num, uint16 _lod = 0);
    /*void position(const vector3 pos, uint16 lod = 0);*/
    void position(const vector<vector3> &pos, uint16 lod = 0);
    void normal(vector3 *norms, uint32 num, uint16 _lod = 0);
    void normal(vector<vector3> normal, uint16 lod = 0);
    /*void normal(vector3 pos, uint16 lod = 0);
    void colour(colour col, uint16 lod = 0);
    void texture(vector2 tex, uint16 lod = 0);*/
    // vector<blub::triangle> *triangles(uint16 lod = 0);
    void triangle (blub::triangle*, uint32 num, uint16 _lod = 0);
    void triangle (const vector<blub::triangle> &tris, uint16 lod = 0);
    void triangle (const vector<blub::uint32> &tris, uint16 lod = 0);

    lod *lods();

    void aabb(blub::axisAlignedBox box);
    blub::axisAlignedBox aabb(void) const {return m_aabb;}

    graphic::mesh *mesh();

    void deleteTriangles(uint16 id);
    void deleteAllTriangles();
    void deleteAllVertices();

    void deleteAllGeometry();

    void reInitGraphic(material* mat);

    void staticVertexBuffer(bool set);
    // void updateVertices();
protected:

    /*void getLockedVertexIndexBase(unsigned char **vertexbase, int& numverts,PHY_ScalarType& type, int& stride,unsigned char **indexbase,int & indexstride,int& numfaces,PHY_ScalarType& indicestype,int subpart=0);
    void getLockedReadOnlyVertexIndexBase(const unsigned char **vertexbase, int& numverts,PHY_ScalarType& type, int& stride,const unsigned char **indexbase,int & indexstride,int& numfaces,PHY_ScalarType& indicestype,int subpart=0) const;
    void unLockVertexBase(int subpart);
    void unLockReadOnlyVertexBase(int subpart) const;
    int getNumSubParts() const;
    void preallocateVertices(int numverts);
    void preallocateIndices(int numindices);*/

    graphic::scene *scene() {return m_scene;}
    uint32 id() {return m_id;}
 private:
    void createSubmesh(Ogre::MeshPtr mesh, uint16 lod, string materialName);
    void createSharedVertexData(Ogre::MeshPtr mesh);
    void createVertexData(Ogre::VertexData* data, const lod *work);
    // void updateVerticesSubmesh(Ogre::SubMesh* mesh);
    // void initialise();
    void initialiseMesh(Ogre::MeshPtr mesh);
/*
    void begin();
    void end();
*/
    graphic::scene *m_scene;

    // bool m_begin;

    lod *m_lods;

    static uint32 g_idCounter;
    uint32 m_id;

    graphic::mesh *m_mesh;
    Ogre::MeshPtr m_mainMesh;
    Ogre::MeshPtr m_lodMesh[2];

    blub::axisAlignedBox m_aabb;

    uint16 m_numLod;

    bool m_staticVertexBuffer;


};

}
}

#endif // MANUALOBJECT_HPP
