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
  Material(const Vec3f &d_color, const Vec3f &r_color, const Vec3f &t_color, float IOR) 
  { diffuseColor = d_color; reflectiveColor = r_color; transparentColor = t_color; indexOfRefraction = IOR; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,const Vec3f& lightColor,float light_dis) const = 0;
  virtual void glSetMaterial(void) const = 0;
  virtual Vec3f getReflectiveColor() const { return reflectiveColor; }
  virtual Vec3f getTransparentColor() const { return transparentColor; }
  virtual float getIndexOfRefraction() const { return indexOfRefraction; }

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
};

class PhongMaterial : public Material {
private:
	Vec3f specularCol;
	float exp;
public:
	PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent, const Vec3f& reflectiveColor,
		const Vec3f& transparentColor, float indexOfRefraction) :Material(diffuseColor, reflectiveColor, transparentColor, indexOfRefraction)
	{
		specularCol = specularColor;
		exp = exponent;
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor, float light_dis) const;
	void glSetMaterial(void) const;
	Vec3f getSpecularColor() const { return specularCol; }
};

// ====================================================================
// ====================================================================

#endif
