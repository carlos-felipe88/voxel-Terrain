#ifndef EDITSPHERE_HPP
#define EDITSPHERE_HPP

#include "blub/procedural/voxelMap/edit.hpp"
#include "blub/core/sphere.hpp"

namespace blub
{

namespace procedural
{

namespace voxelMap
{

class editSphere : public edit
{
public:
    editSphere(sphere sp, bool cut);

    axisAlignedBox aab(real scale) const;
    real calcVoxel(const vector3 &pos, const real &scale);
protected:
    static sphere scaleSphere(const sphere sp, const real scale);

    sphere m_sp;
};

}
}
}

#endif // EDITSPHERE_HPP
