#ifndef GRAPHIC_NODE_HPP
#define GRAPHIC_NODE_HPP

#include "blub/core/vector3.hpp"
#include "blub/core/quaternion.hpp"

namespace Ogre
{
    class SceneNode;
}

namespace blub
{
namespace graphic
{
    class camera;
    class scene;
    class moveable;

    class node
    {
    public:

        node(node *nd);
        node(scene *sc);
        ~node();

        void attach(moveable *ma);
        void attach(node *nd);
        void attach(camera *cam);

        vector3 position();
        vector3 positionAbsolut();
        void position(vector3 pos);
        void rotation(quaternion rot);
        quaternion rotation();
        quaternion rotationAbsolut();
        void scale(vector3 sc);

        void inheritOrientation(bool inhert);

        void yawAxis(vector3 ax);

        void yaw(real abs);
        void roll(real abs);

    protected:
        Ogre::SceneNode *_node() {return m_node;}
        scene *_scene() {return m_scene;}
    private:
        Ogre::SceneNode *m_node;
        scene *m_scene;
    };

}
}

#endif // GRAPHIC_NODE_HPP
