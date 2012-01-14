#include "blub/procedural/voxelMap/edit.hpp"
#include "blub/procedural/voxelMap/map3.hpp"

using namespace blub;
using namespace blub::procedural::voxelMap;

void edit::createVoxel(map3 *mp,
                 const int32 _startX,
                 const int32 _startY,
                 const int32 _startZ,
                 const int32 _endX,
                 const int32 _endY,
                 const int32 _endZ,
                 const real scale)
{
    axisAlignedBox aabb = aab(scale);

    int32 startX = (aabb.minimum().x)-1;
    if (startX < _startX)
        startX = _startX;
    int32 endX = aabb.maximum().x+1;
    if (endX > _endX)
        endX = _endX;

    int32 startY = (aabb.minimum().y)-1;
    if (startY < _startY)
        startY = _startY;
    int32 endY = aabb.maximum().y+1;
    if (endY > _endY)
        endY = _endY;

    int32 startZ = (aabb.minimum().z)-1;
    if (startZ < _startZ)
        startZ = _startZ;
    int32 endZ = aabb.maximum().z+1;
    if (endZ > _endZ)
        endZ = _endZ;

    // real buffer = 1./scale;

    int32 localX((startX-_startX)-mp->offsetX());
    // loop voxel in sphere
    for (int32 x = startX; x < endX; ++x, ++localX)
    {
        int32 localY((startY-_startY)-mp->offsetY());
        for (int32 y = startY; y < endY; ++y, ++localY)
        {
            int32 localZ((startZ-_startZ)-mp->offsetZ());
            for (int32 z = startZ; z < endZ; ++z, ++localZ)
            {
                real result = calcVoxel(vector3(x, y, z), scale);
//                 result*=scale;
                if (result < -1.)
                    continue;
                if (result > 1.)
                    result = 1.;

                voxel newVoxel;
                if (!m_cut)
                {
                    newVoxel.scalar = result;
                    mp->setVoxelIfLowerZero(localX, localY, localZ, newVoxel);
                }
                else
                {
                    newVoxel.scalar = -result;
                    mp->setVoxelIfHigherZero(localX, localY, localZ, newVoxel);
                }
            }
        }
    }
}
