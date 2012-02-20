#ifndef MULTISHAPE_HPP
#define MULTISHAPE_HPP

#include <bullet/BulletCollision/CollisionShapes/btCompoundShape.h>
#include "blub/physic/shape/shape.hpp"
#include "blub/core/vector.hpp"

namespace blub
{
namespace physic
{
    class rigidEntity;
namespace shape
{

class multiShape : public shape
{
public:
    multiShape()
    {
        m_shape = new btCompoundShape();
    }

    ~multiShape()
    {
        delete m_shape;
    }

    void add (shape *sh, vector3 offset)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(offset);
        m_shape->addChildShape(trans, sh->_shape());
    }

protected:
    friend class blub::physic::rigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btCompoundShape *m_shape;
};

}
}
}

#endif // MULTISHAPE_HPP
