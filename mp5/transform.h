#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Object3D.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"
#include "matrix.h"

class Transform : public Object3D{
private:
    Object3D* ptr_obj;
    Matrix ma;
public:
    Transform(Matrix &m, Object3D *o):Object3D(){ptr_obj = o; ma = m; SetBoundingBox();}
    ~Transform() { delete BBox; }
    bool intersect(const Ray &r, Hit &h, float tmin);
    void SetBoundingBox();
    void paint();
    void insertIntoGrid(Grid* g, Matrix* m);
};


#endif