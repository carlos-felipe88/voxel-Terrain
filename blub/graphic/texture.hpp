#ifndef BLUBTEXTURE_HPP
#define BLUBTEXTURE_HPP

#include "blub/core/globals.hpp"
#include "blub/graphic/graphic.hpp"

namespace blub
{
namespace graphic
{

    class texture
    {
    public:
        texture(uint32 width, uint32 height);
        ~texture();

        string name() {
            std::string str = m_tex->getName();
            return m_tex->getName();}
    private:
        Ogre::TexturePtr m_tex;
        static uint32 m_texturesCreated;
    };

}
}


#endif // BLUBTEXTURE_HPP
