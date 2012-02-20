#ifndef SKY_HPP
#define SKY_HPP

#include "blub/core/globals.hpp"
#include "blub/graphic/camera.hpp"
#include "blub/graphic/scene.hpp"


namespace blub
{
namespace graphic
{
namespace sky
{

    class system
    {
    public:
        system(scene *sc);

    private:
        bool update(real timeSinceLastFrame);
    };


}
}
}

#endif // SKY_HPP
