#ifndef __TORUS_H__
#define __TORUS_H__

#include "../scene/scene.h"

class Torus
	: public MaterialSceneObject
{
public:
	Torus(Scene* scene, Material* mat)
		: MaterialSceneObject(scene, mat)
	{
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }


	virtual BoundingBox ComputeLocalBoundingBox()
	{
		BoundingBox localBounds;
		localBounds.min = vec3f(-sweptRadius - tubeRadius, -sweptRadius - tubeRadius, -tubeRadius);
		localBounds.max = vec3f(sweptRadius + tubeRadius, sweptRadius + tubeRadius, tubeRadius);
		return localBounds;
	}

	double sweptRadius = 1;
	double tubeRadius = 0.2;
};

#endif // __TORUS_H__
