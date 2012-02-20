#include "texture.hpp"

#include <OGRE/OgreHardwarePixelBuffer.h>
#include <OGRE/OgreTextureManager.h>

#ifdef BLUB_WIN
#define M_PI 3.14159265358979323846
#endif

using namespace blub;
using namespace blub::graphic;

uint32 texture::m_texturesCreated = 0;

texture::texture(uint32 width, uint32 height)
{
    m_tex = Ogre::TextureManager::getSingleton().createManual(
            string::number(graphic::texture::m_texturesCreated),
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_A8R8G8B8, Ogre::TU_DYNAMIC_WRITE_ONLY);
    graphic::texture::m_texturesCreated++;

    Ogre::HardwarePixelBufferSharedPtr mTexBuf = m_tex->getBuffer();

    // initialise the texture to have full luminance
    mTexBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);

    uint32* data = static_cast<uint32*>(mTexBuf->getCurrentLock().data);
    uint8 alpha(255);
    uint8 red(0);
    uint8 green(0);
    uint8 blue(0);
    for (uint32 ind = 0; ind < mTexBuf->getWidth()*mTexBuf->getHeight(); ind++)
    {
        double posX = (double)(ind % mTexBuf->getWidth()) / (double)mTexBuf->getWidth();
        double posY = (double)(ind / mTexBuf->getWidth()) / (double)mTexBuf->getHeight();
        red = green = blue = ( (cos(posX*8.0*M_PI)+1.0)*128.0 + (sin(posY*8.0*M_PI)+1.0)*128.0 ) /2.0;

        /*red = qrand()%256;
        green = qrand()%256;
        blue = qrand()%256;*/

        data[ind] = 256*256*256*alpha + 256*256*red + 256*green + blue + 256*256*256*alpha;
    }

    mTexBuf->unlock();
}

texture::~texture()
{
    Ogre::TextureManager::getSingleton().remove(m_tex->getName());
}
