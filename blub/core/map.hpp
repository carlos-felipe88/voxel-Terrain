#ifndef MAP_HPP
#define MAP_HPP

#include <QtCore/QMap>

namespace blub
{

template <class S, class T>
class map : public QMap<S, T>
{
public:
    map<S, T>() : QMap<S, T>() {;}
};

}

#endif // MAP_HPP
