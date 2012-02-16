#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "blub/core/globals.hpp"
#include "OGRE/OgreVector3.h"
#include "bullet/LinearMath/btVector3.h"

namespace blub
{

class vector3 : public Ogre::Vector3
{
public:
    vector3()
        : Ogre::Vector3(0.0)
    {
    }

    vector3(const Ogre::Vector3 &vec)
        : Ogre::Vector3(vec)
    {
    }

    vector3(const btVector3 &vec)
        : Ogre::Vector3(vec.x(), vec.y(), vec.z())
    {
    }

    vector3(const real fX, const real fY, const real fZ)
        : Ogre::Vector3(fX, fY, fZ)
    {
    }

    vector3(const real scaler)
        : Ogre::Vector3(scaler, scaler, scaler)
    {
    }

    inline operator btVector3() const
    {return btVector3(x, y, z);}
};

}

#endif // VECTOR3_HPP
