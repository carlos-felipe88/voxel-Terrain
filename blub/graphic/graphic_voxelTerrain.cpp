#include "graphic_voxelTerrain.hpp"

#include "blub/graphic/manualEntity.hpp"
#include "blub/graphic/scene.hpp"
#include "blub/graphic/camera.hpp"
#include "blub/graphic/entity.hpp"
#include "blub/procedural/voxelTerrain.hpp"
#include "blub/core/mutexLocker.hpp"

#ifdef BLUB_DEBUG
#include <boost/chrono.hpp>
#endif

using namespace blub::graphic;
using namespace blub;

voxelTerrain::voxelTerrain(procedural::voxelTerrain *terrain, blub::graphic::scene *sc, graphic::camera *cam)
    : m_terrain(terrain)
    , m_scene(sc)
    , m_camera(cam)
    , m_material(0)
#ifdef BLUB_DEBUG
    , m_timeCalcVisible(0.)
#endif
{
    m_lodDistances = new real[3];
    m_lodDistances[0] = 80.;
    m_lodDistances[1] = 200.;
    m_lodDistances[2] = 500.;

    uint16 numLod = m_terrain->numLod();
    m_material = new material[numLod];
    for (uint16 ind = 0; ind < numLod; ++ind)
    {
        m_material[ind] = material("triPlanarMaterial0");
    }


    m_terrain->addPartChangedListener(boost::bind(&voxelTerrain::partChanged, this, _1, _2));
    m_terrain->addPartVoxelChangedListener(boost::bind(&voxelTerrain::partVoxelChanged, this, _1, _2, _3));

    m_scene->system()->addFrameListener(boost::bind(&voxelTerrain::updateCameraPos, this, _1));

    t_leaf data(new leaf(-1, -1, axisAlignedBox(vector3(-1000000.), vector3(1000000.))));
    m_octree.data(data);

    addFrameListener(boost::bind(&voxelTerrain::updateVisibility, this, _1));
    run(20, true);
}

voxelTerrain::~voxelTerrain()
{
    BOUT("~voxelTerrain()");
    delete [] m_material;
}

void voxelTerrain::partChanged(uint16 lod, int32 id)
{
    // graphic thread shall do
    m_scene->system()->addToDo(boost::bind(&voxelTerrain::partChangedGraphic, this, lod, id));
}


void voxelTerrain::deleteEntityGraphic(manualEntity *ent)
{
    if (ent)
        delete ent;
}


void voxelTerrain::deleteLeaf(t_leaf lf)
{
    deleteEntity(lf);
    lf->lod = -1;
    lf->id = -1;
    lf->loading = false;
}

void voxelTerrain::deleteEntity(t_leaf lf)
{
    if (lf->entity)
    {
        m_scene->system()->addToDo(boost::bind(&voxelTerrain::deleteEntityGraphic, this, lf->entity));
    }
    lf->entity = 0;
    lf->mat = 0;
    lf->loading = false;
}

