#ifndef BLUBGRAPHIC_HPP
#define BLUBGRAPHIC_HPP

#include "blub/core/log.hpp"
#include "blub/core/mutex.hpp"
#include "blub/gui/renderWindow.hpp"
#include "blub/core/frameListener.hpp"

#include <OGRE/OgreRoot.h>

// #include <OGRE/RenderSystems/GL/OgreGLRenderSystem.h>



namespace blub
{
namespace graphic
{
    class scene;
    class camera;

    class system : protected Ogre::FrameListener, public mutex, public frameListener
    {
    public:
        friend class blub::graphic::scene;
        friend class blub::graphic::camera;

        struct resourceType
        {
            enum Enum
            {
                fileSystem,
                zip
            };
        };

        class texture;
        class mesh;

        system();
        ~system();
        bool initialise();
        void enterRenderLoop();

        uint32 numBatchesRendered();
        uint32 numTrianglesRendered();

        gui::renderWindow * renderWindow()
        { return m_renderWin;}

    protected:
        bool frameStarted(const Ogre::FrameEvent &evt);

        Ogre::Root* _root() {return &m_root;}
    private:
        Ogre::Root m_root;
//        Ogre::GLRenderSystem m_rsGl;

        bool m_ogl;
        bool m_dx;

        gui::renderWindow * m_renderWin;
    };
}
}

#endif // BLUBGRAPHIC_HPP
