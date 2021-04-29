#ifndef _RayTracer_H_
#define _RayTracer_H_

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "vectors.h"

class RayTracer
{
public:
	RayTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows) 
	{ SP = s; bounces_num = max_bounces; weight = cutoff_weight; shadows_on = shadows;}
	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const;
private:
	SceneParser* SP;
	int bounces_num;
	float weight;
	bool shadows_on;
};

#endif

