#include "sky_system.hpp"
#include <OGRE/OgreRoot.h>

using namespace blub::graphic::sky;
using namespace blub;

system::system(blub::graphic::scene *sc)
{
    Ogre::SceneManager* mgr = sc->_sceneManager();
    mgr->setSkyBox(true, "Examples/SpaceSkyBox");
}

bool system::update(real timeSinceLastFrame)
{
    (void)timeSinceLastFrame;
    return true;
}
