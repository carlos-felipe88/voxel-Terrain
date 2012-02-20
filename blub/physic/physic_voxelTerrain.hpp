#ifndef PHYSIC_VOXELTERRAIN_HPP
#define PHYSIC_VOXELTERRAIN_HPP

#include "blub/core/hashMap.hpp"
#include "blub/core/vector3.hpp"

namespace blub {
namespace procedural {
    class voxelTerrain;
}

namespace physic {
class rigidEntity;
class root;
namespace shape {
    class mesh;
}

class voxelTerrain
{
public:
    typedef blub::procedural::voxelTerrain t_voxelTerrain;
    voxelTerrain(root* physics, t_voxelTerrain* terrain);

private:
    // voxelMapThread
    void voxelMapPartChanged(blub::uint16 lod, blub::int32 id);
    // physicThread
    void voxelMapPartChangedPhysic(blub::int32 id, blub::physic::shape::mesh* shape, blub::vector3 position);


    struct rigid
    {
        blub::physic::rigidEntity* entity;
        blub::physic::shape::mesh* shape;
    };
    typedef blub::hashMap<blub::int32, rigid> t_partRigidMap;
    t_partRigidMap m_partRigids;

    t_voxelTerrain* m_ground;
    root* m_physics;
};


}
}


#endif // PHYSIC_VOXELTERRAIN_HPP
