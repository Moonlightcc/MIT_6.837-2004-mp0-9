#ifndef _PLANE_H_
#define _PLANE_H_

#include "Object3D.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"

class Plane : public Object3D{
private:
    Vec3f normal;
    Vec3f point;
public:
    Plane(Vec3f &n, float d, Material *m);
    bool intersect(const Ray &r, Hit &h, float tmin);
};
#endif