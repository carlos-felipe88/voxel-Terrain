#include "root.hpp"
#include "blub/core/vector3.hpp"
#include "blub/core/log.hpp"

#include "bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

using namespace blub::physic;
using namespace blub;

root::root()
{
    BOUT("init physics");
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;

    m_root = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);

    addFrameListener(boost::bind(&root::frame, this, _1));
    m_root->setGravity(vector3(0, 0, 0));

/*    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    m_root->addRigidBody(groundRigidBody);
*/
    run(100, true);
}

root::~root()
{
    BOUT("delete physics");
    delete m_root;
    delete m_solver;
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_broadphase;
}

bool root::frame(real delta)
{
    (void)delta;
    lock();

    btAlignedObjectArray<class btCollisionObject*> objects = m_root->getCollisionObjectArray();
    for (uint32 ind = 0; ind < (unsigned)objects.size(); ++ind)
    {
        {
            btRigidBody* body = static_cast<btRigidBody*>(objects.at(ind));
            vector3 pos = body->getWorldTransform().getOrigin();
            pos.normalise();
            pos*=-10;
            body->setGravity(pos);
        }
    }

    m_root->stepSimulation(delta, 0);
    unlock();
    return true;
}
