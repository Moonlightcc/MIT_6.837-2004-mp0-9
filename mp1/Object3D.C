#include "Object3D.h"
#include "group.h"
#include "sphere.h"
bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f R_origin = center- r.getOrigin();
    float R_origin_squa = R_origin.Dot3(R_origin);
    float radius_squa = radius*radius;
    bool out_sphere = (R_origin_squa > radius_squa);
    float time_p = R_origin.Dot3(r.getDirection());
    float Squa_Dis = R_origin_squa-time_p*time_p;
    if (Squa_Dis > radius_squa)
        return false;
    float time_prime = sqrt(radius_squa-Squa_Dis);
    if (out_sphere)
    {
        float time_new = time_p - time_prime;
        if (time_new < h.getT() && time_new > tmin)
        {
            h.set(time_new,get_material(),r);
            return ture;
        }
        return false;
    }
    else
    {
        float time_new = time_p - time_prime;
        if (time_new < h.getT() && time_new > tmin)
        {
            h.set(time_new,get_material(),r);
            return ture;
        }
        return false;
    }
}

/* return True if at least one object intersects */
bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag = false;
    for (int i =0;i<num;i++)
    {
        flag = (ptr_object[i])->intersect(r,h,tmin)|| flag;     
    }
    return flag;
}

void Group::addObject(int index, Object3D *obj)
{
    ptr_object[index] = obj;
    num += 1;
}