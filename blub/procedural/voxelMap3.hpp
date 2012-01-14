#ifndef VOXELMAP3_HPP
#define VOXELMAP3_HPP

#include "blub/core/globals.hpp"
#include "blub/procedural/voxel.hpp"

#include "blub/core/axisAlignedBox.hpp"
#include "blub/core/sphere.hpp"
#include "blub/core/mutexLocker.hpp"

#include "blub/core/triangle.hpp"
#include "blub/core/triangleVector3.hpp"
#include "blub/core/quadVector3.hpp"
#include "blub/core/map.hpp"

#include "blub/core/log.hpp"
#include "blub/core/todoListener.hpp"
#include "blub/core/sharedPointer.hpp"

#include "blub/procedural/voxelMap/map3.hpp"
#include "blub/procedural/voxelMap/part3.hpp"

#include <boost/signals2.hpp>

namespace blub
{

namespace procedural
{

    class voxelMap3 : protected todoListener, public mutex
    {
    public:
        typedef voxelMap::part3 myPart;
        typedef hashMap<int32, sharedPointer<myPart> > t_part;

        // threadsafe
        voxelMap3(uint32 sizeX, uint32 sizeY, uint32 sizeZ, bool calcTransvoxel);

        // threadsafe
        sharedPointer<myPart> getPart(int32 id);
        // lock class while working on parts!!! so that part cannot get deleted while working on it
        hashMap<int32, sharedPointer<myPart> > * getAllParts();

        // threadsafe
        void edit (sharedPointer<voxelMap::edit> ed, bool calcGeometry);

        // threadsafe
        typedef boost::signals2::signal<void (int32)> t_signalPartChanged;
        void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
        {m_sigPartChanged.connect(subscriber);}

        // threadsafe
        typedef boost::signals2::signal<void (int32, bool)> t_signalPartAddedOrRemoved;
        void addPartAddedOrRemovedListener(const t_signalPartAddedOrRemoved::slot_type &subscriber)
        {m_sigPartAddedOrRemoved.connect(subscriber);}

        // threadsafe
        void scale(real sc);

        // threadsafe
        void calculatePart(int32 part);
    private:
        // voxelMap3
        void todoEdit (sharedPointer<voxelMap::edit> ed, bool calculateGeometry);

        static int32 getKey(int32 posX, int32 posY, int32 posZ);
        sharedPointer<myPart> getPart(int32 posX, int32 posY, int32 posZ);

        // voxelMap3
        void aabbIntersectsParts(const axisAlignedBox& aabb, int32 *firstX, int32 *firstY, int32 *firstZ, int32 *lastX, int32 *lastY, int32 *lastZ);

    protected:
        // voxelMap3
        // locks class
        void removePart(int32 id);
        void removePart(int32 posX, int32 posY, int32 posZ);
        // voxelMap3
        // locks class
        sharedPointer<myPart> addPart(int32 posX, int32 posY, int32 posZ);

        void partChanged(int32 id);
        void toDoCalculate(int32 id);

        t_part m_parts;
    private:
        t_signalPartChanged m_sigPartChanged;
        t_signalPartAddedOrRemoved m_sigPartAddedOrRemoved;

        int32 m_sizeX;
        int32 m_sizeY;
        int32 m_sizeZ;

        real m_scale;

        bool m_calcTransvoxel;
    };

}
}

#endif // VOXELMAP3_HPP
