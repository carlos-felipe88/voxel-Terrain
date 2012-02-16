#include "log.hpp"
#include "dateTime.hpp"

using namespace blub;

file log::m_save;
mutex log::m_mutex;
boost::chrono::system_clock::time_point log::m_lastOutput;

void log::error(const string err, const string file, const uint16 line, boost::thread::id threadId)
{
    string out = err + " FILE: " + file + " LINE: " + string::number(line) + "THREAD" + "\n";
    std::wcerr << out.toStdWString().data();
    writeLine("ERR", err, file, line, threadId);
}

void log::warning(const string warn, const string file, const uint16 line, boost::thread::id threadId)
{
    string out = warn + " FILE: " + file + " LINE: " + string::number(line) + "THREAD" + "\n";
    std::clog << out.toStdString().data();
    writeLine("WAR", warn, file, line, threadId);
}

void log::out(const string out, const string file, const uint16 line, boost::thread::id threadId)
{

    string _out = out + " FILE: " + file + " LINE: " + string::number(line) + "\n";
    std::wcout << _out.toStdWString().data() << threadId;
    writeLine("OUT", out, file, line, threadId);
}

bool log::initialise(string src)
{
    m_lastOutput = boost::chrono::system_clock::now();
    m_save.setFileName(src);
    if (!m_save.open(QIODevice::WriteOnly))
        return false;
    return m_save.write("<html><body font-size=11px><table>");
}

void log::writeLine(const string& type, const string &text, const string &file, const uint16 &line, const boost::thread::id threadId, const string module)
{
    std::stringstream ios;
    void* numericId;
    ios << threadId;
    ios >> numericId;

    string colour = "000000";
    if (type == "OUT")
        colour="00AAAA";
    if (type == "WAR")
        colour="00AA00";
    if (type == "ERR")
        colour="AA0000";
    m_mutex.lock();

    boost::chrono::duration<double> duration = boost::chrono::system_clock::now() - m_lastOutput;
    m_lastOutput = boost::chrono::system_clock::now();

    m_save.write(string("<tr><td><font color=\"" + colour + "\">" + type +
                        "</color></td><td>" + text +
                        "</td><td>" + file +
                        "</td><td>" + string::number(line) +
                        "</td><td>" + module +
                        "</td><td>" + dateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz") +
                        "</td><td>" + string::number(duration.count(), 'f') +
                        // "</td><td>" + string::number((int)numericId) +
                        "</td></tr>"));
    m_save.flush();
    m_mutex.unlock();
}
