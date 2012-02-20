#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <blub/core/triangle.hpp>

namespace blub
{

template <typename vertexType>
class geometry
{
public:
    geometry()
    {;}

    void addVertices(const blub::vector<vertexType> *vert);
    void addVertices(const vertexType *vert, blub::uint32 num);
    void addTriangles(const blub::vector<blub::triangle> *tri);
    void addTriangles(const blub::triangle *indices, blub::uint32 num);
    void removeTriangles(const blub::vector<blub::triangle> *tri);
    void removeTriangles(const blub::triangle *tri, blub::uint32 num);

    blub::vector<vertexType> *vertices() {return &m_vertices;}
    blub::vector<blub::triangle> *triangles() {return &m_triangles;}
private:
    blub::vector<vertexType> m_vertices;
    blub::vector<blub::triangle> m_triangles;
};

}

#endif // GEOMETRY_HPP
