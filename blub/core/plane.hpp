#ifndef PLANE_HPP
#define PLANE_HPP

#include "vector3.hpp"
#include "line.hpp"

namespace blub
{

class plane : public Ogre::Plane
{
public:
    plane()
        : Ogre::Plane()
    {;}
    plane(vector3 p0, vector3 p1, vector3 p2)
        : Ogre::Plane(p0, p1, p2)
    {;}
    plane(vector3 pos, vector3 normal)
        : Ogre::Plane(normal, pos)
    {;}
    plane(real a, real b, real c, real d)
        : Ogre::Plane(a, b, c, d)
    {;}
    inline operator Ogre::Plane()
    {return Ogre::Plane(normal, d);}

    bool operator!=(const plane& rhs) const
    {
        return rhs.d != d || rhs.normal != normal;
    }

    bool intersects (const plane* other, line3 *result = 0)
    {
        (void)other;
        (void)result;
        // http://www.softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm#Intersection of 2 Planes
/*
        vector3 n1 = normal.normalisedCopy();
        vector3 n2 = other->normal.normalisedCopy();
        real d1 = d;
        real d2 = other->d;

        real dot = n1.dotProduct(n2);
        if (dot < 0.000001 && dot > -0.000001)*/
            return false; // parallell
/*
        vector3 u = n1.crossProduct(n2).normalisedCopy();

        real c = n1.dotProduct(n2);
        vector3 origin = ((d2*c-d1) / (1-c*c))*n1 + ((d1*c-d2) / (1-c*c))*n2;
        c = n1.absDotProduct(n2);
        origin = ((d2*c-d1) / (1-c*c))*n1 + ((d1*c-d2) / (1-c*c))*n2;
        origin = (d1*n2-d2*n1).crossProduct(u).normalisedCopy()/(u.squaredLength());
        origin = u.crossProduct(d1*n2-d2*n1).normalisedCopy()/(u.squaredLength()); // FUCK FUCK FUCK - NOT WORKING

        result->setOrigin(
                origin

                );
        result->setDirection( u );

        return true;*/
    }
};

}

#endif // PLANE_HPP

