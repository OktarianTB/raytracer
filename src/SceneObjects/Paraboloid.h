#ifndef __PARABOLOID_H__
#define __PARABOLOID_H__

#include "../scene/scene.h"

class Paraboloid
	: public MaterialSceneObject
{
public:
	Paraboloid(Scene* scene, Material* mat, int type_x)
		: MaterialSceneObject(scene, mat), type_x(type_x)
	{
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return false; }

	virtual BoundingBox ComputeLocalBoundingBox()
	{
		BoundingBox localbounds;
		localbounds.min = vec3f(-1.0f, -1.0f, -1.0f);
		localbounds.max = vec3f(1.0f, 1.0f, 1.0f);
		return localbounds;
	}

	bool type_x;
};

#endif // __PARABOLOID_H__
#pragma once
