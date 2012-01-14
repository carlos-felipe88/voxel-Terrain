#ifndef EDIT_H
#define EDIT_H

#include "blub/core/axisAlignedBox.hpp"

namespace blub
{

namespace procedural
{

namespace voxelMap
{
class map3;

class edit
{
public:
    edit(bool cut) : m_cut(cut) {;}

    virtual axisAlignedBox aab(real scale) const = 0;
    // mp has to be locked! AND begin had to be called
    void createVoxel(map3 *mp,
                     const int32 startX,
                     const int32 startY,
                     const int32 startZ,
                     const int32 endX,
                     const int32 endY,
                     const int32 endZ,
                     const real scale);
    virtual real calcVoxel(const vector3& pos, const real &scale) = 0;
protected:
    bool m_cut;
};

}
}
}

#endif // EDIT_H
