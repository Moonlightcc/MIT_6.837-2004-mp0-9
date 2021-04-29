#include "generator.h"
#include "gl/GLUT.h"
int HoseGenerator::numNewParticles(float current_time, float dt) const
{
	return int(dt * desired_num_particles / lifespan);
}

Particle* HoseGenerator::Generate(float current_time, int i)
{
	Vec3f position = pos + pos_ran * ran->randomVector();
	Vec3f velocity = vel + vel_ran * ran->randomVector();
	Vec3f col = color + color_randomness * ran->randomVector();
	Vec3f dead_col = dead_color + color_randomness * ran->randomVector();
	float m = mass + mass_randomness * ran->next();
	float f = lifespan + lifespan_randomness * ran->next();
	Particle* part = new Particle(position, velocity, col, dead_col, m, f);
	return part;
}

int RingGenerator::numNewParticles(float current_time, float dt) const
{
	if (current_time < radius)
		return int(current_time / radius * dt * desired_num_particles / lifespan);
	else
		return int(dt * desired_num_particles / lifespan);
}

Particle* RingGenerator::Generate(float current_time, int i)
{
	float pos = min(current_time, radius);
	Vec3f position = Vec3f(pos*sin(i) +pos_ran*ran->next(), -4,pos*cos(i)+pos_ran*ran->next());
	Vec3f velocity = vel + vel_ran * ran->randomVector();
	Vec3f col = color + color_randomness * ran->randomVector();
	Vec3f dead_col = dead_color + color_randomness * ran->randomVector();
	float m = mass + mass_randomness * ran->next();
	float f = lifespan + lifespan_randomness * ran->next();
	Particle* part = new Particle(position, velocity, col, dead_col, m, f);
	return part;
}

void RingGenerator::Paint() const {
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(5, -4, 5);
	glVertex3f(5, -4, -5);
	glVertex3f(-5, -4, -5);
	glVertex3f(-5, -4, 5);
	glEnd();
}