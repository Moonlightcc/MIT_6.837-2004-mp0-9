#ifndef _HIT_H
#define _HIT_H

#include "vectors.h"
#include "ray.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { material = NULL; inside = false; }
  Hit(float _t, Material *m, Vec3f n) { 
    t = _t; material = m; normal = n; inside = false;}
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal; 
    intersectionPoint = h.intersectionPoint;
    inside = h.inside; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getNormal() const { return normal; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  bool isInside() const {return inside;}
  // MODIFIER
  void set(float _t, Material *m, Vec3f n, const Ray &ray,bool in) {
    t = _t; material = m; normal = n; 
    intersectionPoint = ray.pointAtParameter(t); inside = in; }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f normal;
  Vec3f intersectionPoint;
  bool inside;
};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <" <<h.getT()<<", "<<h.getNormal()<<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif
