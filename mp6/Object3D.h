#ifndef  _OBJECT_3D_
#define  _OBJECT_3D_

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"
#include "boundingbox.h"
#include "MarchingInfo.h"
#include "object3dvector.h"

class Grid;

class Object3D {
protected:
    Material* mat;
    BoundingBox* BBox;
	bool grid_detect;
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;
	Object3D() { grid_detect = false; }
	Object3D(Material* material) { mat = material; grid_detect = false; }
    Material* get_material(){return mat;}
    BoundingBox* getBoundingBox() { return BBox; }
	virtual void insertIntoGrid(Grid* g, Matrix* m);
	void SetDetect() { grid_detect = true; }
	bool isDetect() { return grid_detect; }
	void InitDetect() { grid_detect = false; }
    ~Object3D(){}
};


class Grid : public Object3D {
private:
	Object3DVector* cells;
	Object3DVector* Infinite_Primitives;
	int x, y, z;
public:
	Grid(BoundingBox* bb, int nx, int ny, int nz) {
		BBox = bb; cells = new Object3DVector[nx * ny * nz]; x = nx; y = ny; z = nz; Infinite_Primitives = new Object3DVector();
	}
	~Grid() { delete[] cells; delete Infinite_Primitives; }
	int getx() { return x; }
	int gety() { return y; }
	int getz() { return z; }
	void set(int dx, int dy, int dz, Object3D* val) { cells[dx * y * z + dy * z + dz].addObject(val); }
	void setInfinitePri(Object3D* inf_pri) { Infinite_Primitives->addObject(inf_pri); }
	Object3D* get(int dx, int dy, int dz, int index) { return cells[dx * y * z + dy * z + dz].getObject(index); }
	Object3D* getInfinitePri(int index) { return Infinite_Primitives->getObject(index); }
	int getNumObject(int dx, int dy, int dz) { return cells[dx * y * z + dy * z + dz].getNumObjects(); }
	int getNumInfiniteObject() { return Infinite_Primitives->getNumObjects(); }
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint();
	void initializeRayMarch(MarchingInfo& mi, const Ray& r, float tmin);
	bool ShodowJudge(const Ray& r, Hit& h, float tmin);
	void InitAllObjDetect()
	{
		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				for (int k = 0; k < z; k++)
					cells[i * y * z + j * z + k].ClearDetect();
	}
	void showInfo()
	{
		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				for (int k = 0; k < z; k++)
					std::cout << "i: " << i << " ,j: " << j << " ,k: " << k << " ,num: " << getNumObject(i, j, k) << std::endl;
	}
};
#endif

