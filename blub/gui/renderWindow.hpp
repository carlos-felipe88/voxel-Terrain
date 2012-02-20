#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include "blub/core/string.hpp"
#include <OGRE/OgreRenderWindow.h>

namespace blub
{
namespace gui
{

class renderWindow
{
public:
    renderWindow(Ogre::RenderWindow *win)
        : m_rw(win)
    {;}
    void customAttribute(string name, void * data)
    {return m_rw->getCustomAttribute(name, data);}

    void saveScreenshot(string prefix, string suffix);
private:
    Ogre::RenderWindow *m_rw;
};

}
}

#endif // RENDERWINDOW_HPP
