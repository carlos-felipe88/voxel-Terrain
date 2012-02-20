#include "graphic_node.hpp"

#include "blub/graphic/camera.hpp"
#include "blub/graphic/scene.hpp"
#include "blub/graphic/moveable.hpp"

#include <OGRE/OgreSceneNode.h>

using namespace blub;
using namespace blub::graphic;

node::node(node *nd)
    : m_node(nd->_node()->createChildSceneNode())
    , m_scene(nd->_scene())
{
}

node::node(scene *sc)
    : m_node(sc->_sceneManager()->getRootSceneNode()->createChildSceneNode())
    , m_scene(sc)
{
}

node::~node()
{
    m_scene->_sceneManager()->destroySceneNode(m_node);
}

void node::inheritOrientation(bool inhert)
{
    m_node->setInheritOrientation(inhert);
}

void node::attach(moveable *ma)
{
    m_node->attachObject(ma->_movableObject());
}

void node::attach(node *nd)
{
    m_node->addChild(nd->_node());
}

void node::attach(camera *cam)
{
    m_node->attachObject(cam->_camera());
}

vector3 node::position()
{
    return m_node->getPosition();
}

vector3 node::positionAbsolut()
{
    return m_node->_getDerivedPosition();
}

void node::position(vector3 pos)
{
    m_scene->system()->lock();
    m_node->setPosition(pos);
    m_scene->system()->unlock();
}

quaternion node::rotationAbsolut()
{
    return m_node->_getDerivedOrientation();
}

quaternion node::rotation()
{
    return m_node->getOrientation();
}

void node::rotation(quaternion rot)
{
    m_node->setOrientation(rot);
}

void node::scale(vector3 sc)
{
    m_node->setScale(sc);
}

void node::yawAxis(vector3 ax)
{
    m_node->setFixedYawAxis(true, ax);
}

void node::yaw(real abs)
{
    m_node->yaw(Ogre::Radian(abs));
}
void node::roll(real abs)
{
    m_node->roll(Ogre::Radian(abs));
}
