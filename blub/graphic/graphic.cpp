#include "graphic.hpp"
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreRenderWindow.h>

using namespace blub;
using namespace blub::graphic;

system::system()
#ifdef BLUB_DEBUG
    : m_root("../data/config/ogrePlugins_d.cfg", "../data/config/ogre.cfg", "../data/logs/ogre.log")
#else
    : m_root("../data/config/ogrePlugins.cfg", "../data/config/ogre.cfg", "../data/logs/ogre.log")
#endif
{
    Ogre::LogManager::getSingleton().getDefaultLog()->setDebugOutputEnabled(true);

/*
    try
    {
#ifdef BLUB_LINUX
#ifdef BLUB_DEBUG
        m_root.loadPlugin("../../dependencies/linux/lib64/Plugin_CgProgramManager_d.so");
        m_root.loadPlugin("../../dependencies/linux/lib64/RenderSystem_GL_d.so");
        m_root.loadPlugin("../../dependencies/linux/lib64/Plugin_OctreeSceneManager_d.so");
#else
        m_root.loadPlugin("../../dependencies/linux/lib64/Plugin_CgProgramManager.so");
        m_root.loadPlugin("../../dependencies/linux/lib64/RenderSystem_GL.so");
        m_root.loadPlugin("../../dependencies/linux/lib64/Plugin_OctreeSceneManager.so");
#endif
        // m_root.addRenderSystem(&m_rsGl);
//        m_root.loadPlugin("/usr/lib/OGRE/RenderSystem_GL");
#else
//        m_root.loadPlugin("RenderSystem_GL");
#endif
#ifdef BLUB_WIN
        m_root.loadPlugin("Plugin_CgProgramManager");
        m_root.loadPlugin("Plugin_OctreeSceneManager");
        m_root.loadPlugin("RenderSystem_GL");
#endif
        m_ogl = true;
    }
    catch (...)
    {
        m_ogl = false;
    }
*/
/*#ifdef BLUB_WIN
    try
    {
        m_root.loadPlugin("RenderSystem_Direct3D9");
        m_dx = true;
    }
    catch (...)
    {
        m_dx = false;
    }
#endif*/
/*    if (!m_ogl && !m_dx)
    {
        BERROR("Could neither load DirectX nor OpenGL");
        //return false;
    }
*/
    /*Ogre::RenderSystem *rs;

    if (false)
    {

    }
    else
    {
        if (m_ogl)
        {
            rs = m_root.getRenderSystemByName("OpenGL Rendering Subsystem");
            rs->setConfigOption("Full Screen", "No");
            rs->setConfigOption("Video Mode", "1024 x 768");
        }
        else
        {
            rs = m_root.getRenderSystemByName("Direct3D9 Rendering Subsystem");
            rs->setConfigOption("Full Screen", "No");
            rs->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
        }
    }
    m_root.setRenderSystem(rs);*/

}

system::~system()
{
    /*delete m_renderWin;
    delete m_camera;*/
    m_root.shutdown();
    BOUT("graphic shutted down");
}

bool system::initialise()
{
    if (!m_root.restoreConfig())
        m_root.showConfigDialog();

    m_root.initialise(true);
    m_renderWin = new gui::renderWindow(m_root.getAutoCreatedWindow());

    // resources
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("../data/config/resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    // Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

void system::enterRenderLoop()
{
    lock();
    m_root.addFrameListener(this);
    m_root.startRendering();
    unlock();
}

bool system::frameStarted(const Ogre::FrameEvent &evt)
{
    Ogre::RenderWindow *win = m_root.getAutoCreatedWindow();
    if (win->isClosed())
        return false;
    unlock();
    frame(evt.timeSinceLastFrame);
    lock();
    return true;
}

uint32 system::numBatchesRendered()
{
    return m_root.getAutoCreatedWindow()->getStatistics().batchCount;
}

uint32 system::numTrianglesRendered()
{
    return m_root.getAutoCreatedWindow()->getStatistics().triangleCount;
}
