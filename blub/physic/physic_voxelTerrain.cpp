#include "physic_voxelTerrain.hpp"

#include "blub/procedural/voxelTerrain.hpp"
#include "blub/physic/shape/mesh.hpp"
#include "blub/physic/rigidEntity.hpp"
#include "blub/physic/root.hpp"

using namespace blub;
using namespace blub::physic;

voxelTerrain::voxelTerrain(root *physics, t_voxelTerrain *terrain)
    : m_ground(terrain)
    , m_physics(physics)
{
    m_ground->addPartChangedListener(boost::bind(&voxelTerrain::voxelMapPartChanged, this, _1, _2));
}

void voxelTerrain::voxelMapPartChanged(uint16 lod, int32 id)
{
    blub::sharedPointer<blub::procedural::voxelMap::part3> part = m_ground->part(lod, id);
    if (!part.isNull())
    {
        blub::triangle* triangles = part->hullTriangles(0);
        if (part->numTriangles(0) > 0)
        {
            blub::vector3* vertices = part->hullVertices();
            blub::physic::shape::mesh* shape = new blub::physic::shape::mesh(vertices, triangles, part->numTriangles(0));
            m_physics->addToDo(boost::bind(&voxelTerrain::voxelMapPartChangedPhysic, this, id, shape, part->aabb().minimum()));
        }
        else
            m_physics->addToDo(boost::bind(&voxelTerrain::voxelMapPartChangedPhysic, this, id, static_cast<blub::physic::shape::mesh*>(0), part->aabb().minimum()));
    }
}

void voxelTerrain::voxelMapPartChangedPhysic(blub::int32 id, blub::physic::shape::mesh *shape, blub::vector3 position)
{
    // delete old one
    t_partRigidMap::iterator it = m_partRigids.find(id);
    if (it != m_partRigids.end())
    {
        delete it.value().entity;
        delete it.value().shape;
        m_partRigids.erase(it);
    }
    if (shape)
    {
        // get new one
        // add/replace old one
        rigid rig;
        rig.entity = new blub::physic::rigidEntity(m_physics, shape, 0);
        rig.entity->position(position);
        rig.shape = shape;
        m_partRigids[id] = rig;
    }
}
