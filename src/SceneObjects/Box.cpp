#include <cmath>
#include <assert.h>
#include <iostream>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// Bottom corner is (-0.5, -0.5, -0.5) and top corner is (0.5, 0.5, 0.5)
    BoundingBox bb;
    bb.max = vec3f(0.5, 0.5, 0.5);
    bb.min = vec3f(-0.5, -0.5, -0.5);

    double tmin, tmax;
    if (!bb.intersect(r, tmin, tmax))
    {
        return false;
    }

    if (tmin < RAY_EPSILON || tmax < RAY_EPSILON || abs(tmax - tmin) < RAY_EPSILON)
    {
        return false;
    }

    i.t = tmin;
    i.obj = this;
    i.N = r.at(tmin).normalize();

	return true;
}
