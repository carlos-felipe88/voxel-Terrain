#ifndef QUADVECTOR3_HPP
#define QUADVECTOR3_HPP

#include "blub/core/vector3.hpp"
#include "blub/core/plane.hpp"
#include "blub/core/vector.hpp"
#include "blub/core/triangleVector3.hpp"

namespace blub
{

struct quadVector3
{
    quadVector3()
    {
        ind[0] = vector3(0);
        ind[1] = vector3(0);
        ind[2] = vector3(0);
        ind[3] = vector3(0);
    }
    quadVector3(vector3 a, vector3 b, vector3 c, vector3 d)
    {
        ind[0] = a;
        ind[1] = b;
        ind[2] = c;
        ind[3] = d;
    }
    quadVector3(triangleVector3 a, triangleVector3 b)
    {
        /*vector<vector3> points;
        for (uint16 index = 0; index < 3; ++index)
        {
            points.push_back_distinct(a.ind[index]);
        }
        for (uint16 index = 0; index < 3; ++index)
        {
            points.push_back_distinct(b.ind[index]);
        }
        assert(points.size() == 4);
        for (uint16 index = 0; index < 4; ++index)
        {
            ind[index] = points.at(index);
        }*/
        /*ind[0] = a.ind[0];
        ind[1] = b.ind[1];
        ind[2] = a.ind[1];
        ind[3] = a.ind[2];*/

        vector<vector3> notSamePoints;
        for (uint16 index = 0; index < 3; ++index)
        {
            if (notSamePoints.indexOf(a.ind[index]) == -1)
                notSamePoints.push_back(a.ind[index]);
        }
        for (uint16 index = 0; index < 3; ++index)
        {
            if (notSamePoints.indexOf(b.ind[index]) == -1)
                notSamePoints.push_back(b.ind[index]);
        }
        if (notSamePoints.size() != 4)
            BERROR("FATAL notSamePoints.size() != 4");
        for (int index = 0; index < notSamePoints.size(); ++index)
        {
            ind[index] = notSamePoints.at(index);
        }
    }

    bool operator == (const quadVector3 & other) const
    {
        return other.ind[0] == ind[0]
                && other.ind[1] == ind[1]
                && other.ind[2] == ind[2]
                && other.ind[3] == ind[3];
    }

    vector3 midPoint()
    {
        return (ind[0] + ind[1] + ind[2] + ind[3]) / 4.0;
    }

    bool combine(const quadVector3 &other, quadVector3* result)
    {
        // same plane?
        plane mine(ind[0], ind[1], ind[2]);
        plane oPlane(other.ind[0], other.ind[1], other.ind[2]);
        if (mine != oPlane)
        {
            BOUT("aha0");
            return false;
        }
        // find same points
        vector<vector3> samePoints;
        for (uint16 index = 0; index < 4; ++index)
        {
            for (uint16 index2 = 0; index2 < 4; ++index2)
            {
                if (ind[index] == other.ind[index2] && samePoints.indexOf(ind[index]) == -1)
                    samePoints.push_back(ind[index]);
            }
        }
        if (samePoints.size() > 2)
        {
            BERROR("Fatal samePoints.size() > 2 4 expected: " + string::number(samePoints.size()));
            return false;
        }
        if (samePoints.size() != 2)
            return false;
        // find points not same
        vector<vector3> notSamePoints;
        for (uint16 index = 0; index < 4; ++index)
        {
            if (samePoints.indexOf(ind[index]) == -1)
                notSamePoints.push_back(ind[index]);
            if (samePoints.indexOf(other.ind[index]) == -1)
                notSamePoints.push_back(other.ind[index]);
        }
        if (notSamePoints.size() != 4)
        {
            BERROR("Fatal notSamePoints.size() != 4: " + string::number(notSamePoints.size()));
            return false;
        }
        // do new quad
        uint16 counter(0);
        for (uint16 index = 0; index < 4; ++index)
        {
            if (notSamePoints.indexOf(ind[index]) != -1)
            {
                result->ind[counter] = ind[index];
                ++counter;
            }
        }
        for (uint16 index = 0; index < 4; ++index)
        {
            if (notSamePoints.indexOf(other.ind[index]) != -1)
            {
                result->ind[counter] = other.ind[index];
                ++counter;
            }
        }/*
        result->ind[0] = notSamePoints.at(0);
        result->ind[1] = notSamePoints.at(1);
        result->ind[2] = notSamePoints.at(2);
        result->ind[3] = notSamePoints.at(3);*/
        /*notSamePoints.removeAt(0);
        for (uint32 ind = 0; ind < 3; ++ind)
        {
            for (uint32 ind2 = 0; ind2 < notSamePoints.size(); ++ind2)
            {
                uint16 counter(0);
                if (notSamePoints.at(ind2).x == result->ind[ind].x)
                    ++counter;
                if (notSamePoints.at(ind2).y == result->ind[ind].y)
                    ++counter;
                if (notSamePoints.at(ind2).z == result->ind[ind].z)
                    ++counter;
                if (counter == 2)
                {
                    result->ind[ind+1] = notSamePoints.at(ind2);
                    notSamePoints.removeAt(ind2);
                    break;
                }
            }
        }*/

        /*
        uint16 counter(0);
//        uint16 numFirst(0);
        uint16 indexFirst(0);
        uint16 indexSecond(0);
//        uint16 numSecond(0);
        while (counter < 4)
        {
            while (indexFirst < 4 && counter < 4)
            {
                if (samePoints.indexOf(ind[indexFirst]) == -1)
                {
                    result->ind[counter] = ind[indexFirst];
                    ++indexFirst;
                    ++counter;
                }
                else
                {
                    ++indexFirst;
                    break;
                }
            }
            while (indexSecond < 4 && counter < 4)
            {
                if (samePoints.indexOf(other.ind[indexSecond]) == -1)
                {
                    result->ind[counter] = other.ind[indexSecond];
                    ++indexSecond;
                    ++counter;
                }
                else
                {
                    ++indexSecond;
                    break;
                }
            }
        }*/
        return true;
    }

    vector3 ind[4];
};

}

#endif // QUADVECTOR3_HPP
