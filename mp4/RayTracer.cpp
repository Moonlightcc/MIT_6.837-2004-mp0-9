#include "RayTracer.h"
#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include "light.h"
#include "Object3D.h"
#include "group.h"
#include "rayTree.h"
#include "glCanvas.h"

#include <iostream>
extern bool shadows;
extern bool test_bool;
Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming);
bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
    float index_i, float index_t, Vec3f& transmitted);

Vec3f RayTracer::traceRay(Ray& ray, float tmin, int bounces, float w, Hit& hit) const
{
    Vec3f ambient_light = SP->getAmbientLight();
    int num_light = SP->getNumLights();

    if (SP->getGroup()->intersect(ray, hit, tmin))
    {
        if (bounces == 0) {
            RayTree::SetMainSegment(ray, tmin, hit.getT());
        }
        Vec3f tot_light = Vec3f(0, 0, 0);
        Material* material = hit.getMaterial();
        Vec3f hit_color = material->getDiffuseColor();
        Vec3f hit_point = hit.getIntersectionPoint();
        tot_light += Vec3f(ambient_light[0] * hit_color[0], ambient_light[1] * hit_color[1], ambient_light[2] * hit_color[2]);
        for (int k = 0; k < num_light; k++)
        {
            Vec3f light_dir;
            Vec3f light_color;
            Vec3f shade_color;
            float light_dis;
            Light* light = SP->getLight(k);
            light->getIllumination(hit_point, light_dir, light_color, light_dis);
            Ray shadow_ray = Ray(hit_point, light_dir);
            Hit shadow_hit = Hit(10000, NULL, Vec3f(0, 0, 0));
            if (shadows)
            {
                if (!SP->getGroup()->intersect(shadow_ray, shadow_hit, 0.001) || shadow_hit.getT()>light_dis)
                {
                    shade_color = material->Shade(ray, hit, light_dir, light_color,light_dis);
                    tot_light += shade_color;
                }
            }
            else
            {
                shade_color = material->Shade(ray, hit, light_dir, light_color, light_dis);
                tot_light += shade_color;
            }
            RayTree::AddShadowSegment(shadow_ray, tmin, shadow_hit.getT());
        }

        Vec3f mat_ref_color = material->getReflectiveColor();
        float dec = mat_ref_color.Length();
        if (bounces < bounces_num && w > weight && dec > 0)
        {
            Vec3f reflect_dir = mirrorDirection(hit.getNormal(), ray.getDirection());
            Ray reflect_ray = Ray(hit_point, reflect_dir);
            Hit reflect_hit = Hit(10000, NULL, Vec3f(0, 0, 0));
            Vec3f reflect_color = traceRay(reflect_ray, 0.001, bounces + 1, w * dec, reflect_hit);
            tot_light += reflect_color * mat_ref_color;
            RayTree::AddReflectedSegment(reflect_ray, tmin, reflect_hit.getT());
        }
        
        Vec3f mat_tra_color = material->getTransparentColor();
        float dec_tra = mat_tra_color.Length();
        Vec3f transmitted_dir;
        bool par_or_not = transmittedDirection(hit.getNormal(), ray.getDirection(), 1, material->getIndexOfRefraction(), transmitted_dir);
        if (bounces < bounces_num && w > weight && dec_tra > 0 && par_or_not)
        {
            Ray trans_ray = Ray(hit_point, transmitted_dir);
            Hit trans_hit = Hit(10000, NULL, Vec3f(0, 0, 0));
            Vec3f trans_color = traceRay(trans_ray, 0.001, bounces + 1, w * dec_tra, trans_hit);
            tot_light += trans_color * mat_tra_color;
            RayTree::AddTransmittedSegment(trans_ray, tmin, trans_hit.getT());
        }
        return tot_light;
    }
    else
        return SP->getBackgroundColor();
    
}

Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming)
{
    Vec3f normal_deviator =  normal.Dot3(incoming) * normal;
    Vec3f tang_deviator = incoming - normal_deviator;
    return tang_deviator + (-1) * normal_deviator;
}

bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
    float index_i, float index_t, Vec3f& transmitted)
{
    float pro = incoming.Dot3(normal);
    if (pro < 0)
    {
        float temp = sqrt(1 - pow(pro,2));
        float sin_sita = index_i * temp / index_t;
        if (abs(sin_sita) > 1.01)
            return false;
        float cos_sita = sqrt(1 - pow(sin_sita, 2));
        Vec3f v, b;
        v = -1*normal;
        b = incoming - incoming.Dot3(normal) * normal;
        b.Normalize();
        transmitted = v * cos_sita + b * sin_sita;
        return true;
    }
    else
    {
        float temp = sqrt(1 - pow(pro, 2));
        float sin_sita = index_t * temp / index_i;
        if (abs(sin_sita) > 1.01)
            return false;
        float cos_sita = sqrt(1 - pow(sin_sita, 2));
        Vec3f v, b;
        v = normal;
        b = incoming - incoming.Dot3(normal) * normal;
        b.Normalize();
        transmitted = v * cos_sita + b * sin_sita;
        return true;
    }
}