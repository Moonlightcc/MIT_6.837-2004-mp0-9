#include "MarchingInfo.h"
#include "raytracing_stats.h"

void MarchingInfo::nextCell()
{
	RayTracingStats::IncrementNumGridCellsTraversed();
	float t_x = tnext[0];
	float t_y = tnext[1];
	float t_z = tnext[2];

	if (t_x <= t_y && t_x <= t_z)
	{
		curindex[0] += sign[0];
		tcur = t_x;
		tnext[0] += dt[0];
		normal = Vec3f(-1, 0, 0) * sign[0];
	}
	else if (t_y <= t_x && t_y <= t_z)
	{
		curindex[1] += sign[1];
		tcur = t_y;
		tnext[1] += dt[1];
		normal = Vec3f(0, -1, 0) * sign[1];
	}
	else
	{
		curindex[2] += sign[2];
		tcur = t_z;
		tnext[2] += dt[2];
		normal = Vec3f(0, 0, -1) * sign[2];
	}
}