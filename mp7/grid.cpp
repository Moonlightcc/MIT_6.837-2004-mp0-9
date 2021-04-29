#include "Object3D.h"
#include "GL/GLUT.h"

int counter = 0;
extern bool visualize_grid;
extern bool test;
void Grid::paint()
{
	Vec3f _min = getBoundingBox()->getMin();
	Vec3f _max = getBoundingBox()->getMax();

	float dx = (_max[0] - _min[0]) / float(x);
	float dy = (_max[1] - _min[1]) / float(y);
	float dz = (_max[2] - _min[2]) / float(z);

	for (int i=0;i<x;i++)
		for(int j=0;j<y;j++)
			for (int k = 0; k < z; k++)
			{
				int num = getNumObject(i, j, k);
				if (num)
				{
					Vec3f points[8] = {
						_min + Vec3f(i * dx,j * dy,k * dz),_min + Vec3f(i * dx,j * dy,k * dz + dz),
						_min + Vec3f(i * dx,j * dy + dy,k * dz),_min + Vec3f(i * dx,j * dy + dy,k * dz + dz),
						_min + Vec3f(i * dx + dx,j * dy,k * dz),_min + Vec3f(i * dx + dx,j * dy,k * dz + dz),
						_min + Vec3f(i * dx + dx,j * dy + dy,k * dz),_min + Vec3f(i * dx + dx,j * dy + dy,k * dz + dz)
					};

					PhongMaterial* m;
					switch (num) {
					case 1: m = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 2: m = new PhongMaterial(Vec3f(1, 0, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 3: m = new PhongMaterial(Vec3f(0, 1, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 4: m = new PhongMaterial(Vec3f(1, 1, 0), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 5: m = new PhongMaterial(Vec3f(0.3, 0, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 6: m = new PhongMaterial(Vec3f(0.7, 0, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 7: m = new PhongMaterial(Vec3f(0, 0.3, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 8: m = new PhongMaterial(Vec3f(0, 0.7, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 9: m = new PhongMaterial(Vec3f(0, 0.3, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					case 10: m = new PhongMaterial(Vec3f(0, 0.7, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					default: m = new PhongMaterial(Vec3f(1, 0, 0), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
					}
					m->glSetMaterial();
					glBegin(GL_QUADS);

					// bottom
					if (i == 0 || !getNumObject(i - 1, j, k))
					{
						glNormal3f(1, 0, 0);
						glVertex3f(points[0][0], points[0][1], points[0][2]);
						glVertex3f(points[1][0], points[1][1], points[1][2]);
						glVertex3f(points[3][0], points[3][1], points[3][2]);
						glVertex3f(points[2][0], points[2][1], points[2][2]);
					}
					// up
					if (i == x - 1 || !getNumObject(i + 1, j, k))
					{
						glNormal3f(-1, 0, 0);
						glVertex3f(points[4][0], points[4][1], points[4][2]);
						glVertex3f(points[5][0], points[5][1], points[5][2]);
						glVertex3f(points[7][0], points[7][1], points[7][2]);
						glVertex3f(points[6][0], points[6][1], points[6][2]);
					}
					// left
					if (j == 0 || !getNumObject(i, j-1, k))
					{
						glNormal3f(0, 1, 0);
						glVertex3f(points[0][0], points[0][1], points[0][2]);
						glVertex3f(points[1][0], points[1][1], points[1][2]);
						glVertex3f(points[5][0], points[5][1], points[5][2]);
						glVertex3f(points[4][0], points[4][1], points[4][2]);
					}
					// right
					if (j == y-1 || !getNumObject(i, j+1, k))
					{
						glNormal3f(0, -1, 0);
						glVertex3f(points[2][0], points[2][1], points[2][2]);
						glVertex3f(points[3][0], points[3][1], points[3][2]);
						glVertex3f(points[7][0], points[7][1], points[7][2]);
						glVertex3f(points[6][0], points[6][1], points[6][2]);
					}
					//front
					if (k == 0 || !getNumObject(i, j, k-1))
					{
						glNormal3f(0, 0, 1);
						glVertex3f(points[0][0], points[0][1], points[0][2]);
						glVertex3f(points[2][0], points[2][1], points[2][2]);
						glVertex3f(points[6][0], points[6][1], points[6][2]);
						glVertex3f(points[4][0], points[4][1], points[4][2]);
					}
					// back
					if (k == z - 1 || !getNumObject(i, j, k + 1))
					{
						glNormal3f(0, 0, -1);
						glVertex3f(points[1][0], points[1][1], points[1][2]);
						glVertex3f(points[3][0], points[3][1], points[3][2]);
						glVertex3f(points[7][0], points[7][1], points[7][2]);
						glVertex3f(points[5][0], points[5][1], points[5][2]);
					}
					glEnd();
					delete m;
				}

			}
}

void Grid::initializeRayMarch(MarchingInfo& mi, const Ray& r, float tm)
{
	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();

	BoundingBox* box = getBoundingBox();
	Vec3f _min = box->getMin();
	Vec3f _max = box->getMax();

	float dx = fabs((_max[0] - _min[0]) / float(x) / direction[0]);
	float dy = fabs((_max[1] - _min[1]) / float(y) / direction[1]);
	float dz = fabs((_max[2] - _min[2]) / float(z) / direction[2]);

	mi.setdt(0, dx);
	mi.setdt(1, dy);
	mi.setdt(2, dz);

	int sign;
	sign = (direction[0] > 0) ? 1 : -1;
	mi.setsign(0, sign);
	sign = (direction[1] > 0) ? 1 : -1;
	mi.setsign(1, sign);
	sign = (direction[2] > 0) ? 1 : -1;
	mi.setsign(2, sign);

	float t1_x = (_min[0] - origin[0])/direction[0];
	float t2_x = (_max[0] - origin[0])/direction[0];
	if (t1_x > t2_x)
		swap(t1_x, t2_x);
	float t1_y = (_min[1] - origin[1]) / direction[1];
	float t2_y = (_max[1] - origin[1]) / direction[1];
	if (t1_y > t2_y)
		swap(t1_y, t2_y);
	float t1_z = (_min[2] - origin[2]) / direction[2];
	float t2_z = (_max[2] - origin[2]) / direction[2];
	if (t1_z > t2_z)
		swap(t1_z, t2_z);

	float t_near = max(max(t1_x, t1_y), t1_z);
	float t_far = min(min(t2_x, t2_y), t2_z);
	// don't hit
	if (t_far < t_near || t_far < tm)
		return;

	if (t_near < tm)
	{
		//inside
		//avoid parallel light
		if (t1_x > -INFINITY)
			while (t1_x < tm)
				t1_x += dx;
		if (t1_y > -INFINITY)
			while (t1_y < tm)
				t1_y += dy;
		if (t1_z > -INFINITY)
			while (t1_z < tm)
				t1_z += dz;
		t1_x = t1_x >= tm ? t1_x : INFINITY;
		t1_y = t1_y >= tm ? t1_y : INFINITY;
		t1_z = t1_z >= tm ? t1_z : INFINITY;

		float temp = min(min(t1_x, t1_y), t1_z);
		mi.settcur(min(tm,temp));
		if (temp == t1_x) {
			mi.setnormal(Vec3f(-1, 0, 0) * mi.getsign(0));
			mi.settnext(0, t1_x);
			mi.settnext(1, t1_y);
			mi.settnext(2, t1_z);
		}
		else if (temp == t1_y) {
			mi.setnormal(Vec3f(0, -1, 0) * mi.getsign(1));
			mi.settnext(0, t1_x);
			mi.settnext(1, t1_y);
			mi.settnext(2, t1_z);
		}
		else {
			mi.setnormal(Vec3f(0, 0, -1) * mi.getsign(2));
			mi.settnext(0, t1_x);
			mi.settnext(1, t1_y);
			mi.settnext(2, t1_z);
		}
	}
	else
	{
		//outside
		mi.settcur(t_near);
		if (t_near == t1_x) mi.setnormal(Vec3f(-1, 0, 0) * mi.getsign(0));
		if (t_near == t1_y) mi.setnormal(Vec3f(0, -1, 0) * mi.getsign(1));
		if (t_near == t1_z) mi.setnormal(Vec3f(0, 0, -1) * mi.getsign(2));
		if (t1_x > -INFINITY) {
			while (t1_x <= t_near)
				t1_x += dx;
			mi.settnext(0,t1_x);
		}
		if (t1_y > -INFINITY) {
			while (t1_y <= t_near)
				t1_y += dy;
			mi.settnext(1, t1_y);
		}
		if (t1_z > -INFINITY) {
			while (t1_z <= t_near)
				t1_z += dz;
			mi.settnext(2, t1_z);
		}
	}

	float vx = (_max[0] - _min[0]) / float(x);
	float vy = (_max[1] - _min[1]) / float(y);
	float vz = (_max[2] - _min[2]) / float(z);

	Vec3f c = origin + direction * t_near - _min;
	mi.setindex(0,int(c[0] / vx));
	if (mi.getsign(0) < 0 && mi.getindex(0) == x) mi.setindex(0, x-1);
	mi.setindex(1, int(c[1] / vy));
	if (mi.getsign(1) < 0 && mi.getindex(1) == y) mi.setindex(1, y-1);
	mi.setindex(2, int(c[2] / vz));
	if (mi.getsign(2) < 0 && mi.getindex(2) == z) mi.setindex(2, z-1);
}

bool Grid::intersect(const Ray& r, Hit& h, float tmin)
{
	MarchingInfo mi;
	initializeRayMarch(mi, r, tmin);
	//InitAllObjDetect();
	bool intersect_in_grid = false;
	bool intersect_in_infite = false;
	
	if (mi.gettcur() < h.getT())
	{
		int index_x = mi.getindex(0);
		int index_y = mi.getindex(1);
		int index_z = mi.getindex(2);
		
		while (index_x >= 0 && index_x < x && index_y >= 0 && index_y < y && index_z >= 0 && index_z < z)
		{
			int num = getNumObject(index_x, index_y, index_z);
			if (visualize_grid && num)
			{
				PhongMaterial* m;
				switch (num) {
				case 1: m = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 2: m = new PhongMaterial(Vec3f(1, 0, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 3: m = new PhongMaterial(Vec3f(0, 1, 1), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 4: m = new PhongMaterial(Vec3f(1, 1, 0), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 5: m = new PhongMaterial(Vec3f(0.3, 0, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 6: m = new PhongMaterial(Vec3f(0.7, 0, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 7: m = new PhongMaterial(Vec3f(0, 0.3, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 8: m = new PhongMaterial(Vec3f(0, 0.7, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 9: m = new PhongMaterial(Vec3f(0, 0.3, 0.7), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				case 10: m = new PhongMaterial(Vec3f(0, 0.7, 0.3), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				default: m = new PhongMaterial(Vec3f(1, 0, 0), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1); break;
				}
				h.set(mi.gettcur(), m, mi.getnormal(), r, false);
				return true;
			}
			else if (!visualize_grid && num)
			{
				for (int i = 0; i < num; i++)
				{
					Object3D* obj = get(index_x, index_y, index_z, i);
					//if (obj->isDetect())
					//	continue;
					//if (!obj->intersect(r, h, tmin))
						//obj->SetDetect();
					obj->intersect(r, h, tmin);
				}
				if (h.getT() < mi.gettnext(0)+0.0001f && h.getT() < mi.gettnext(1) + 0.0001f && h.getT() < mi.gettnext(2) + 0.0001f)
				{
					intersect_in_grid = true;
					break;
				}
			}
			mi.nextCell();
			index_x = mi.getindex(0);
			index_y = mi.getindex(1);
			index_z = mi.getindex(2);
		}
	}

	int num_inf = getNumInfiniteObject();
	if (!visualize_grid && num_inf)
	{
		for (int i = 0; i < num_inf; i++)
			if (getInfinitePri(i)->intersect(r,h,tmin))
				intersect_in_infite = true;
	}
	return intersect_in_grid|| intersect_in_infite;
}

bool Grid::ShodowJudge(const Ray& r, Hit& h, float tmin)
{
	Vec3f _min = getBoundingBox()->getMin();
	Vec3f _max = getBoundingBox()->getMax();

	float dx = (_max[0] - _min[0]) / float(x);
	float dy = (_max[1] - _min[1]) / float(y);
	float dz = (_max[2] - _min[2]) / float(z);
	MarchingInfo mi;
	initializeRayMarch(mi, r, tmin);
	bool intersect_in_grid = false;
	bool intersect_in_infite = false;

	if (mi.gettcur() < h.getT())
	{
		int index_x = mi.getindex(0);
		int index_y = mi.getindex(1);
		int index_z = mi.getindex(2);

		while (index_x >= 0 && index_x < x && index_y >= 0 && index_y < y && index_z >= 0 && index_z < z)
		{
			int num = getNumObject(index_x, index_y, index_z);
			if (!visualize_grid && num)
			{
				for (int i = 0; i < num; i++)
				{
					Object3D* obj = get(index_x, index_y, index_z, i);
					obj->intersect(r, h, tmin);
				}
				if (h.getT() < mi.gettnext(0) + 0.001f && h.getT() < mi.gettnext(1) + 0.001f && h.getT() < mi.gettnext(2) + 0.001f)
				{
					intersect_in_grid = true;
					break;
				}
			}
			mi.nextCell();
			index_x = mi.getindex(0);
			index_y = mi.getindex(1);
			index_z = mi.getindex(2);
		}
	}

	int num_inf = getNumInfiniteObject();
	if (!visualize_grid && num_inf)
	{
		for (int i = 0; i < num_inf; i++)
			if (getInfinitePri(i)->intersect(r, h, tmin))
				intersect_in_infite = true;
	}
	return intersect_in_grid || intersect_in_infite;
}