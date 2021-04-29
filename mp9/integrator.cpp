#include "integrator.h"
#include "particle.h"
#include "forcefield.h"
#include "vectors.h"

void EulerIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	Vec3f pos = particle->getPosition();
	Vec3f vel = particle->getVelocity();
	Vec3f new_pos = pos + vel * dt;
	Vec3f new_vel = vel + forcefield->getAcceleration(pos, particle->getMass(), t) * dt;
	particle->setPosition(new_pos);
	particle->setVelocity(new_vel);
	particle->increaseAge(dt);
}

void MidpointIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	Vec3f pos = particle->getPosition();
	Vec3f vel = particle->getVelocity();

	Vec3f mid_pos = pos + vel * dt*0.5;
	Vec3f mid_vel = vel + forcefield->getAcceleration(pos, particle->getMass(), t) * dt*0.5;

	Vec3f new_pos = pos + mid_vel * dt;
	Vec3f new_vel = vel + forcefield->getAcceleration(mid_pos, particle->getMass(), t + dt / 2) * dt;

	particle->setPosition(new_pos);
	particle->setVelocity(new_vel);
	particle->increaseAge(dt);
}

void RungeKuttaIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	Vec3f pos = particle->getPosition();
	Vec3f vel = particle->getVelocity();

	Vec3f a1 = forcefield->getAcceleration(pos, particle->getMass(), t);
	Vec3f v1 = vel;
	Vec3f a2 = forcefield->getAcceleration(pos+dt*v1*0.5, particle->getMass(), t+dt/2);
	Vec3f v2 = vel + a1 * dt *0.5;
	Vec3f a3 = forcefield->getAcceleration(pos + dt * v2 * 0.5, particle->getMass(), t + dt / 2);
	Vec3f v3 = vel + a2 * dt * 0.5;
	Vec3f a4 = forcefield->getAcceleration(pos + dt * v3, particle->getMass(), t + dt);
	Vec3f v4 = vel + a3 * dt;

	Vec3f new_pos = pos + dt / 6 * (v1 + v2 * 2 + v3 * 2 + v4);
	Vec3f new_vel = vel + dt / 6 * (a1 + a2 * 2 + a3 * 2 + a4);

	particle->setPosition(new_pos);
	particle->setVelocity(new_vel);
	particle->increaseAge(dt);
}