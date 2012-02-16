#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <QVector>
#include "blub/core/globals.hpp"

namespace blub
{

template <class T>
class vector : public QList<T>
{
public:
    vector<T>() : QList<T>() {;}
    vector<T>(QList<T> lst) : QList<T>(lst) {;}
    vector<T>(T item0, T item1) : QList<T>() {append(item0); append(item1);}

    /*void removeAt(uint32 ind)
    {
        QVector<T>::remove(ind);
    }

    T takeAt(uint32 ind)
    {
        T result = *(this->begin()+ind);
        removeAt(ind);
        return result;
    }

    T takeFirst()
    {
        T result = *this->begin();
        removeAt(0);
        return result;
    }*/

    bool push_back_distinct(const T & ins)
    {
        if (indexOf(ins) == -1)
        {
            push_back(ins);
            return true;
        }
        else
            return false;
    }
};

}

#endif // VECTOR_HPP
