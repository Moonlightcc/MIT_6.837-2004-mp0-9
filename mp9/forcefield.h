#ifndef  _FORCEFIELD_H_
#define  _FORCEFIELD_H_

#include "vectors.h"

class ForceField {
public:
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const = 0;
};

class GravityForceField : public ForceField {
private:
	Vec3f g;
public:
	GravityForceField() { g = Vec3f(0,0,-9.8); }
	GravityForceField(Vec3f g_val) { g = g_val; }
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override { return g; }
};

class ConstantForceField : public ForceField {
private:
	Vec3f f;
public:
	ConstantForceField() { f = Vec3f(0, 0, 0); }
	ConstantForceField(Vec3f force) { f = force; }
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override { return f*(1.0/mass); }
};

class RadialForceField : public ForceField {
private:
	float mag;
public:
	RadialForceField() { mag = 0; }
	RadialForceField(float magnitude) { mag = magnitude; }
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override { return -mag * position * (1.0 / mass); }
};

class VerticalForceField : public ForceField {
private:
	float mag;
public:
	VerticalForceField() { mag = 0; }
	VerticalForceField(float magnitude) { mag = magnitude; }
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override { return -mag * Vec3f(0,position[1],0) * (1.0 / mass); }
};

class WindForceField : public ForceField {
private:
	float mag;
public:
	WindForceField() { mag = 0; }
	WindForceField(float magnitude) { mag = magnitude; }
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override {return mag * Vec3f(sin(15 * t), cos(15 * t), 0) * (1.0 / mass);}
};

#endif // ! _FORCEFIELD_H_

