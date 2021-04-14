#include <cmath>
#include <vector>
#include "quartic.h"
#include "Paraboloid.h"

bool Paraboloid::intersectLocal(const ray& r, isect& i) const
{
	vec3f origin = r.getPosition();
	vec3f direction = r.getDirection();

	double ox = origin[0];
	double oy = origin[1];
	double oz = origin[2];

	double dx = direction[0];
	double dy = direction[1];
	double dz = direction[2];

	double A = -dy * dy + dx * dx;
	double B = -2 * oy * dy + 2 * ox * dx - dz;
	double C = -oy * oy + ox * ox - oz;

	double discriminant = B * B - 4 * A * C;

	if (discriminant < 0.0)
	{
		return false;
	}

	discriminant = sqrt(discriminant);

	double t1 = (-B + sqrt(discriminant)) / (2 * A);

	if (t1 <= RAY_EPSILON) {
		return false;
	}

	double t2 = (-B - sqrt(discriminant)) / (2 * A);

	if (t2 > RAY_EPSILON) {
		i.t = t2;
		i.N = r.at(t2).normalize();
	}
	else {
		i.t = t1;
		i.N = r.at(t1).normalize();
	}

	i.obj = this;

	return true;
}