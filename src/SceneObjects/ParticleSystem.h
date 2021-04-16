#pragma once
#include <vector>
#include "../scene/scene.h"

extern double drand(double low, double high);
extern int irand(int low, int high);

typedef struct Particle
{
	vec3f position;
	vec3f color;
};

class ParticleSystem : public MaterialSceneObject
{
public:

	ParticleSystem(Scene* scene, Material* mat);

	void init();
	Particle generateRandomParticle();

	bool intersectLocal(const ray& r, isect& i) const override;
	bool intersectParticle(vec3f& particle_pos, vec3f& ray_pos, vec3f& ray_dir) const;
	
	std::vector<Particle> particles;

	vec3f origin, dir, acceleration, initColor, u, v;
	int framesCnt, particlesCnt;
	double speedMin, speedMax;
};

