#include <cmath>
#include "Square.h"


bool Square::intersectLocal( const ray& r, isect& i ) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if( d[2] == 0.0 ) {
		return false;
	}

	double t = -p[2]/d[2];

	if( t <= RAY_EPSILON ) {
		return false;
	}

	vec3f P = r.at( t );

	if( P[0] < -0.5 || P[0] > 0.5 ) {	
		return false;
	}

	if( P[1] < -0.5 || P[1] > 0.5 ) {	
		return false;
	}

	i.obj = this;
	i.t = t;
	if( d[2] > 0.0 ) {
		i.N = vec3f( 0.0, 0.0, -1.0 );
	} else {
		i.N = vec3f( 0.0, 0.0, 1.0 );
	}

	return true;
}

double clamp(double v, double min, double max)
{
	if (v < min)
		return min;
	if (v > max)
		return max;

	return v;
}

std::pair<double, double> Square::getUV(const ray& r, const isect& i) const
{
	vec3f pos = transform->globalToLocalCoords(r.getPosition());
	vec3f dir = transform->globalToLocalCoords(r.getPosition() + r.getDirection()) - pos;
	double length = dir.length();
	dir /= length;

	ray localRay(pos, dir);

	auto bigP = getP(localRay, i);

	if (bigP[0] < -0.5 || bigP[0] > 0.5)
	{
		return {};
	}

	if (bigP[1] < -0.5 || bigP[1] > 0.5)
	{
		return {};
	}

	return { clamp(bigP[0] + 0.5, 0.0, 1.0), clamp(bigP[1] + 0.5, 0.0, 1.0) };
}

vec3f Square::getP(const ray& r, const isect& i)
{
	auto p = r.getPosition();
	auto d = r.getDirection();
	const auto t = -p[2] / d[2];
	if (t <= RAY_EPSILON)
	{
		return {};
	}
	return r.at(t);
}