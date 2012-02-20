#include "entity.hpp"

#include "blub/graphic/mesh.hpp"

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>

using namespace blub::graphic;
using namespace blub;

entity::entity(scene *sc, string src)
    : m_entity(sc->_sceneManager()->createEntity(src))
    , m_scene(sc)
{
}

entity::entity(scene *sc, const mesh *_mesh)
    : m_entity(sc->_sceneManager()->createEntity(_mesh->_mesh()->getName()))
    , m_scene(sc)
{
}

entity::~entity()
{
    m_scene->_sceneManager()->destroyEntity(m_entity);
}

Ogre::MovableObject *entity::_movableObject()
{
    return m_entity;
}

void entity::visible(bool vis)
{
    m_entity->setVisible(vis);
}

bool entity::visible()
{
    return m_entity->isVisible();
}

void entity::subEntityVisible(blub::uint16 ind, bool vis)
{
    m_entity->getSubEntity(ind)->setVisible(vis);
}

bool entity::subEntityVisible(blub::uint16 ind)
{
    return m_entity->getSubEntity(ind)->isVisible();
}


