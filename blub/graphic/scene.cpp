#include "scene.hpp"
#include "blub/core/vector3.hpp"
#include <OGRE/OgreLight.h>

using namespace blub::graphic;

scene::scene(blub::graphic::system *sys)
    : m_system(sys)
    , m_mgr(sys->_root()->createSceneManager("OctreeSceneManager"))
{
    // m_mgr->setShadowTextureSettings(1024, 2);
    // m_mgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    m_mgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

    // m_mgr->setShadowFarDistance(20);
    Ogre::Light *light = m_mgr->createLight();
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    // light->setPosition(-1000, 1000, -1000);
    light->setDirection(vector3(0.25, -0.5, -1));
    light->setDiffuseColour(1, 1, 1);
    //light = m_mgr->createLight();
    //light->setType(Ogre::Light::LT_DIRECTIONAL);
    // light->setPosition(-1000, 1000, -1000);
    //light->setDirection(vector3(0, -0.25, 0));
    // light->setCastShadows(true);
}
