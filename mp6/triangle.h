#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Object3D.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"

class Triangle : public Object3D{
private:
    Vec3f points[3];
    Vec3f normal;
public:
    Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m) :Object3D(m)
    {
        points[0] = a; points[1] = b; points[2] = c; Vec3f::Cross3(normal, a - b, b - c); normal.Normalize();
        BBox = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY),Vec3f(-INFINITY, -INFINITY, -INFINITY));
        BBox->Extend(a); BBox->Extend(b); BBox->Extend(c);
    }
    ~Triangle() { delete BBox; }
    Vec3f get_bary_coor(const Ray& r);
    bool intersect(const Ray &r, Hit &h, float tmin);
    void paint();
    void insertIntoGrid(Grid* g, Matrix* m);
};

#endif