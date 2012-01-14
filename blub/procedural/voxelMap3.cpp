#include "voxelMap3.hpp"

using namespace blub;
using namespace blub::procedural;


voxelMap3::voxelMap3(uint32 sizeX, uint32 sizeY, uint32 sizeZ, bool calcTransvoxel)
    : todoListener(1)
    , m_sizeX(sizeX)
    , m_sizeY(sizeY)
    , m_sizeZ(sizeZ)
    , m_scale(1.)
    , m_calcTransvoxel(calcTransvoxel)
{
}

sharedPointer<voxelMap3::myPart> voxelMap3::getPart(int32 id)
{
    mutexLocker locker(*this); (void)locker;
    t_part::iterator it = m_parts.find(id);
    if (it == m_parts.end())
        return 0;
    return *it;
}
hashMap<int32, sharedPointer<voxelMap3::myPart> > * voxelMap3::getAllParts()
{
    return &m_parts;
}

void voxelMap3::edit (sharedPointer<voxelMap::edit> ed, bool calcGeometry)
{
    addToDo(boost::bind(&voxelMap3::todoEdit, this, ed, calcGeometry));
}


void voxelMap3::todoEdit (sharedPointer<voxelMap::edit> ed, bool calculateGeometry)
{
    BOUT("void edit (sphere sp, bool cut)");

    axisAlignedBox aabb(ed->aab(m_scale));

    // calc position of first part
    int32 firstX;
    int32 firstY;
    int32 firstZ;
    // calc position of last part
    int32 lastX;
    int32 lastY;
    int32 lastZ;
    aabbIntersectsParts(aabb, &firstX, &firstY, &firstZ, &lastX, &lastY, &lastZ);

    // loop parts
    for (int32 indX = firstX; indX <= lastX; ++indX)
    {
        for (int32 indY = firstY; indY <= lastY; ++indY)
        {
            for (int32 indZ = firstZ; indZ <= lastZ; ++indZ)
            {
                sharedPointer<voxelMap3::myPart> part = getPart(indX, indY, indZ);
                if (part.isNull())
                    part = addPart(indX, indY, indZ);
                part->lock();

                part->edit(ed);

                part->unlock();

                m_sigPartAddedOrRemoved(getKey(indX, indY, indZ), false);

                // m_lockToCalculate.lock();
                if (calculateGeometry)
                    calculatePart(getKey(indX, indY, indZ));
                // m_lockToCalculate.unlock();

            }
        }
    }   

    BOUT("void edit (sphere sp, bool cut) end");
}

void voxelMap3::calculatePart(int32 part)
{
    addToDo(boost::bind(&voxelMap3::toDoCalculate, this, part));
}

int32 voxelMap3::getKey(int32 posX, int32 posY, int32 posZ)
{
    return (posX+128) + 256*(posY+128) + 256*256*(posZ+128);
    // return (posX) + 256*(posY) + 256*256*(posZ);
}

sharedPointer<voxelMap3::myPart> voxelMap3::getPart(int32 posX, int32 posY, int32 posZ)
{
    return getPart(getKey(posX, posY, posZ));
}


void voxelMap3::aabbIntersectsParts(const axisAlignedBox& aabb, int32 *firstX, int32 *firstY, int32 *firstZ, int32 *lastX, int32 *lastY, int32 *lastZ)
{
    vector3 vec = aabb.minimum()-vector3(1.);
    int32 posX = vec.x;
    int32 posY = vec.y;
    int32 posZ = vec.z;
    vec = aabb.getSize()+vector3(2.);
    int32 sizeX = vec.x;
    int32 sizeY = vec.y;
    int32 sizeZ = vec.z;

    // calc position of first part
    *firstX = posX / m_sizeX;
    if (posX < 0) --(*firstX);
    *firstY = posY / m_sizeY;
    if (posY < 0) --(*firstY);
    *firstZ = posZ / m_sizeZ;
    if (posZ < 0) --(*firstZ);

    // calc position of last part
    *lastX = (posX + sizeX) / m_sizeX;
    if (posX + sizeX < 0) --(*lastX);
    *lastY = (posY + sizeY) / m_sizeY;
    if (posY + sizeY < 0) --(*lastY);
    *lastZ = (posZ + sizeZ) / m_sizeZ;
    if (posZ + sizeZ < 0) --(*lastZ);
}

void voxelMap3::removePart(int32 key)
{
    m_sigPartAddedOrRemoved(key, true);
    mutexLocker locker(*this);(void)locker;
    hashMap<int32, sharedPointer<myPart> >::iterator it = m_parts.find(key);
    if (it == m_parts.end())
        return;
    m_parts.erase(it);
}

void voxelMap3::removePart(int32 posX, int32 posY, int32 posZ)
{
    int32 key = getKey(posX, posY, posZ);
    removePart(key);
}

sharedPointer<voxelMap3::myPart> voxelMap3::addPart(int32 posX, int32 posY, int32 posZ)
{
    int32 key = getKey(posX, posY, posZ);
    sharedPointer<voxelMap3::myPart> part = new voxelMap::part3(posX*m_sizeX, posY*m_sizeY, posZ*m_sizeZ, m_sizeX, m_sizeY, m_sizeZ, m_scale, m_calcTransvoxel);
    part->addPartChangedListener(boost::bind(&voxelMap3::partChanged, this, key));
    {
        mutexLocker locker(*this);(void)locker;
        if (m_parts.find(key) != m_parts.end())
            BERROR("m_parts.find(key) != m_parts.end()");
        m_parts[key] = part;
    }
    //  m_sigPartAddedOrRemoved(key, false);

    return part;
}

void voxelMap3::partChanged(int32 id)
{
    m_sigPartChanged(id);
}

void voxelMap3::toDoCalculate(int32 id)
{
    sharedPointer<voxelMap3::myPart> part (getPart(id));
    if (!part.isNull())
    {
        mutexLocker locker(*part); (void)locker;
        part->calculateGeometry();
        if (part->map()->numAboveZero() == 0)
            removePart(id);
    }
    else
    {
        ;//BERROR("part.isNull()");
    }
}

void voxelMap3::scale(real sc)
{
    m_scale = sc;
}
