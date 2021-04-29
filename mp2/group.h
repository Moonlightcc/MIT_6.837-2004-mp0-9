#ifndef _GROUP_H_
#define _GROUP_H_
#include "Object3D.h"
class Group : public Object3D{
private:
    Object3D** ptr_object;
    int size;
public:
    Group(int n): Object3D() {ptr_object = new Object3D* [n];size=0;}
    ~Group(){delete[] ptr_object;}
    bool intersect(const Ray &r, Hit &h, float tmin);
    int get_size(){return size;}
    void addObject(int index, Object3D *obj); 
};
#endif