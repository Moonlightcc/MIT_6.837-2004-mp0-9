#ifndef _Camera_H_
#define _Camera_H_
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"
#define  FLT_MIN -10000
class Camera{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera{
private:
    Vec3f center_point;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float image_size;
public:
    OrthographicCamera(Vec3f cp,Vec3f dir, Vec3f u, float size);
    Ray generateRay(Vec2f point);
    float getTMin() const;
    int get_image_size(){return image_size;}
};

class PerspectiveCamera : public Camera{
private:
    Vec3f center_point;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float angle;
    float virtual_location;
    float image_size;
public:
    PerspectiveCamera(Vec3f &c, Vec3f &dir, Vec3f &u, float a);
    Ray generateRay(Vec2f point);
    float getTMin() const;
    float get_virtual_location(){return virtual_location;}
};
#endif
