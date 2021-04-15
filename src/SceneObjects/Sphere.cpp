#include <cmath>

#include "Sphere.h"

bool Sphere::intersectLocal( const ray& r, isect& i ) const
{
	vec3f v = -r.getPosition();
	double b = v.dot(r.getDirection());
	double discriminant = b*b - v.dot(v) + 1;

	if( discriminant < 0.0 ) {
		return false;
	}

	discriminant = sqrt( discriminant );
	double t2 = b + discriminant;

	if( t2 <= RAY_EPSILON ) {
		return false;
	}

	i.obj = this;

	double t1 = b - discriminant;

	if( t1 > RAY_EPSILON ) {
		i.t = t1;
		i.N = r.at( t1 ).normalize();
	} else {
		i.t = t2;
		i.N = r.at( t2 ).normalize();
	}

	return true;
}

void Sphere::intersect(const ray& r, double& tMin, BoundingBox bb) const
{
	vec3f R0 = r.getPosition();
	vec3f Rd = r.getDirection();

	tMin = -1.0e308;
	double ttemp;

	for (int currentaxis = 0; currentaxis < 3; currentaxis++)
	{
		double vd = Rd[currentaxis];

		// if the ray is parallel to the face's plane (=0.0)
		if (vd == 0.0)
			continue;

		double v1 = bb.min[currentaxis] - R0[currentaxis];
		double v2 = bb.max[currentaxis] - R0[currentaxis];

		// two slab intersections
		double t1 = v1 / vd;
		double t2 = v2 / vd;

		if (t1 > t2) { // swap t1 & t2
			ttemp = t1;
			t1 = t2;
			t2 = ttemp;
		}

		if (t1 > tMin)
			tMin = t1;
	}
}

std::pair<double, double> Sphere::getUV(const ray& r, const isect& i) const
{
	vec3f pos = transform->globalToLocalCoords(r.getPosition());
	vec3f dir = transform->globalToLocalCoords(r.getPosition() + r.getDirection()) - pos;
	dir = dir.normalize();

	ray localRay(pos, dir);

	BoundingBox bb;
	bb.min = vec3f(-1.0f, -1.0f, -1.0f);
	bb.max = vec3f(1.0f, 1.0f, 1.0f);

	double tMin = -999;
	intersect(localRay, tMin, bb);
	vec3f posOnBox = localRay.at(tMin);

	vec3f d = localRay.at(tMin).normalize();

	double u = 0.5 + (atan2(d[0], d[1]) / (2 * 3.14));
	double v = 0.5 - asin(d[2]) / 3.13;

	return std::pair<double, double>(u, v);
}

