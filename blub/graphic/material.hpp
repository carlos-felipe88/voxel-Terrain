#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "blub/core/string.hpp"
#include "blub/core/colour.hpp"

#include <OGRE/OgreMaterial.h>

namespace blub
{
namespace graphic
{

class material
{
public:
    material();
    material(string loadFromFile);
    material(const material& mat);

    void ambient(const colour &col);
protected:
    friend class manualObject;
    Ogre::MaterialPtr _material() const;
private:
    Ogre::MaterialPtr m_material;

    static uint32 g_idCounter; // for coping materials
};

}
}

#endif // MATERIAL_HPP
