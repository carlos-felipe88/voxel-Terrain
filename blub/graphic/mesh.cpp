#include "mesh.hpp"

using namespace blub::graphic;

/*mesh::mesh()
{
    BERROR("mesh() NOT IMPLEMENTED!!!");
}*/

/*mesh(string name)
{

}*/

mesh::mesh(Ogre::MeshPtr mesh)
{
    m_mesh = mesh;
}

Ogre::MeshPtr mesh::_mesh() const
{
    return m_mesh;
}



