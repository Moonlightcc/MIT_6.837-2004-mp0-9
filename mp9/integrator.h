#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "vectors.h"
#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator {
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
	virtual Vec3f getColor() override { return Vec3f(1, 0, 0); }
};

class MidpointIntegrator : public Integrator {
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
	virtual Vec3f getColor() override { return Vec3f(0, 1, 0); }
};

class RungeKuttaIntegrator : public Integrator {
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
	virtual Vec3f getColor() override { return Vec3f(0, 0, 1); }
};
#endif // !_INTEGRATOR_H_

