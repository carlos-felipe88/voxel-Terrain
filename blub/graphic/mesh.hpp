#ifndef BLUBMESH_HPP
#define BLUBMESH_HPP

#include "graphic.hpp"

#include <OGRE/OgreMesh.h>

namespace blub
{
namespace graphic
{
    class manualObject;
    class entity;

    class mesh
    {

    public:
        Ogre::MeshPtr m_mesh;

    protected:
        friend class blub::graphic::manualObject;
        friend class blub::graphic::entity;

        mesh(string name);
        mesh(Ogre::MeshPtr mesh);
        Ogre::MeshPtr _mesh() const;
    };

}
}

#endif // BLUBMESH_HPP
