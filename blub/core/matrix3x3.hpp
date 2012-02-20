#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include <OGRE/OgreMatrix3.h>

#include "blub/core/globals.hpp"

namespace blub
{

class matrix3x3 : public Ogre::Matrix3
{
public:
    matrix3x3()
    {;}
    matrix3x3(real m00, real m01, real m02,
              real m10, real m11, real m12,
              real m20, real m21, real m22)
        : Ogre::Matrix3(m00, m01, m02,
                        m10, m11, m12,
                        m20, m21, m22)
    {;}
    matrix3x3(Ogre::Matrix3 &mat)
        : Ogre::Matrix3(mat)
    {;}
};

}

#endif // MATRIX3X3_HPP
