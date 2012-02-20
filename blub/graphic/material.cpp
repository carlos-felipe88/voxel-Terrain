#include "material.hpp"
#include "blub/core/log.hpp"

#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgrePass.h>

using namespace blub;
using namespace blub::graphic;

uint32 material::g_idCounter = 0;

material::material()
{
}

material::material(string loadFromFile)
{
    m_material = Ogre::MaterialManager::getSingletonPtr()->getByName(loadFromFile);
}

material::material(const material& mat)
{
    m_material = mat._material()->clone(blub::string("material__material" + blub::string::number(g_idCounter++)));
}

Ogre::MaterialPtr material::_material() const
{
    return m_material;
}

void material::ambient(const colour &col)
{
    m_material->getTechnique(0)->getPass(0)->setAmbient(col);
}
