#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "blub/core/globals.hpp"
#include <OGRE/OgreVector2.h>

namespace blub
{

class vector2 : public Ogre::Vector2
{
public:
    vector2()
        : Ogre::Vector2()
    {
    }

    vector2(Ogre::Vector2 vec)
        : Ogre::Vector2(vec)
    {
    }

    vector2(const real fX, const real fY)
        : Ogre::Vector2(fX, fY)
    {
    }

    vector2(const real scaler)
        : Ogre::Vector2(scaler, scaler)
    {
    }
};

}

#endif // VECTOR2_HPP
