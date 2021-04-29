#ifndef _GROUP_H_
#define _GROUP_H_
#include "Object3D.h"
#include "grid.h"

class Group : public Object3D{
private:
    Object3D** ptr_object;
    int size;
public:
    Group(int n) : Object3D() {
        ptr_object = new Object3D * [n]; size = 0; BBox = new BoundingBox(Vec3f(INFINITY, INFINITY,INFINITY),
            Vec3f(-INFINITY, -INFINITY, -INFINITY));
    }
    ~Group() { delete[] ptr_object; delete BBox; }
    bool intersect(const Ray &r, Hit &h, float tmin);
    int get_size(){return size;}
    void addObject(int index, Object3D *obj); 
    virtual void insertIntoGrid(Grid* g, Matrix* m);
    void paint();
};
#endif