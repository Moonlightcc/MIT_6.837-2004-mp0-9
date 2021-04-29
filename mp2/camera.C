#include "camera.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"

OrthographicCamera::OrthographicCamera(Vec3f cp,Vec3f dir, Vec3f u, float size)
{
    center_point = cp;
    direction = dir;
    direction.Normalize();
    Vec3f temp = u;
    up = u-temp*u.Dot3(direction);
    Vec3f::Cross3(horizontal,direction,up);
    image_size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    return Ray(center_point+(point[0]-0.5)*horizontal*image_size+(point[1]-0.5)*up*image_size,direction);
}

float OrthographicCamera::getTMin() const
{
    return FLT_MIN;
}

PerspectiveCamera::PerspectiveCamera(Vec3f &c, Vec3f &dir, Vec3f &u, float a)
{
    center_point = c;
    direction = dir;
    direction.Normalize();
    Vec3f temp = u;
    up = u-temp*u.Dot3(direction);
    Vec3f::Cross3(horizontal,direction,up);
    angle = a;
    virtual_location = 10;
    image_size = 2*virtual_location*tan(angle/2);
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f dir = (direction*virtual_location+(point[0]-0.5)*horizontal*image_size+(point[1]-0.5)*up*image_size);
    dir.Normalize();
    return Ray(center_point,dir);
}

float PerspectiveCamera::getTMin() const
{
    return 0;
}
