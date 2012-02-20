#ifndef ROOT_HPP
#define ROOT_HPP

#include <bullet/btBulletDynamicsCommon.h>
// #include <bullet/BulletSoftBody/btSoftBodyHelpers.h>

#include "blub/core/frameListener.hpp"
#include "blub/core/mutex.hpp"

namespace blub
{
namespace physic
{
    class root : public frameListener, public mutex
    {
    public:
        root();
        ~root();
    protected:
        friend class rigidEntity;
        friend class rope;

        btCollisionConfiguration* _collisionConfiguration() {return m_collisionConfiguration;}
//        btSoftBodyWorldInfo* _softBodyWorldInfo() {return &m_softBodyWorldInfo;}
    private:
        bool frame(real delta);
        btDiscreteDynamicsWorld* _dynamicsWorld() {return m_root;}

        btBroadphaseInterface* m_broadphase;
        btDefaultCollisionConfiguration* m_collisionConfiguration;
        btCollisionDispatcher* m_dispatcher;
        btSequentialImpulseConstraintSolver* m_solver;

        btDiscreteDynamicsWorld* m_root;
    };

}
}

#endif // ROOT_HPP
