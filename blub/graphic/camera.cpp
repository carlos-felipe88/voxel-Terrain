#include "camera.hpp"
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>

using namespace blub::graphic;

camera::camera(scene *sm)
    : m_cam(sm->_sceneManager()->createCamera("blub_camera"))
{
    m_cam->setAutoAspectRatio(true);
    m_cam->setNearClipDistance(0.1);
    m_cam->setPolygonMode(Ogre::PM_WIREFRAME);
    sm->system()->_root()->getAutoCreatedWindow()->addViewport(m_cam);
    m_cam->getViewport()->setOverlaysEnabled(true);
}
