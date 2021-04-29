#ifndef _RayTracer_H_
#define _RayTracer_H_

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include "Object3D.h"
#include "group.h"

extern int nx, ny, nz;

class RayTracer
{
public:
	RayTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows)
	{
		SP = s; bounces_num = max_bounces; weight = cutoff_weight; shadows_on = shadows;
		if (nx != 0 && ny != 0 && nz != 0)
		{
			grid = new Grid(s->getGroup()->getBoundingBox(), nx, ny, nz);
			s->getGroup()->insertIntoGrid(grid, nullptr);
			//grid->showInfo();
		}
		else
			grid = new Grid(nullptr,0,0,0);
	}
	~RayTracer() { delete grid; }
	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const;
	Grid* getGrid() { return grid; }
	Vec3f RayCast(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const;
	Vec3f RayCastFast(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const;
private:
	SceneParser* SP;
	int bounces_num;
	float weight;
	bool shadows_on;
	Grid* grid;
};

#endif

