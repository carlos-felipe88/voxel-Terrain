#include "rigidEntity.hpp"
#include "blub/core/string.hpp"
#include "blub/core/log.hpp"

using namespace blub;
using namespace blub::physic;

rigidEntity::rigidEntity()
    : m_system(0), m_collisionShape(0), m_rigidBody(0)
{;}

rigidEntity::rigidEntity(root *sys, const shape::shape *shape, real mass)
    : m_system(sys)
    , m_transform(btQuaternion(0,0,0,1), vector3(0, 0, 0))
{
    BOUT("rigidEntity::rigidEntity(root *sys, const shape::shape *shape, real mass)");
    m_collisionShape = shape->_shape();
    btVector3 fallInertia = btVector3(0, 0, 0);
    if (mass > 0)
        m_collisionShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,this,m_collisionShape,fallInertia);
    m_rigidBody = new btRigidBody(rigidBodyCI);
    sys->_dynamicsWorld()->addRigidBody(m_rigidBody);
}

btScalar rigidEntity::addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1,int index1)
{
    m_sigContact(cp.getPositionWorldOnA());
    (void)colObj0;
    (void)partId0;
    (void)index0;
    (void)colObj1;
    (void)partId1;
    (void)index1;
    m_isCollidating = true;
    return 0;
}

rigidEntity::~rigidEntity()
{
    BOUT("~rigidEntity()");
    if (m_system)
    {
        mutexLocker locker(*m_system);
        (void)locker;
        m_system->_dynamicsWorld()->removeRigidBody(m_rigidBody);
        delete m_rigidBody;
        m_frameListenerConnection.disconnect();
        // delete m_collisionShape;
    }
}

void rigidEntity::getWorldTransform(btTransform& worldTrans ) const
{
    worldTrans = m_transform;
}

void rigidEntity::setWorldTransform(const btTransform& worldTrans)
{
    m_transform = worldTrans;
}

void rigidEntity::applyForce(vector3 force)
{
    if (force.squaredLength() == 0.0)
        return;
    m_rigidBody->applyCentralForce(force);
    m_rigidBody->activate();
}

void rigidEntity::applyImpulse(vector3 imp)
{
    m_rigidBody->applyImpulse(imp, vector3(0));
}

void rigidEntity::velocity(vector3 vel)
{
    m_rigidBody->setLinearVelocity(vel);
    m_rigidBody->activate();
}

void rigidEntity::angularFactor(vector3 fac)
{
    m_rigidBody->setAngularFactor(fac);
    m_rigidBody->activate();
}

blub::vector3 rigidEntity::velocity()
{
    return m_rigidBody->getLinearVelocity();
}

blub::vector3 rigidEntity::angularFactor()
{
    return m_rigidBody->getAngularFactor();
}

void rigidEntity::enableCollisionTest(bool en)
{
    if (en)
        m_frameListenerConnection = m_system->addFrameListener(boost::bind(&rigidEntity::updateCollision, this, _1));
    else
        m_frameListenerConnection.disconnect();
}

bool rigidEntity::collidating()
{
    return m_isCollidating;
}

bool rigidEntity::updateCollision(real time)
{
    (void)time;
    m_isCollidating = false;
    // m_system->_dynamicsWorld()->contactTest(m_rigidBody, *this);
    return true;
}
