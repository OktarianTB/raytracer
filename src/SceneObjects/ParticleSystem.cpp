#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Scene* scene, Material* mat)
	: MaterialSceneObject(scene, mat)
{
	origin = { 0,-1.4,0 };
	dir = { 0,1,0 };
	u = { 1,0,0 };
	v = { 0,0,1 };
	acceleration = { 0,-0.03,0 };
	speedMin = 0.01;
	speedMax = 0.1;
	particlesCnt = 1000;
	framesCnt = 70;

	// Initiate particle system
	init();
}

Particle ParticleSystem::generateRandomParticle()
{
	int t = irand(0, framesCnt);
	double factor = drand(speedMin, speedMax);

	vec3f position = origin + drand(-0.01, 0.01) * factor * u + drand(-0.01, 0.01) * factor * v;
	vec3f velocity = (dir + drand(-0.4, 0.4) * u + drand(-0.6, 0.6) * v) * factor;
	position -= velocity * t + acceleration * t * t / 2;

	vec3f color;
	switch (irand(0, 4))
	{
	case 0:
		color = { 0.45, 0.72, 1 };
		break;
	case 1:
		color = { 0.04, 0.52, 0.89 };
		break;
	case 2:
		color = { 0.63, 0.61, 1 };
		break;
	case 3:
		color = { 0.42, 0.36, 0.91 };
		break;
	}

	color *= drand(0.6, 1);
	return { position, color };
}

void ParticleSystem::init()
{
	particles.clear();

	for (int i = 0; i < particlesCnt; i++)
	{
		Particle particle = generateRandomParticle();
		particles.push_back(particle);
	}
}

bool ParticleSystem::intersectLocal(const ray& r, isect& i) const
{
	vec3f color(0, 0, 0);
	bool foundIntersection = false;
	double minT = 999999;
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	for (auto&& particle : particles)
	{
		vec3f q = particle.position;
		if (intersectParticle(q, p, d)) {
			foundIntersection = true;
			double t = (q - p).length();
			minT = std::min(minT, t);

			for (int j = 0; j < 3; j++) {
				color[j] = std::max(color[j], particle.color[j]);
			}
		}
	}

	if (foundIntersection)
	{
		Material* m = new Material;
		m->ke = color;
		i.t = minT;
		i.N = -d;
		i.material = m;
	}

	return foundIntersection;
}

bool ParticleSystem::intersectParticle(vec3f& particle_pos, vec3f& ray_pos, vec3f& ray_dir) const
{
	vec3f pq = (particle_pos - ray_pos).normalize();
	return (abs(abs(pq * ray_dir) - 1) < RAY_EPSILON);
}
