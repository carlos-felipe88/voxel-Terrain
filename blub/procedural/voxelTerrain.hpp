#ifndef VOXELTERRAIN_HPP
#define VOXELTERRAIN_HPP

#include "voxelMap3.hpp"

namespace blub
{
namespace procedural
{

    class voxelMap3;

    class voxelTerrain
    {
    public:
        voxelTerrain(uint16 numLod, uint32 sizeX, uint32 sizeY, uint32 sizeZ);
        ~voxelTerrain();

        // threadsafe
        void edit (sharedPointer<voxelMap::edit> ed, bool calcGeometry = false);
        // void edit (axisAlignedBox aab, bool cut);

        // lod count does not change, so threadsafe
        uint16 numLod();

        // threadsafe
        typedef boost::signals2::signal<void (uint16, int32)> t_signalPartChanged;
        void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
        {m_sigPartChanged.connect(subscriber);}

        // threadsafe
        typedef boost::signals2::signal<void (uint16, int32, bool)> t_signalPartAddedOrRemoved;
        void addPartVoxelChangedListener(const t_signalPartAddedOrRemoved::slot_type &subscriber)
        {m_sigPartAddedOrRemoved.connect(subscriber);}

        voxelMap3 *voxelMap(uint16 lod);
        typedef sharedPointer<voxelMap::part3> t_part3Ptr;
        t_part3Ptr part(uint16 lod, int32 par);
    private:
        void partChanged (uint16 map, int32 part);
        void partRemovedOrAdded (uint16 map, int32 part, bool remove);

        t_signalPartChanged m_sigPartChanged;
        t_signalPartAddedOrRemoved m_sigPartAddedOrRemoved;

        vector<voxelMap3 *> m_maps;
    };

}
}

#endif // VOXELTERRAIN_H
