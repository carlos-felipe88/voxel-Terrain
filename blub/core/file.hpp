#ifndef FILE_HPP
#define FILE_HPP

#include "string.hpp"
#include <QtCore/QFile>

namespace blub
{
class file : public QFile
{
public:
    file() : QFile() {;}
    file(string src) : QFile(src) {;}
};
}

#endif // FILE_HPP