void voxelTerrain::partVoxelChanged(uint16 lod, int32 id, bool removePart)
{
    // voxelMap3Thread shall insert/remove part into octree
    // depending on distance it shall calculate hull
    mutexLocker locker(m_lockOctree); (void)locker;

    typedef procedural::voxelTerrain::t_part3Ptr t_part3Ptr;
    t_part3Ptr part = m_terrain->part(lod, id);

    if (part.isNull())
    {
        // remove
        // fatal should not happen even if part gets removed
        return;
    }
    // (void)locker2; already locked by caller
    // mutexLocker locker2(*part.data());
    if (removePart)
    {
        octree<t_leaf> *child = findNode(part->aabb(), &m_octree);
        if (!child)
        {
            // already removed?
            BWARNING("!child");
            return;
        }
        deleteLeaf(child->data());
    }
    else
    {
        // add
        octree<t_leaf> *current = &m_octree;
        axisAlignedBox aabNew = part->aabb();
        axisAlignedBox aabCurrent = current->data()->aab;
        t_leaf lf(new leaf(lod, id, aabNew, 0));
        for (;;)
        {
            int16 quad = calcQuadrant(aabCurrent, aabNew.center());
            // is in quad?
            if (quad != -1)
            {
                octree<t_leaf> **child = current->child(quad);
                // is there a chid in that quad
                if (*child)
                {
                    axisAlignedBox aabChild = (**child).data()->aab;
                    if (aabChild == aabNew)
                    {
                        // lod level
                        if ((**child).data()->entity)
                        {
                            /*m_scene->system()->addToDo(boost::bind(&voxelTerrain::deleteLeafGraphic, this, (**child).data()));*/
                            lf->entity = (**child).data()->entity;
                            lf->loading = true;
                            m_terrain->voxelMap(lod)->calculatePart(lf->id);
                        }
                        (**child).data(lf);
                        break;
                    }
                    else
                    {
                        // has child larger size?
                        if (aabChild.getSize().x > aabNew.getSize().x && calcQuadrant(aabChild, aabNew.center()) != -1)
                        {
                            current->data()->hasVisilbeChild[quad] = true;
                            aabCurrent = aabChild;
                            current = *child;
                            continue;
                        }
                        else
                        {
                            // create octree between current and child
                            octree<t_leaf> *buffer = *child;
                            *child = new octree<t_leaf>();
                            uint32 sizeX = aabChild.getSize().x;
                            uint32 sizeY = aabChild.getSize().y;
                            uint32 sizeZ = aabChild.getSize().z;
                            vector3 minNew(aabChild.minimum() - vector3(((int32)aabChild.minimum().x)%(sizeX*2), ((int32)aabChild.minimum().y)%(sizeY*2), ((int32)aabChild.minimum().z)%(sizeZ*2)));
                            axisAlignedBox aabNew2(minNew, minNew + aabChild.getSize()*2.);
                            t_leaf lf2(new leaf(-1, -1, aabNew2, 0));
                            (*child)->data(lf2);
                            int16 quad2 = calcQuadrant(aabNew2, aabChild.center());
                            if (quad2 == -1)
                            {
                                BERROR("fatal!");
                                return;
                            }
                            (*child)->child(quad2, buffer);
                            continue;
                        }
                    }
                }
                else
                {
                    // insert here
                    *child = new octree<t_leaf>();
                    (**child).data(lf);
                    break;
                }
            }
            else
            {
                BERROR("fatal!");
                return;
            }
        }
    }
}

int16 voxelTerrain::calcQuadrant(axisAlignedBox aab, vector3 check)
{
    if (!aab.intersects(check))
    {
        return -1;
    }
    vector3 center(aab.getCenter());
    if (center.x < check.x && center.y < check.y && center.z < check.z)
        return 0;
    if (center.x < check.x && center.y < check.y && center.z > check.z)
        return 1;
    if (center.x < check.x && center.y > check.y && center.z < check.z)
        return 2;
    if (center.x < check.x && center.y > check.y && center.z > check.z)
        return 3;
    if (center.x > check.x && center.y < check.y && center.z < check.z)
        return 4;
    if (center.x > check.x && center.y < check.y && center.z > check.z)
        return 5;
    if (center.x > check.x && center.y > check.y && center.z < check.z)
        return 6;
    if (center.x > check.x && center.y > check.y && center.z > check.z)
        return 7;
    BOUT("fatal!");
    return -1;
}

octree<voxelTerrain::t_leaf> *voxelTerrain::findNode(axisAlignedBox aab, octree<t_leaf> *tree)
{
    if (!tree)
        return 0;
    if (tree->data()->aab == aab)
        return tree;
    if (tree->data()->aab.size().x < aab.size().x)
        return 0;
    int16 id = calcQuadrant(tree->data()->aab, aab.center());
    if (id < 0)
        return 0;
    return findNode(aab, *tree->child(id));
}

