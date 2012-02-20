#ifndef MULTISPHERE_HPP
#define MULTISPHERE_HPP

#include <bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include "blub/physic/shape/shape.hpp"
#include "blub/core/vector.hpp"
#include "blub/core/sphere.hpp"

namespace blub
{
namespace physic
{
    class rigidEntity;
namespace shape
{

class multiSphere : public shape
{
public:
    multiSphere(vector<blub::sphere> spheres)
    {
        btVector3 *positions = new btVector3[spheres.count()];
        btScalar *radius = new btScalar[spheres.count()];
        uint16 ind(0);
        for (vector<blub::sphere>::iterator it = spheres.begin(); it != spheres.end(); ++it)
        {
            positions[ind] = it->center();
            radius[ind++] = it->radius();
        }
        m_shape = new btMultiSphereShape(positions, radius, ind);
        delete positions;
        delete radius;
    }

    ~multiSphere()
    {
        delete m_shape;
    }

protected:
    friend class blub::physic::rigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btMultiSphereShape *m_shape;
};

}
}
}

#endif // MULTISPHERE_HPP
