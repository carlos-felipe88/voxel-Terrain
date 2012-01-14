#include "voxelTerrain.hpp"

#include <blub/core/vector.hpp>

using namespace blub;
using namespace blub::procedural;

voxelTerrain::voxelTerrain(uint16 numLod, uint32 sizeX, uint32 sizeY, uint32 sizeZ)
{
    real factor(1.);
    for (uint16 ind = 0; ind < numLod; ++ind)
    {
        voxelMap3 *newOne = new voxelMap3(sizeX, sizeY, sizeZ, ind > 0);
        newOne->scale(factor);
        factor*=2.;
        m_maps.push_back(newOne);
        newOne->addPartChangedListener(boost::bind(&voxelTerrain::partChanged, this, ind, _1));
        newOne->addPartAddedOrRemovedListener(boost::bind(&voxelTerrain::partRemovedOrAdded, this, ind, _1, _2));
    }
}

voxelTerrain::~voxelTerrain()
{
    BOUT("~voxelTerrain()");
    for (uint16 ind = 0; ind < m_maps.size(); ++ind)
    {
        delete m_maps[ind];
    }
}

void voxelTerrain::partChanged(uint16 map, int32 part)
{
    m_sigPartChanged(map, part);
}

void voxelTerrain::partRemovedOrAdded(uint16 map, int32 part, bool remove)
{
    m_sigPartAddedOrRemoved(map, part, remove);
}

void voxelTerrain::edit(sharedPointer<voxelMap::edit> ed, bool calcGeometry)
{
    for (uint16 ind = 0; ind < m_maps.size(); ++ind)
    {
        voxelMap3 *work = m_maps.at(ind);
        work->edit(ed, calcGeometry);
    };
}

voxelMap3 *voxelTerrain::voxelMap(uint16 lod)
{
    return m_maps[lod];
}

voxelTerrain::t_part3Ptr voxelTerrain::part(uint16 lod, int32 par)
{
    return voxelMap(lod)->getPart(par);
}

uint16 voxelTerrain::numLod()
{
    return m_maps.size();
}

