#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <boost/thread/mutex.hpp>

/*#ifdef BLUB_LOG_MUTEX
#include "blub/core/log.hpp"
#else
#include "blub/core/string.hpp"
#endif*/

namespace blub
{

class mutex : public boost::mutex
{
public:
/*#ifdef BLUB_LOG_MUTEX
    mutex(string desc, bool debug = true)
        : m_desc(desc)
        , m_debug(debug)
    {
    }

    void lock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("lock(): " + m_desc);
        boost::mutex::lock();
    }

    void unlock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("unlock(): " + m_desc);
        boost::mutex::unlock();
    }
    bool tryLock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("tryLock(): " + m_desc);
        return try_lock();
    }
#else*/
    mutex()
    {
    }
    void lock()
    {
        boost::mutex::lock();
    }
    void unlock()
    {
        boost::mutex::unlock();
    }
    bool tryLock()
    {
        return try_lock();
    }
/*#endif



protected:
#ifdef BLUB_LOG_MUTEX
    string m_desc;
    bool m_debug;
#endif*/
};

}

#endif // MUTEX_HPP
