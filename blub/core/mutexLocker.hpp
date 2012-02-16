#ifndef MUTEXLOCKER_HPP
#define MUTEXLOCKER_HPP

#include "blub/core/mutex.hpp"

namespace blub
{

class mutexLocker
{
public:
    mutexLocker(mutex& toLock)
        : m_mutex(toLock)
    {
        m_mutex.lock();
    }
    ~mutexLocker()
    {
        m_mutex.unlock();
    }
private:
    mutex &m_mutex;
};

}

#endif // MUTEXLOCKER_HPP
