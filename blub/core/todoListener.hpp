#ifndef TODOLISTENER_HPP
#define TODOLISTENER_HPP

#include "blub/core/log.hpp"
#include "blub/core/vector.hpp"

#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace blub
{

class todoListener
{
public:
    todoListener(uint16 numThreads = 1)
        : m_work(m_toDo)
        , m_numThreads(numThreads)
    {
        if (m_numThreads > 0)
        {
            for (uint16 ind = 0; ind < m_numThreads; ++ind)
                m_threads.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_toDo)));
        }
    }
    ~todoListener()
    {
        m_toDo.stop();
        for (t_threads::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
        {
            (*it)->join();
            delete (*it);
        }
    }

    template<typename CompletionHandler>
    void addToDo(CompletionHandler handler)
    {
        m_toDo.post(handler);
    }
private:
    boost::asio::io_service m_toDo;
    boost::asio::io_service::work m_work;
    uint16 m_numThreads;
    typedef vector<boost::thread*> t_threads;
    t_threads m_threads;
    // boost::thread m_thread;
};

}
#endif // TODOLISTENER_HPP
