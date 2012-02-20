#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <QtCore/QDateTime>

namespace blub
{

class dateTime : public QDateTime
{
public:
    dateTime() {;}
    dateTime(QDateTime &other) : QDateTime(other) {;}
    dateTime(QDateTime other) : QDateTime(other) {;}

    // static dateTime currentDateTime() {return QDateTime::currentDateTime();}
};

}

#endif // DATETIME_HPP
