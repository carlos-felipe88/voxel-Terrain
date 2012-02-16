#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <QHash>
#include "blub/core/vector.hpp"

namespace blub
{

template <class S, class T>
class hashMap : public QHash<S, T>
{
public:
    hashMap<S, T>() : QHash<S, T>() {;}

    void insertMultiList(S key, const vector<T> &toInsert)
    {
        for (typename vector<T>::const_iterator it = toInsert.constBegin(); it != toInsert.constEnd(); ++it)
        {
            insertMulti(key, *it);
        }
    }
};

}

#endif // HASHMAP_HPP
