#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "Object3D.h"

class Sphere : public Object3D{
private:
    Vec3f center;
    float radius;
public:
    Sphere(Vec3f c, float r, Material* m) : Object3D(m) {
        center = c; radius = r;
        BBox = new BoundingBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }
    ~Sphere() { delete BBox; }
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    Vec3f get_center(){return center;}
    float get_radius(){return radius;}
    virtual void insertIntoGrid(Grid* g, Matrix* m);
    void paint();
};
#endif
