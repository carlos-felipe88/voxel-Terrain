#ifndef MANUALENTITY_HPP
#define MANUALENTITY_HPP

#include "blub/graphic/manualObject.hpp"
#include "blub/graphic/node.hpp"

namespace blub
{
namespace graphic
{

class entity;
class material;

class manualEntity : public manualObject
{
public:
    // manualEntity();
    manualEntity(graphic::scene *sc, uint16 numLod, node *attachTo);
    manualEntity(graphic::scene *sc, uint16 numLod = 1);
    ~manualEntity();

    void reInitGraphic(material* mat);
    blub::graphic::node* getNode();
    blub::graphic::entity* entity();

private:
    blub::graphic::entity *m_entity;
    blub::graphic::node m_node;
};

}
}

#endif // MANUALENTITY_HPP