void voxelTerrain::partChangedGraphic(uint16 lod, int32 id)
{
    /*if (lod != 0)
        return;*/

    // get part
    sharedPointer<procedural::voxelMap::part3> part = m_terrain->part(lod, id);
    if (part.isNull())
    {
        // should not happen, but may occur because of async
        // untested because error quite difficult to construct
        // BERROR("part.isNull()");
        return; // TODO : fix bool loading!!!
    }

    mutexLocker locker2(*part); (void)locker2; // dont switch locks... deadlock
    mutexLocker locker(m_lockOctree); (void)locker;

    octree<t_leaf> *node = findNode(part->aabb(), &m_octree);
    if (!node)
    {
        // should not happen, but may occur because of async
        BERROR("!node");
        return;
    }
    t_leaf data = node->data();
    data->loading = false;
    manualEntity *gr(data->entity);
    material *mat(data->mat);
    uint16 numSubmeshes(1);
    if (lod > 0)
        numSubmeshes = 1+6;
    if (gr)
    {
        // deleteLeafGraphic(data); // not possible because of lock in it
        delete gr;
        gr = 0;
    }
    gr = new manualEntity(m_scene, numSubmeshes);
    mat = &m_material[lod];

    vector3* positions = part->hullVertices();
    vector3* normals = part->hullNormals();
    gr->numVertices(part->numVertices());
    gr->position(positions, part->numVertices());
    gr->normal(normals, part->numVertices());

    // indices
    for (uint16 ind = 0; ind < numSubmeshes-1; ++ind)
        data->indexSubEntities[ind] = -1;
    uint16 counter(0);
    for (uint16 ind = 0; ind < numSubmeshes; ++ind)
    {
        uint32 num = part->numTriangles(ind);
        if (num == 0)
        {
            if (ind > 0)
                data->indexSubEntities[ind-1] = -1;
            continue;
        }
        triangle* triangles = part->hullTriangles(ind);
        gr->numTriangles(num, ind);
        gr->triangle(triangles, num, ind);
        if (ind > 0)
        {
            data->indexSubEntities[ind-1] = counter;
        }
        ++counter;
    }

    axisAlignedBox aabb = part->aabb();
    // save memory
    part->clearGeometry();
    // aabb
    gr->aabb(axisAlignedBox(vector3(0), aabb.getSize()));
    // pos
    gr->getNode()->position(aabb.getMinimum());

    // (re)init Graphics
    gr->reInitGraphic(mat);
    // remove Buffer Geometry. Geometry is now in HardwareVertexBuffer, so delete buffer;
    gr->deleteAllGeometry();

    // set all transvoxel invisible
    if (gr->entity() && lod > 0)
    {
        for (uint16 ind = 0; ind < 6; ++ind)
        {
            int16 id2 = data->indexSubEntities[ind];
            if (id2 > -1)
            {
                gr->entity()->subEntityVisible(id2, false);
            }
        }
    }

    // save gr
    data->entity = gr;
    data->mat = mat;
}

axisAlignedBox voxelTerrain::calcAabOneHigher(const axisAlignedBox& aab)
{
    vector3 pos = aab.minimum();
    vector3 size = aab.getSize();
    // pos in higher lod
    size *= 2.;
    vector3 delta(std::abs(((int)pos.x)%(int)size.x),
                  std::abs(((int)pos.y)%(int)size.y),
                  std::abs(((int)pos.z)%(int)size.z)
                  );
    vector3 posOneLodHigher = pos - delta;
    return axisAlignedBox(posOneLodHigher, posOneLodHigher + size);
}

bool voxelTerrain::isAabVisible(const axisAlignedBox& aab, const vector3& camPos, const uint16 lod, bool *tooFarOut)
{
    axisAlignedBox aabbOneLodHigher = calcAabOneHigher(aab);

    /*real radiusAAb = aab.getCenter().squaredDistance(aab.getMaximum());
    real radius = aabbOneLodHigher.getCenter().squaredDistance(aabbOneLodHigher.getMaximum());
*/
    bool tooNear(false);
    bool tooFar(false);

    real near2 (aab.getCenter().squaredDistance(camPos));
    if (lod > 0)
    {
        tooNear = near2 <= m_lodDistances[lod-1]*m_lodDistances[lod-1];
    }
    real far2(aabbOneLodHigher.getCenter().squaredDistance(camPos));
    tooFar = far2 >= m_lodDistances[lod]*m_lodDistances[lod];
    if (tooFarOut)
        *tooFarOut = tooFar;

    return  !tooNear && !tooFar;
}

