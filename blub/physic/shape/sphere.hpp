#ifndef SHAPE_SPHERE_HPP
#define SHAPE_SPHERE_HPP

#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include "blub/physic/shape/shape.hpp"

namespace blub
{
namespace physic
{
    class rigidEntity;
namespace shape
{

class sphere : public shape
{
public:
    sphere(real rad)
    {
        m_shape = new btSphereShape(rad);
    }

    ~sphere()
    {
        delete m_shape;
    }

protected:
    friend class blub::physic::rigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btSphereShape *m_shape;
};

}
}
}


#endif // SHAPE_SPHERE_HPP
