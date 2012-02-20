#ifndef LINE_HPP
#define LINE_HPP

#include "blub/core/log.hpp"
#include "vector3.hpp"
#include <OGRE/OgreRay.h>

namespace blub
{

class line3 : public Ogre::Ray
{
public:
    line3()
    {;}
    line3(vector3 point, vector3 dir)
        : Ogre::Ray(point, dir)
    {;}

    bool intersects (const vector3 &point)
    {
        real k = 0;
        if (mDirection.x != 0 && point.x - mOrigin.x != 0)
            k = (point.x - mOrigin.x) / mDirection.x;
        else if (mDirection.y != 0 && point.y - mOrigin.y != 0)
            k = (point.y - mOrigin.y) / mDirection.y;
        else if (mDirection.z != 0 && point.z - mOrigin.z != 0)
            k = (point.z - mOrigin.z) / mDirection.z;
        else
        {
            BERROR("line3::intersects");
            return false;
        }
        return getPoint(k) == point;
    }

    bool intersects (const line3& other, vector3 *point = 0)
    {
        vector3 p1 = mOrigin;
        vector3 p2 = other.mOrigin;
        vector3 v1 = mDirection;
        vector3 v2 = other.mDirection;

        /*if (v1.y*v2.x - v1.x*v2.y == 0)
            return false;*/
        real t(0.0);
        if (v2.x - v1.x != 0)
        {
            t = (p1.x - p2.x) / (v2.x - v1.x);
        }
        else
        if (v2.y - v1.y != 0)
        {
            t = (p1.y - p2.y) / (v2.y - v1.y);
        }
        else
        if (v2.z - v1.z != 0)
        {
            t = (p1.z - p2.z) / (v2.z - v1.z);
        }
        if (t == 0)
            return false;
        vector3 result;
        point->x = p1.x + v1.x * t;
        point->y = p1.y + v1.y * t;
        point->z = p1.z + v1.z * t;
        return true;
    }

};

}

#endif // LINE_HPP
