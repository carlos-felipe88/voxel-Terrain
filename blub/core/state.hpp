#ifndef STATE_HPP
#define STATE_HPP

#include <boost/signals2.hpp>

namespace blub
{
    class state
    {
    public:
        state() {;}
        ~state() {clean();}

        typedef boost::signals2::signal<void (state* st)> t_signal;
        void addDoneListener(const t_signal::slot_type &subscriber)
        {m_sigDone.connect(subscriber);}

        virtual bool initialise() {return true;}
        virtual void clean() {;}

    protected:
        void callDone() {m_sigDone(this);}
    private:
        t_signal m_sigDone;
    };
}

#endif // STATE_HPP
