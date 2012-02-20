#include "rope.hpp"
#include "blub/physic/rigidEntity.hpp"

#include <bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include "bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"

using namespace blub::physic;

rope::rope(root *sys)
    : m_sys(sys)
{
/*    btSoftRigidDynamicsWorld* world = static_cast<btSoftRigidDynamicsWorld*>(sys->_dynamicsWorld());
    m_rope = btSoftBodyHelpers::CreateRope(world->getWorldInfo(), btVector3(30, 10, 0), btVector3(0, 0, 0), 10, 1);
    m_rope->setTotalMass(1);
    world->addSoftBody(m_rope);*/



    /*for (uint16 ind = 0; ind < 12; ++ind)
    {
        real mass(1);
        if (ind == 0)
            mass = 0;
        m_nodes[ind].body = new rigidEntity(m_sys, blub::sphere(vector3(0, 22-ind*2, 0), 0.5), mass);
        if (ind > 0)
        {
            m_nodes[ind].constraint = new btPoint2PointConstraint(*m_nodes[ind-1].body->_rigidBody(), *m_nodes[ind].body->_rigidBody(), btVector3(0, 0, 0), btVector3(0, 2, 0));
            m_nodes[ind].constraint->m_setting.m_damping = 0.01;
            m_sys->_dynamicsWorld()->addConstraint(m_nodes[ind].constraint, false);
        }
    }*/
}


void rope::appendAnchor(rigidEntity *ent)
{
    m_sys->_dynamicsWorld()->addConstraint(new btPoint2PointConstraint(*m_nodes[11].body->_rigidBody(), *ent->_rigidBody(), btVector3(0, 0, 0), btVector3(0, 0, 0)), false);
}

blub::vector<blub::vector3> rope::nodes()
{
//    bt6DofFlags
    blub::vector<blub::vector3> result;
    for (uint16 ind = 0; ind < 12; ++ind)
    {
        result.push_back(m_nodes[ind].body->position());
    }
    return result;
}

