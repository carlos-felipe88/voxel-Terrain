#ifndef RIGIDENTITY_HPP
#define RIGIDENTITY_HPP

#include "bullet/LinearMath/btMotionState.h"
#include "blub/core/sphere.hpp"
#include "blub/core/vector.hpp"
#include "blub/core/vector3.hpp"
#include "blub/core/quaternion.hpp"
#include "blub/core/triangle.hpp"
#include "blub/physic/root.hpp"

#include "blub/physic/shape/shape.hpp"

#include "blub/graphic/manualObject.hpp"
#include "blub/core/mutexLocker.hpp"

namespace blub
{
namespace physic
{

class rigidEntity : public btMotionState, public btCollisionWorld::ContactResultCallback
{
public:
    rigidEntity();
    rigidEntity(root *sys, const shape::shape *shape, real mass);

    ~rigidEntity();

    vector3 position() const {return m_transform.getOrigin();}
    void position(const vector3 pos) {m_transform.setOrigin(pos);m_rigidBody->setWorldTransform(m_transform);}
    quaternion rotation() const {return m_transform.getRotation();}
    void rotation(const quaternion rot) {m_transform.setRotation(rot);m_rigidBody->setWorldTransform(m_transform);}

    void applyForce(vector3 force);
    void applyImpulse(vector3 imp);
    void velocity(vector3 vel);
    void angularFactor(vector3 fac);

    vector3 velocity();
    vector3 angularFactor();

    void enableCollisionTest(bool en = true);
    bool collidating();

    typedef boost::signals2::signal<void (vector3)> t_sigContactListener;
    boost::signals2::connection addContactListener(const t_sigContactListener::slot_type &subscriber)
    {
        return m_sigContact.connect(subscriber);
    }

protected:
    virtual void getWorldTransform(btTransform& worldTrans ) const;
    virtual void setWorldTransform(const btTransform& worldTrans);
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1,int index1);

    bool updateCollision(real time);

    friend class rope;

private:
    btRigidBody *_rigidBody() {return m_rigidBody;}

    // graphic::manualObject m_manual;
    root *m_system;
    btTransform m_transform;
    btCollisionShape *m_collisionShape;
    btRigidBody *m_rigidBody;

    bool m_isCollidating;

    t_sigContactListener m_sigContact;
    boost::signals2::connection m_frameListenerConnection;
};

}
}

#endif // RIGIDENTITY_HPP
