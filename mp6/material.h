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
	Material() { diffuseColor = Vec3f(0, 0, 0); reflectiveColor = Vec3f(0, 0, 0); transparentColor = Vec3f(0, 0, 0); indexOfRefraction = 1.0; }
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

class Checkerboard : public Material {
private:
	Material* material1;
	Material* material2;
	Matrix* mat;
public:
	Checkerboard(Matrix* m, Material* mat1, Material* mat2):Material() { material1 = mat1; material2 = mat2; mat = m; }
	virtual void glSetMaterial(void) const { material1->glSetMaterial(); }
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor, float light_dis) const;
};

class Noise : public Material {
private:
	Material* material1;
	Material* material2;
	Matrix* mat;
	int oct;
public:
	Noise(Matrix* m, Material* mat1, Material* mat2, int octaves) :Material() { material1 = mat1; material2 = mat2; mat = m; oct = octaves; }
	virtual void glSetMaterial(void) const { material1->glSetMaterial(); }
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor, float light_dis) const;
};

class Marble : public Material {
private:
	Material* material1;
	Material* material2;
	Matrix* mat;
	int oct;
	float fre;
	float amp;
public:
	Marble(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude):Material()
	{material1 = mat1; material2 = mat2; mat = m; oct = octaves; fre = frequency; amp = amplitude;}
	virtual void glSetMaterial(void) const { material1->glSetMaterial(); }
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor, float light_dis) const;
};

class Wood : public Material {
private:
	Material* material1;
	Material* material2;
	Matrix* mat;
	int oct;
	float fre;
	float amp;
public:
	Wood(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude) :Material()
	{
		material1 = mat1; material2 = mat2; mat = m; oct = octaves; fre = frequency; amp = amplitude;
	}
	virtual void glSetMaterial(void) const { material1->glSetMaterial(); }
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor, float light_dis) const;
};
#endif
