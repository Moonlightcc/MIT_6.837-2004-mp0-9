#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,const Vec3f& lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;
protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

class PhongMaterial : public Material {
private:
	Vec3f specularCol;
	float exp;
public:
	PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent):Material(diffuseColor) 
	{
		specularCol = specularColor;
		exp = exponent;
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const;
	void glSetMaterial(void) const;
	Vec3f getSpecularColor() const { return specularCol; }
};

// ====================================================================
// ====================================================================

#endif
