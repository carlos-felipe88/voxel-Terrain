#ifndef MAP3_HPP
#define MAP3_HPP

#include "blub/core/axisAlignedBox.hpp"
#include "blub/procedural/voxel.hpp"

namespace blub
{

namespace procedural
{

namespace voxelMap
{
    class map3
    {
    public:
        struct deltaPos
        {
            deltaPos(int32 x, int32 y, int32 z)
                : deltaX(x)
                , deltaY(y)
                , deltaZ(z)
            {
            }

            int32 deltaX;
            int32 deltaY;
            int32 deltaZ;
        };

        map3(); // invalid reinit with snd constructor;
        map3(int32 offsetX, int32 offsetY, int32 offsetZ, uint32 sizeX, uint32 sizeY, uint32 sizeZ);
        ~map3();


        voxel *getVoxel(int32 x, int32 y, int32 z);

        // void setVoxel (int32 x, int32 y, int32 z, voxel data);
        void setVoxelIfLowerZero(int32 x, int32 y, int32 z, voxel data);
        // void setVoxelIfHigherEqualOne(int32 x, int32 y, int32 z, voxel data);
        void setVoxelIfHigherZero(int32 x, int32 y, int32 z, voxel data);
        // void removeVoxel(int32 x, int32 y, int32 z);

        uint32 numAboveZero();

        void begin();
        void end();

        int32 offsetX();
        int32 offsetY();
        int32 offsetZ();
    private:
        uint32 posMap(int32 x, int32 y, int32 z);
        void calcNumAboveZero(uint32 ind, const voxel &data);
        uint32 size();

        uint32 m_sizeX;
        uint32 m_sizeY;
        uint32 m_sizeZ;

        int32 m_offsetX;
        int32 m_offsetY;
        int32 m_offsetZ;

        voxel* m_voxel;

        uint32 m_numAboveZero;
    };

}

}

}

#endif // MAP3_HPP
