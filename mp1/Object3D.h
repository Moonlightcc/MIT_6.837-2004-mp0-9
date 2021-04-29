#ifndef  _OBJECT_3D_
#define  _OBJECT_3D_
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"

class Object3D{
private:
    Material* mat;
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    Object3D() {}
    Object3D(Material* material){mat = material;}
    Material* get_material(){return mat;}
    ~Object3D(){}
};

#endif

