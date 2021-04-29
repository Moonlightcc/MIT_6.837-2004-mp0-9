#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "particle.h"
#include "random.h"

class Generator {
protected:
	Vec3f color;
	Vec3f dead_color;
	float color_randomness;
	float lifespan;
	float lifespan_randomness;
	int desired_num_particles;
	float mass;
	float mass_randomness;

	Random* ran;

public:
	Generator() { ran = new Random(777); }
	~Generator() { delete ran; }
	// initialization
	virtual void SetColors(Vec3f c, Vec3f dc, float color_ran) { color = c; dead_color = dc; color_randomness = color_ran; }
	virtual void SetLifespan(float ls, float ls_ran, int desired_num_particles_) 
	{ lifespan = ls; lifespan_randomness = ls_ran; desired_num_particles = desired_num_particles_; }
	virtual void SetMass(float m, float mass_ran) { mass = m; mass_randomness = mass_ran; }

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const = 0;
	virtual Particle* Generate(float current_time, int i) = 0;

	// for the gui
	virtual void Paint() const {/*no gui used*/}
	virtual void Restart() { delete ran; ran = new Random(777);}

};

class HoseGenerator : public Generator {
private:
	Vec3f pos;
	float pos_ran;
	Vec3f vel;
	float vel_ran;
public:
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness):Generator()
	{pos = position; pos_ran = position_randomness; vel = velocity; vel_ran = velocity_randomness;}
	virtual int numNewParticles(float current_time, float dt) const override;
	virtual Particle* Generate(float current_time, int i) override;
};

class RingGenerator : public Generator {
private:
	float pos_ran;
	Vec3f vel;
	float vel_ran;
	float radius;
public:
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness):Generator()
	{pos_ran = position_randomness; vel = velocity; vel_ran = velocity_randomness; radius = 5;}
	virtual int numNewParticles(float current_time, float dt) const override;
	virtual Particle* Generate(float current_time, int i) override;
	virtual void Paint() const override;
};

#endif // !_GENERATOR_H_