bool voxelTerrain::updateVisibilityRecursive(octree<t_leaf> *node, uint32 *debugCounter)
{
    if (!node)
        return false;
    ++(*debugCounter);
    const uint16 numLod = m_terrain->numLod();
    const vector3 camPos = m_posCamera;

    t_leaf lf = node->data();
    uint16 lod = lf->lod;

    axisAlignedBox aabb = lf->aab;

    bool shouldBeVisible(false);
    /*bool tooNear(false);*/
    bool tooFar(false);
    if (lod < numLod)
    {
        // delete children
        if(lf->shouldBeVisible && lf->entity)
        {
            for (uint16 ind = 0; ind < 8; ++ind)
            {
                octree<t_leaf> *child = *node->child(ind);
                if (lf->hasVisilbeChild[ind] && child)
                {
                    if (!child->data().isNull())
                    {
                        if (child->data()->entity)
                        {
                            deleteEntity(child->data());
                        }
                    }
                }
            }
        }
        // BOUT("\n" + string::number(lf->id));
        shouldBeVisible = isAabVisible(aabb, camPos, lod, &tooFar);
/*
        shouldBeVisible = (lod == 1 && lf->id == 8421504) || (lod == 0 && !axisAlignedBox(vector3(0), vector3(32)).contains(aabb.center()));
        shouldBeVisible = (lod == 2);
        shouldBeVisible = (lod == 1 && axisAlignedBox(vector3(32), vector3(32+32)).contains(aabb.center())) || (lod == 0 && !axisAlignedBox(vector3(32), vector3(32+32)).contains(aabb.center()));
        shouldBeVisible = (lod == 1 && axisAlignedBox(vector3(0, 0, 32), vector3(32, 32, 64)).contains(aabb.center())) || (lod == 0 && !axisAlignedBox(vector3(0, 0, 32), vector3(32, 32, 64)).contains(aabb.center()));
*/        // shouldBeVisible = (lod == 2 && axisAlignedBox(vector3(0), vector3(64)).contains(aabb.center())) || (lod == 1 && !axisAlignedBox(vector3(0), vector3(64)).contains(aabb.center()));

        if (lf->id > -1)
        {
            if (lod > 0 && shouldBeVisible && lf->entity)
            {
                if (lf->entity->entity())
                {
                    const int16 indices[] = {0, 0, -1,
                                             0, 0, 1,
                                             0, -1, 0,
                                             0, 1, 0,
                                             -1, 0, 0,
                                             1, 0, 0};
                    // axisAlignedBox aabHigher = calcAabOneHigher(aabb);
                    for (uint16 ind = 0; ind < 6; ++ind)
                    {
                        axisAlignedBox neigh = aabb.move(aabb.size()*vector3(indices[ind*3+0], indices[ind*3+1], indices[ind*3+2]));
                        if (neigh.center().squaredDistance(camPos) > aabb.center().squaredDistance(camPos))
                            continue;
                        bool buffer = isAabVisible(neigh, camPos, lod, 0);
                        int16 id = lf->indexSubEntities[ind];
                        if (id > -1)
                        {
                            lf->entity->entity()->subEntityVisible(id, !buffer);
                        }
                    }
                }
            }

            lf->shouldBeVisible = shouldBeVisible;
            if (!lf->entity && !lf->loading && shouldBeVisible)
            {
                m_terrain->voxelMap(lod)->calculatePart(lf->id);
                lf->loading = true;
            }
            if (lf->entity && !shouldBeVisible && lod == numLod-1 && tooFar)
            {
                deleteEntity(lf);
            }
        }
        else
            lf->shouldBeVisible = false;
    }

    bool childVis(false);
    for (uint16 ind = 0; ind < 8; ++ind)
    {
        if (!lf->hasVisilbeChild[ind] && tooFar)
        {
            continue;
        }
        bool childVis2 = updateVisibilityRecursive(*node->child(ind), debugCounter);
        lf->hasVisilbeChild[ind] = childVis2;
        childVis = childVis || childVis2;
    }


    // delete yourself
    if(!lf->shouldBeVisible && lf->entity && lf->lod > 0 && !tooFar)
    {
        bool del(true);
        for (uint16 ind = 0; ind < 8; ++ind)
        {
            octree<t_leaf> *child = *node->child(ind);
            if (child)
            {
                if (!child->data().isNull())
                {
                    if (!child->data()->entity && child->data()->shouldBeVisible && child->data()->id > -1)
                    {
                        del = false;
                        break;
                    }
                }
            }
        }
        if (del)
        {
            deleteEntity(lf);
        }
    }



    return lf->entity != 0 || lf->loading || childVis;
}

bool voxelTerrain::updateCameraPos(real)
{
    mutexLocker locker(m_lockOctree); (void)locker;
    m_posCamera = m_camera->positionAbsolut();
    return true;
}

bool voxelTerrain::updateVisibility(real)
{   
/*#ifdef BLUB_DEBUG
    boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
#endif*/
    mutexLocker locker(m_lockOctree); (void)locker;
    uint32 counter(0);
    updateVisibilityRecursive(&m_octree, &counter);
/*#ifdef BLUB_DEBUG
    boost::chrono::duration<real> sec = boost::chrono::system_clock::now() - start;
    m_timeCalcVisible = sec.count();
#endif*/
    return true;
}

#ifdef BLUB_DEBUG
real voxelTerrain::timeCalcVisible()
{
    return m_timeCalcVisible;
}
#endif
