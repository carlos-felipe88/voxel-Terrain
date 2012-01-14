#include "map3.hpp"
#include "blub/core/log.hpp"

#include <QDir>

using namespace blub;
using namespace blub::procedural;
using namespace blub::procedural::voxelMap;

map3::map3()
{
}

map3::map3(int32 offsetX, int32 offsetY, int32 offsetZ, uint32 sizeX, uint32 sizeY, uint32 sizeZ)
    : m_sizeX(sizeX)
    , m_sizeY(sizeY)
    , m_sizeZ(sizeZ)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_offsetZ(offsetZ)
    , m_voxel(0)
    , m_numAboveZero(0)
{
}

map3::~map3()
{
    if (m_voxel)
        delete [] m_voxel;
}

uint32 map3::posMap(int32 x, int32 y, int32 z)
{
    x+=m_offsetX;y+=m_offsetY;z+=m_offsetZ;
    uint32 res = x + y*(m_sizeX) + z*(m_sizeX)*(m_sizeY);
    return res;
}

voxel *map3::getVoxel(int32 x, int32 y, int32 z)
{
    uint32 id = posMap(x,y,z);
    assert(id < size());
    return &m_voxel[id];
}

void map3::calcNumAboveZero(uint32 ind, const voxel &data)
{
    if (data.scalar > 0. && m_voxel[ind].scalar <= 0)
        ++m_numAboveZero;
    if (data.scalar <= 0. && m_voxel[ind].scalar > 0)
        --m_numAboveZero;
}

/*
void map3::setVoxel (int32 x, int32 y, int32 z, voxel data)
{
    uint32 pos = posMap(x, y, z);
    if (pos >= this->size())
        assert(0);
    calcNumAboveZero(pos, data);
    m_voxel[pos] = data;
    addToDelta(x, y, z);
}
*/

void map3::setVoxelIfLowerZero(int32 x, int32 y, int32 z, voxel data)
{
    uint32 pos = posMap(x, y, z);
    assert(pos < this->size());
    if (m_voxel[pos].scalar < 0.)
    {
        real oldScalar = m_voxel[pos].scalar;
        calcNumAboveZero(pos, data);
        m_voxel[pos] = data;
        if (oldScalar >= 0.)
        {
            m_voxel[pos].scalar = (oldScalar + m_voxel[pos].scalar) / 2.;
        }
    }
}

/*void map3::setVoxelIfHigherEqualOne(int32 x, int32 y, int32 z, voxel data)
{
    uint32 pos = posMap(x, y, z);
    if (pos >= this->size())
        assert(0);
    if (m_voxel[pos].scalar >= 1.)
    {
        // data.scalar = ((-m_voxel[pos].scalar) + data.scalar) / 2.;
        calcNumAboveZero(pos, data);
        m_voxel[pos] = data;
        addToDelta(x, y, z);
    }
}*/

void map3::setVoxelIfHigherZero(int32 x, int32 y, int32 z, voxel data)
{
    uint32 pos = posMap(x, y, z);
    assert(pos < this->size());
    if (m_voxel[pos].scalar > 0.)
    {
        calcNumAboveZero(pos, data);
        m_voxel[pos] = data;
    }
}
/*
void map3::removeVoxel(int32 x, int32 y, int32 z)
{
    uint32 pos = posMap(x, y, z);
    voxel rem;
    rem.scalar = -1;
    calcNumAboveZero(pos, rem);
    m_voxel[pos] = rem;
    addToDelta(x, y, z);
}
*/

uint32 map3::numAboveZero()
{
    return m_numAboveZero;
}

void map3::begin()
{
    uint32 size = map3::size();
    if (m_voxel)
        BWARNING("m_voxel memleak?!");
    m_voxel = new voxel[size];
    for (uint32 ind = 0; ind < size; ++ind)
        m_voxel[ind].scalar = -1.;
    m_numAboveZero = 0;
}

void map3::end()
{  
    delete [] m_voxel;
    m_voxel = 0;
}

uint32 map3::size()
{
    return (m_sizeX)*(m_sizeY)*(m_sizeZ);
}

int32 map3::offsetX()
{
    return m_offsetX;
}

int32 map3::offsetY()
{
    return m_offsetY;
}

int32 map3::offsetZ()
{
    return m_offsetZ;
}
