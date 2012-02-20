#ifndef SHAPE_HPP
#define SHAPE_HPP

namespace blub
{
namespace physic
{
    class rigidEntity;
namespace shape
{
    class multiShape;

class shape
{
public:
protected:
    friend class blub::physic::rigidEntity;
    friend class multiShape;
    virtual btCollisionShape* _shape() const = 0;
private:
};

}
}
}


#endif // SHAPE_HPP
