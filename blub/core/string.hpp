#ifndef BLUBSTRING_HPP
#define BLUBSTRING_HPP

#include "./globals.hpp"

#include <QString>
#include <OGRE/OgreUTFString.h>
#include "blub/core/quaternion.hpp"
#include "blub/core/vector3.hpp"
#include "blub/core/vector2.hpp"


namespace blub
{

    class string : public QString
    {
    public:
        // qt string
        string()
            : QString()
        {;}
        inline string(const char* str)
            : QString(str)
        {}
        inline string(const QString& str)
            : QString(str)
        {}
        // Ogre3D strings
        inline string(const Ogre::String& str)
            : QString(str.c_str())
        {}
        inline operator QString()
        {return QString(this->data());}
        inline operator Ogre::String()
        {return toStdString();}
        inline operator Ogre::UTFString()
        {return Ogre::UTFString(toStdWString());}
        inline operator QByteArray()
        {return toUtf8();}

        static string number(int n, int base = 10 )
        {return QString::number(n, base);}

        static string number(uint32 n, int base = 10 )
        {return QString::number(n, base);}


        /*static string number(size_t n, int base = 10 )
        {return QString::number(n, base);}*/

        static string number(long n, int base = 10 )
        {return QString::number(n, base);}

        static string number (double n, char format = 'f', int precision = 6)
        {return QString::number(n, format, precision);}

        static string number (vector3 vec, char format = 'f', int precision = 6)
        {return number(vec.x, format, precision) + " " + number(vec.y, format, precision) + " " + number(vec.z, format, precision);}

        static string number (vector2 vec, char format = 'f', int precision = 6)
        {return number(vec.x, format, precision) + " " + number(vec.y, format, precision);}

        static string number (quaternion quat, char format = 'f', int precision = 6)
        {return number(quat.x, format, precision) + " " + number(quat.y, format, precision) + " " + number(quat.z, format, precision) + " " + number(quat.w, format, precision);}
    };

}

#endif // BLUBSTRING_HPP
