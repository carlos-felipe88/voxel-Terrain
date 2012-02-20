#ifndef SCENE_HPP
#define SCENE_HPP

#include "blub/graphic/graphic.hpp"

namespace blub
{
namespace graphic
{
    class system;
    class node;
    class entity;
    namespace sky
    {
        class system;
    }

    class scene
    {
    public:
        friend class sky::system;
        friend class camera;
        friend class manualObject;
        friend class node;
        friend class entity;

        scene(graphic::system *sys);

        graphic::system* system() {return m_system;}
    protected:

    private:
        Ogre::SceneManager *_sceneManager() {return m_mgr;}

        graphic::system *m_system;
        Ogre::SceneManager *m_mgr;
    };

}
}

#endif // SCENE_HPP
