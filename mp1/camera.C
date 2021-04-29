#include "camera.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"

OrthographicCamera::OrthographicCamera(Vec3f cp,Vec3f dir, Vec3f u, int size)
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