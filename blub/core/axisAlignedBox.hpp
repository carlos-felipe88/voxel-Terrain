#ifndef AXISALIGNEDBOX_HPP
#define AXISALIGNEDBOX_HPP

#include "vector3.hpp"
#include <OGRE/OgreAxisAlignedBox.h>

namespace blub
{
    class axisAlignedBox : public Ogre::AxisAlignedBox
    {
    public:
        axisAlignedBox ()
            : Ogre::AxisAlignedBox()
        {
        }
        axisAlignedBox (vector3 min, vector3 max)
            : Ogre::AxisAlignedBox(min, max)
        {
        }

        axisAlignedBox (real minX, real minY, real minZ, real maxX, real maxY, real maxZ)
            : Ogre::AxisAlignedBox(minX, minY, minZ, maxX, maxY, maxZ)
        {
        }

        vector3 size() const {return getSize();}
        vector3 center() const {return getCenter();}
        vector3 minimum() const {return getMinimum();}
        vector3 maximum() const {return getMaximum();}

        axisAlignedBox move(const vector3& vec) const {return axisAlignedBox(minimum()+vec, maximum()+vec);}
    };
}

#endif // AXISALIGNEDBOX_HPP
