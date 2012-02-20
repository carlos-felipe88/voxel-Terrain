#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <OGRE/OgreColourValue.h>

namespace blub
{

class colour : public Ogre::ColourValue
{
public:
    colour()
        : Ogre::ColourValue()
    {;}
    colour(float r, float g, float b, float a)
        : Ogre::ColourValue(r, g, b, a)
    {;}
    colour(Ogre::ColourValue col)
        : Ogre::ColourValue(col)
    {;}
};

}

#endif // COLOUR_HPP
