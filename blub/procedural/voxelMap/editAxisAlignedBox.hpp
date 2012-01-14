#ifndef EDITAXISALIGNEDBOX_HPP
#define EDITAXISALIGNEDBOX_HPP

#include "blub/procedural/voxelMap/edit.hpp"
#include "blub/core/axisAlignedBox.hpp"

namespace blub
{

namespace procedural
{

namespace voxelMap
{

class editAxisAlignedBox : public edit
{
public:
    editAxisAlignedBox(axisAlignedBox abb, bool cut);

    axisAlignedBox aab(real scale) const;
    real calcVoxel(const vector3 &pos, const real &scale);
protected:
    static axisAlignedBox scaleAab(const axisAlignedBox& aab, const real scale);

    axisAlignedBox m_abb;
};
}
}
}
#endif // EDITAXISALIGNEDBOX_HPP
