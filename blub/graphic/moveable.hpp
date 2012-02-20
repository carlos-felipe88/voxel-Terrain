#ifndef GRAPHIC_MOVEABLE_HPP
#define GRAPHIC_MOVEABLE_HPP

#include <OGRE/OgreMovableObject.h>

namespace blub
{
namespace graphic
{
    class node;
    class moveable
    {
        friend class node;
    protected:
        virtual Ogre::MovableObject *_movableObject() = 0;
    };

}
}

#endif // GRAPHIC_MOVEABLE_HPP
