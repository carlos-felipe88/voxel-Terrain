#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "blub/graphic/scene.hpp"
#include "blub/graphic/moveable.hpp"

#include "blub/core/string.hpp"

namespace Ogre
{
class Entity;
class MovableObject;
}

namespace blub
{
namespace graphic
{

class mesh;

class entity : public moveable
{
public:
    entity(scene *sc, string src);
    entity(scene *sc, const mesh *_mesh);
    ~entity();

    void visible(bool vis);
    bool visible();
    void subEntityVisible(uint16 ind, bool vis);
    bool subEntityVisible(uint16 ind);
protected:
    Ogre::MovableObject *_movableObject();
private:
    Ogre::Entity *m_entity;
    scene *m_scene;
};

}
}

#endif // ENTITY_HPP
