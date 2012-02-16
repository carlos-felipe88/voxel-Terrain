#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "blub/core/globals.hpp"

namespace blub
{
template <typename dataType>
class octree
{
public:
    octree()
    {
        for (uint16 ind = 0; ind < 8; ++ind)
            m_children[ind] = 0;
    }

    octree<dataType> **child(const uint16 ind)
    {return &m_children[ind];}
    void child(const uint16 ind, octree<dataType>* tree)
    {m_children[ind] = tree;}

    dataType data()
    {return m_data;}
    void data(dataType dT)
    {m_data = dT;}

private:
    octree<dataType> *m_children[8];
    dataType m_data;
};
}

#endif // OCTREE_HPP
