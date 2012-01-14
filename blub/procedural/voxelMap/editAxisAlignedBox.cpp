#include "editAxisAlignedBox.hpp"
#include "blub/procedural/voxelMap/map3.hpp"
// #include "blub/core/log.hpp"

using namespace blub;
using namespace blub::procedural::voxelMap;

editAxisAlignedBox::editAxisAlignedBox(axisAlignedBox abb, bool cut)
    : edit(cut)
    , m_abb(abb)
{
}

axisAlignedBox editAxisAlignedBox::aab(real scale) const
{
    axisAlignedBox aab = scaleAab(m_abb, scale);
    axisAlignedBox result (aab.minimum()-vector3(1), aab.maximum()+vector3(1));
    return result;
}

real editAxisAlignedBox::calcVoxel(const vector3 &pos, const real &scale)
{
    axisAlignedBox aab = scaleAab(m_abb, scale);
    if (aab.contains(pos))
        return 1.;
    else
        return -2.;
}

axisAlignedBox editAxisAlignedBox::scaleAab(const axisAlignedBox& aab, const real scale)
{
    return axisAlignedBox (aab.minimum()/scale, aab.maximum()/scale);
}


