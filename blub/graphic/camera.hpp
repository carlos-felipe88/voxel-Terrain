#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "blub/core/vector3.hpp"
#include "blub/core/quaternion.hpp"
#include "blub/graphic/scene.hpp"

#include <OGRE/OgreCamera.h>

namespace blub
{
namespace graphic
{

class camera
{
public:
    camera(scene *sm);

    void yaw(float rel)
    {m_cam->yaw(Ogre::Radian(rel));}
    void yawAxis(vector3 ya)
    {m_cam->setFixedYawAxis(true, ya);}
    void roll(float rel)
    {m_cam->roll(Ogre::Radian(rel));}
    void pitch(float rel)
    {m_cam->pitch(Ogre::Radian(rel));}
    void rotation(quaternion quad)
    {m_cam->setOrientation(quad);}

    void move(vector3 vec)
    {m_cam->move(vec);}
    void moveRelative(vector3 vec)
    {m_cam->moveRelative(vec);}

    void position(vector3 pos)
    {setPosition(pos);}
    void setPosition(vector3 pos)
    {return m_cam->setPosition(pos);}

    void setWireFrame(bool set)
    {if (!set) m_cam->setPolygonMode(Ogre::PM_SOLID); else m_cam->setPolygonMode(Ogre::PM_WIREFRAME);}

    vector3 position()
    {return m_cam->getPosition();}
    vector3 positionAbsolut()
    {return m_cam->getDerivedPosition();}
    vector3 direction()
    {return m_cam->getDirection();}
    vector3 directionAbsolut()
    {return m_cam->getDerivedDirection();}

    Ogre::Viewport *_viewport() {return m_cam->getViewport();}
protected:
    friend class node;
    Ogre::Camera *_camera() {return m_cam;}
private:
    Ogre::Camera *m_cam;
};

}
}

#endif // CAMERA_HPP
