#ifndef MESH_HPP
#define MESH_HPP

#include <bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <bullet/BulletCollision/CollisionShapes/btTriangleMesh.h>

#include "blub/physic/shape/shape.hpp"
#include "blub/core/vector.hpp"

namespace blub
{
namespace physic
{
    class rigidEntity;
namespace shape
{

class mesh : public shape
{
public:
    mesh(const vector<vector3> *vertices, const vector<triangle> *triangles)
    {
        for (vector<triangle>::const_iterator it = triangles->constBegin(); it != triangles->constEnd(); ++it)
        {
            m_mesh.addTriangle(vertices->at(it->ind[0]), vertices->at(it->ind[1]), vertices->at(it->ind[2]), false);
        }
        m_shape = new btBvhTriangleMeshShape(&m_mesh,false,true);
    }
    mesh(const vector3 *vertices, triangle *tris, uint32 numTriangles)
    {
        for (uint32 ind = 0; ind < numTriangles; ++ind)
        {
            m_mesh.addTriangle(vertices[tris[ind].ind[0]], vertices[tris[ind].ind[1]], vertices[tris[ind].ind[2]], false);
        }
        m_shape = new btBvhTriangleMeshShape(&m_mesh,false,true);
    }

    ~mesh()
    {
        delete m_shape;
    }

protected:
    friend class blub::physic::rigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btBvhTriangleMeshShape *m_shape;
    btTriangleMesh m_mesh;
};

}
}
}
#endif // MESH_HPP
