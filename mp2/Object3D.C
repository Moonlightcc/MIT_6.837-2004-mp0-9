#include "Object3D.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "vectors.h"
#include "transform.h"
#include "matrix.h"
bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    float scale = r.getDirection().Length();
    Vec3f temp = r.getDirection();
    temp.Normalize();

    Vec3f R_origin = center- r.getOrigin();
    float R_origin_squa = R_origin.Dot3(R_origin);
    float radius_squa = radius*radius;
    bool out_sphere = (R_origin_squa > radius_squa);
    float time_p = R_origin.Dot3(temp);
    float Squa_Dis = R_origin_squa-time_p*time_p;
    if (Squa_Dis > radius_squa)
        return false;
    float time_prime = sqrt(radius_squa-Squa_Dis);
    
    if (out_sphere)
    {
        float time_new = (time_p - time_prime)/scale;
        if (time_new < h.getT() && time_new > tmin)
        {
            Vec3f sphere_point = r.pointAtParameter(time_new)-center;
            sphere_point.Normalize();
            h.set(time_new,get_material(),sphere_point,r,false);
            return true;
        }
        return false;
    }
    else
    {
        float time_new = (time_p + time_prime)/scale;
        if (time_new < h.getT() && time_new > tmin)
        {
            Vec3f sphere_point = r.pointAtParameter(time_new)-center;
            sphere_point.Normalize();
            h.set(time_new,get_material(),sphere_point,r,true);
            return true;
        }
        return false;
    }
}

/* return True if at least one object intersects */
bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag = false;
    for (int i =0;i<size;i++)
        flag = (ptr_object[i])->intersect(r,h,tmin)|| flag;
    return flag;
}

void Group::addObject(int index, Object3D *obj)
{
    ptr_object[index] = obj;
    size++;
}


Plane::Plane(Vec3f &n, float d, Material *m):Object3D(m)
{
    normal = n;
    normal.Normalize();
    point = d*normal;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    float D_N = normal.Dot3(r.getDirection());
    if(!D_N)
        return false;
    float new_t = (normal.Dot3(point)-normal.Dot3((r.getOrigin())))/normal.Dot3(r.getDirection());
    if (new_t<h.getT() && new_t > tmin)
    {
        h.set(new_t,get_material(),normal,r,false);
        return true;
    }
    return false;
}

/* only calculate the points on the plane */
Vec3f Triangle::get_bary_coor(const Ray& r)
{   
    Vec3f rc = r.getOrigin();
    Vec3f rd = r.getDirection();
    Vec3f a = points[0];
    Vec3f b = points[1];
    Vec3f c = points[2];
    float A = det3x3(a[0]-b[0],a[0]-c[0],rd[0],a[1]-b[1],a[1]-c[1],rd[1],a[2]-b[2],a[2]-c[2],rd[2]);
    Vec3f res = Vec3f(0,0,0);
    res.Set(det3x3(a[0]-rc[0],a[0]-c[0],rd[0],a[1]-rc[1],a[1]-c[1],rd[1],a[2]-rc[2],a[2]-c[2],rd[2])/A,
            det3x3(a[0]-b[0],a[0]-rc[0],rd[0],a[1]-b[1],a[1]-rc[1],rd[1],a[2]-b[2],a[2]-rc[2],rd[2])/A,
            det3x3(a[0]-b[0],a[0]-c[0],a[0]-rc[0],a[1]-b[1],a[1]-c[1],a[1]-rc[1],a[2]-b[2],a[2]-c[2],a[2]-rc[2])/A);
    return res;
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    bool D_N = (abs(normal.Dot3(r.getDirection()))<0.01);
    if(D_N)
        return false;
    Vec3f coor = get_bary_coor(r);
    if(coor[2] < h.getT() && coor[2] > tmin && coor[0] >= 0 && coor[1] >= 0 && coor[0] + coor[1] <= 1)
    {
        h.set(coor[2],get_material(),normal,r,false);
        return true;
    }
    return false;
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f ori_cen = r.getOrigin();
    Matrix inv = ma;
    inv.Inverse();
    inv.Transform(ori_cen);
    Vec3f ori_dir = r.getDirection();
    inv.TransformDirection(ori_dir);
    Ray new_ray = Ray(ori_cen,ori_dir);
    Hit ori_h = h;
    if(ptr_obj->intersect(new_ray,h,tmin))
    {
        inv.Transpose();
        Vec3f n = h.getNormal();
        inv.TransformDirection(n);
        n.Normalize();
        h.set(h.getT(),h.getMaterial(),n,r,h.isInside());
            return true;
    }
    return false;
}