#include "blub/core/mutex.hpp"

#ifndef BLUBLOG_HPP
#define BLUBLOG_HPP

#include "blub/core/file.hpp"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

namespace blub
{
    class mutex;

    class log
    {
    public:
        static bool initialise(string src);

        static void error(const string err, const string file, const uint16 line, boost::thread::id threadId);
        static void out(const string out, const string file, const uint16 line, boost::thread::id threadId);
        static void warning(const string warn, const string file, const uint16 line, boost::thread::id threadId);

    private:
        static void writeLine(const string& type, const string &text, const string &file, const uint16 &line, const boost::thread::id threadId, const string module = "blub");
        static file m_save;
        static mutex m_mutex;

        static boost::chrono::system_clock::time_point m_lastOutput;
    };

#define BERROR(x) log::error(x, __FILE__, __LINE__, boost::this_thread::get_id())
#define BWARNING(x) log::warning(x, __FILE__, __LINE__, boost::this_thread::get_id())
#define BOUT(x) log::out(x, __FILE__, __LINE__, boost::this_thread::get_id())
}

#endif // BLUBLOG_HPP
