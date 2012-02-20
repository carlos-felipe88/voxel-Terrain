#ifndef ROPE_HPP
#define ROPE_HPP

#include "blub/physic/root.hpp"
#include "blub/physic/rigidEntity.hpp"
#include "blub/core/vector3.hpp"
#include "blub/core/vector.hpp"

#include <bullet/BulletSoftBody/btSoftBody.h>

namespace blub
{
namespace physic
{

class rigidEntity;

class rope
{
public:
    rope(root *sys);

    void appendAnchor(rigidEntity *ent);
    vector<vector3> nodes();
protected:
    // btSoftBody* _rope() {return m_rope;}
private:
    root *m_sys;

    struct node
    {
        node()
            : constraint(0)
            , body(0)
        {;}

        btPoint2PointConstraint *constraint;
        rigidEntity *body;
    };

    node m_nodes[12];

//    btSoftBody* m_rope;
};

}
}

#endif // ROPE_HPP
