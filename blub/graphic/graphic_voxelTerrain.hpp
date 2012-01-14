#ifndef GRAPHIC_VOXELTERRAIN_HPP
#define GRAPHIC_VOXELTERRAIN_HPP

#include "blub/core/map.hpp"
#include "blub/core/mutex.hpp"
#include "blub/core/octree.hpp"
#include "blub/core/frameListener.hpp"
#include "blub/core/axisAlignedBox.hpp"
#include "blub/core/sharedPointer.hpp"
#include "blub/graphic/material.hpp"

#include "blub/core/log.hpp"
namespace blub
{
namespace procedural
{
class voxelTerrain;
}
namespace graphic
{

class manualEntity;
class scene;
class camera;

class voxelTerrain : private frameListener
{
public:
    voxelTerrain(procedural::voxelTerrain *terrain, graphic::scene *sc, graphic::camera *cam);
    ~voxelTerrain();

#ifdef BLUB_DEBUG
    real timeCalcVisible();
#endif
protected:
    // totally ansync and called parallel; called from voxelMap3threads
    void partChanged(uint16 lod, int32 id);
    // totally ansync but called from only one thread at a time; called from voxelMap3threads
    void partVoxelChanged(uint16 lod, int32 id, bool removePart);

    bool updateVisibility(real);
private:  
    struct leaf
    {
        leaf(uint16 _lod, int32 _id, axisAlignedBox _aab, manualEntity* _entity = 0, material* _mat = 0)
            : lod(_lod)
            , id(_id)
            , aab(_aab)
            , entity(_entity)
            , mat(_mat)
            , loading(false)
            , shouldBeVisible(false)
        {for (uint16 ind = 0; ind < 8; ++ind) hasVisilbeChild[ind] = true;}

        uint16 lod;
        int32 id;
        axisAlignedBox aab;
        manualEntity* entity;
        material *mat;
        bool loading;
        bool shouldBeVisible;
        bool hasVisilbeChild[8];
        int16 indexSubEntities[6];
    };
    typedef sharedPointer<leaf> t_leaf;

    // lock tree before!
    static octree<t_leaf> *findNode(axisAlignedBox aab, octree<t_leaf> *tree) ;

    axisAlignedBox calcAabOneHigher(const axisAlignedBox& aab);
    bool isAabVisible(const axisAlignedBox& aab, const vector3& camPos, const uint16 lod, bool *tooFar);
    bool updateVisibilityRecursive(octree<t_leaf> *node, uint32 *debugCounter);
    bool updateCameraPos(real);

    void partChangedGraphic(uint16 lod, int32 id);
    static int16 calcQuadrant(axisAlignedBox aab, vector3 check);

    // graphicThread
    void deleteEntityGraphic(manualEntity*);
    // lock m_lockOctree before
    void deleteLeaf(t_leaf node);
    // lock m_lockOctree before
    void deleteEntity(t_leaf node);

    procedural::voxelTerrain *m_terrain;
    scene *m_scene;
    camera *m_camera;

    mutex m_lockOctree;
    octree<t_leaf> m_octree;
    vector3 m_posCamera; // for async, gets locked by m_lockOctree

    material *m_material;

    real *m_lodDistances;

#ifdef BLUB_DEBUG
    real m_timeCalcVisible;
#endif
};

}
}

#endif // GRAPHIC_VOXELTERRAIN_HPP
