#include "manualEntity.hpp"
#include "blub/graphic/entity.hpp"

using namespace blub::graphic;
using namespace blub;

/*manualEntity::manualEntity()
    : manualObject()
    , m_entity(0)
{
}*/

manualEntity::manualEntity(graphic::scene *sc, uint16 numLod)
    : manualObject(sc, numLod)
    , m_entity(0)
    , m_node(sc)
{
}

manualEntity::manualEntity(graphic::scene *sc, uint16 numLod, node *attachTo)
    : manualObject(sc, numLod)
    , m_entity(0)
    , m_node(attachTo)
{
}

manualEntity::~manualEntity()
{
    if (m_entity)
        delete m_entity;
}

void manualEntity::reInitGraphic(material* mat)
{
    manualObject::reInitGraphic(mat);

    if (m_entity)
    {
        delete m_entity;
        m_entity = 0;
    }
    if (!m_entity && lods()->numTriangles > 0)
    {
        m_entity = new blub::graphic::entity(scene(), string("mO"+string::number(id())));
        m_node.attach(m_entity);
    }

    // m_node->setPosition(m_aabb.getMinimum());
}

blub::graphic::node* manualEntity::getNode()
{
    return &m_node;
}

blub::graphic::entity* manualEntity::entity()
{
    return m_entity;
}
