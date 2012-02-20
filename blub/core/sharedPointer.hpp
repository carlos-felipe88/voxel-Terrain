#ifndef SHAREDPOINTER_HPP
#define SHAREDPOINTER_HPP

#include <QtCore/QSharedPointer>

namespace blub
{


template <typename T>
class sharedPointer : public QSharedPointer<T>
{
public:
    sharedPointer() : QSharedPointer<T>() {}
    sharedPointer(T* ptr) : QSharedPointer<T>(ptr) {}
    // sharedPointer(sharedPointer& ptr) : QSharedPointer(ptr) {}
    sharedPointer(const QSharedPointer<T>& ptr) : QSharedPointer<T>(ptr) {}
};

}

#endif // SHAREDPOINTER_HPP
