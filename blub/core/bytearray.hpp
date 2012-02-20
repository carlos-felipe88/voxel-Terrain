#ifndef BYTEARRAY_HPP
#define BYTEARRAY_HPP

#include <QtCore/QByteArray>

namespace blub
{

class bytearray : public QByteArray
{
public:
    bytearray(const QByteArray &array) : QByteArray(array) {;}
};

}

#endif // BYTEARRAY_HPP
