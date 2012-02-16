#ifndef BLUBGLOBALS_HPP
#define BLUBGLOBALS_HPP

#include <QtCore/QtGlobal>

namespace blub
{
    typedef qint8 int8 ;
    typedef qint16 int16;
    typedef qint32 int32;
    typedef qint64 int64;
    typedef qlonglong longlong;
    typedef qptrdiff ptrdiff;
    typedef float real;
    typedef quint8 uint8;
    typedef quint16 uint16;
    typedef quint32 uint32;
    typedef quint64 uint64;
    typedef quintptr uintptr;
    typedef qulonglong ulonglong;

#define BASSERT(x) assert(x)
}

#endif // BLUBGLOBALS_HPP
