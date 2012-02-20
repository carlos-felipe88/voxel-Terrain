#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <OGRE/OgreQuaternion.h>
#include <bullet/LinearMath/btQuaternion.h>

#include "blub/core/globals.hpp"
#include "blub/core/vector3.hpp"
#include "blub/core/matrix4x4.hpp"


namespace blub
{

    class quaternion : public Ogre::Quaternion
    {
    public:
        quaternion()
            : Ogre::Quaternion(1, 0, 0, 0)
        {;}
        quaternion(const btQuaternion& quad)
            : Ogre::Quaternion(quad.getW(), quad.getX(), quad.getY(), quad.getZ())
        {;}
        quaternion(Ogre::Quaternion quad)
            : Ogre::Quaternion(quad)
        {;}
        quaternion(real _x, real _y, real _z, real _w)
            : Ogre::Quaternion(_w, _x, _y, _z)
        {;}
        quaternion(const real rfAngle, const vector3& rkAxis)
            : Ogre::Quaternion(Ogre::Radian(rfAngle), rkAxis)
        {;}
        quaternion(vector3 vDir)
        {
            vector3 vUp (vDir);
            vector3 vDirection (1, 0, 0);
            vector3 vRight = vUp.crossProduct(vDirection);


            // Step 2. Put the three vectors into the matrix to bulid a basis rotation matrix
            // This step isnt necessary, but im adding it because often you would want to convert from matricies to quaternions instead of vectors to quaternions
            // If you want to skip this step, you can use the vector values directly in the quaternion setup below
            matrix4x4 mBasis= matrix4x4(vRight.x, vRight.y, vRight.z, 0.0f,
                                        vUp.x, vUp.y, vUp.z, 0.0f,
                                        vDirection.x, vDirection.y, vDirection.z, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f);

            // Step 3. Build a quaternion from the matrix
            w = (float)Ogre::Math::Sqrt(1.0f + mBasis[0][0] + mBasis[1][1] + mBasis[2][2]) / 2.0f;
            double dfWScale = w * 4.0;
            x = (float)((mBasis[2][1] - mBasis[1][2]) / dfWScale);
            y = (float)((mBasis[0][2] - mBasis[2][0]) / dfWScale);
            z = (float)((mBasis[1][0] - mBasis[0][1]) / dfWScale);
        }

        inline operator btQuaternion() const
        {return btQuaternion(x, y, z, w);}
        /*quaternion operator* (const quaternion& rkQ) const
        {
            return quaternion(Ogre::Quaternion(w, x, y, z) * Ogre::Quaternion(rkQ.w, rkQ.x, rkQ.y, rkQ.z));
        }*/
    };

}

#endif // QUATERNION_HPP
