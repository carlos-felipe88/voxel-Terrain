#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector3.hpp"
#include "axisAlignedBox.hpp"
#include <OGRE/OgreSphere.h>

namespace blub
{

    class sphere : public Ogre::Sphere
    {
    public:
        sphere()
            : Ogre::Sphere()
        {;}
        sphere(vector3 center, real radius)
            : Ogre::Sphere(center, radius)
        {;}
        bool contains(vector3 vec)
        {return intersects(vec);}

        real radius(void) const {return mRadius;}
        void radius(real rad) {setRadius(rad);}
        vector3 center(void) const {return mCenter;}
        void center(vector3 cen) {setCenter(cen);}

        const vector3 getCenter(void) const { return mCenter; }

        bool inherts(axisAlignedBox aabb) const {
            const Ogre::Vector3 *corners = aabb.getAllCorners();
            for (uint16 ind = 0; ind < 8; ++ind)
            {
                if (corners[ind].squaredDistance(getCenter()) > getRadius()*getRadius())
                    return false;
            }
            return true;
        }
    };

}

#endif // SPHERE_HPP
