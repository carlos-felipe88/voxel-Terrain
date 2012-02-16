#ifndef TRIANGLEVECTOR3_HPP
#define TRIANGLEVECTOR3_HPP

#include "blub/core/vector3.hpp"
#include "blub/core/plane.hpp"
#include "blub/core/line.hpp"
#include "blub/core/log.hpp"

namespace blub
{

struct triangleVector3
{
    triangleVector3()
    {
        ind[0] = vector3(0);
        ind[1] = vector3(0);
        ind[2] = vector3(0);
    }
    triangleVector3(vector3 a, vector3 b, vector3 c)
    {
        ind[0] = a;
        ind[1] = b;
        ind[2] = c;
    }

    bool operator == (const triangleVector3 & other) const
    {
        return other.ind[0] == ind[0]
                && other.ind[1] == ind[1]
                && other.ind[2] == ind[2];
    }

    plane getPlane()
    {
        return plane(ind[0], ind[1], ind[2]);
    }

    vector3 midPoint()
    {
        return (ind[0] + ind[1] + ind[2]) / 3.0;

        line3 ln1(ind[0], ind[1] - ind[0]);
        line3 ln2(ind[1], ind[2] - ind[1]);
        line3 ln3(ln1.getPoint(0.5), ind[2] - ln1.getPoint(0.5));
        line3 ln4(ln2.getPoint(0.5), ind[0] - ln2.getPoint(0.5));
        vector3 mid;
        if (ln3.intersects(ln4, &mid))
        {
            return mid;
        }
        else
            BERROR("FATAL ln3.intersects(ln4, &mid");
        return ind[0];
    }

    vector3 ind[3];
};

}

#endif // TRIANGLEVECTOR3_HPP
