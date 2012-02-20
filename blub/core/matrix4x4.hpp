#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <OGRE/OgreMatrix4.h>

#include "blub/core/globals.hpp"

namespace blub
{

class matrix4x4 : public Ogre::Matrix4
{
public:
    matrix4x4()
    {;}
    matrix4x4(real m00, real m01, real m02, real m03,
              real m10, real m11, real m12, real m13,
              real m20, real m21, real m22, real m23,
              real m30, real m31, real m32, real m33)
        : Ogre::Matrix4(m00, m01, m02, m03,
                        m10, m11, m12, m13,
                        m20, m21, m22, m23,
                        m30, m31, m32, m33)
    {;}
};

}

#endif // MATRIX4X4_HPP
