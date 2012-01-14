#include "editSphere.hpp"
#include "blub/procedural/voxelMap/map3.hpp"

using namespace blub;
using namespace blub::procedural::voxelMap;

editSphere::editSphere(sphere sp, bool cut)
    : edit(cut)
    , m_sp(sp)
{

}

axisAlignedBox editSphere::aab(real scale) const
{
    sphere sp = scaleSphere(m_sp, scale);
    axisAlignedBox aabb(sp.getCenter().x-((int32)sp.getRadius()+1), sp.getCenter().y-((int32)sp.getRadius()+1), sp.getCenter().z-((int32)sp.getRadius()+1),
                        sp.getCenter().x+((int32)sp.getRadius()+1), sp.getCenter().y+((int32)sp.getRadius()+1), sp.getCenter().z+((int32)sp.getRadius()+1));
    return aabb;
}

real editSphere::calcVoxel(const vector3& pos, const real &scale)
{
    sphere sp = scaleSphere(m_sp, scale);
    real dist = sp.getRadius() - sp.getCenter().distance(pos);
    return dist;
}


sphere editSphere::scaleSphere(const sphere _sp, const real scale)
{
    sphere sp = _sp;
    sp.center(sp.center()/scale);
    sp.radius(sp.radius()/scale);
    return sp;
}
