#include <cmath>

#include "light.h"
#include "../helper.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.

	vec3f dir = getDirection(P).normalize();
	vec3f shadAtten;

	if (!scene->softShadow)
	{
		ray shadRay = ray(P, dir);
		isect i;
		shadAtten = getColor(P);

		while (scene->intersect(shadRay, i)) {
			// Non-transparent material
			if (i.getMaterial().kt.iszero())
				return vec3f(0.0, 0.0, 0.0);
			// Transparent material
			shadAtten = prod(shadAtten, i.getMaterial().kt);
			shadRay = ray(shadRay.at(i.t), dir);
		}
	}
	else
	{
		int n = 4;
		vector<vec3f> jitteredLights = jitteredSample3D(dir, n, 0.2);
		for (vec3f& l : jitteredLights) {
			ray shadRay = ray(P, l);
			isect i;
			vec3f newAtten = getColor(P);

			while (scene->intersect(shadRay, i)) {
				// Non-transparent material
				if (i.getMaterial().kt.iszero())
					newAtten = vec3f(0.0, 0.0, 0.0);
				// Transparent material
				newAtten = prod(newAtten, i.getMaterial().kt);
				shadRay = ray(shadRay.at(i.t), l);
			}

			shadAtten += newAtten;
		}
		shadAtten /= n * n * n;
	}

	return shadAtten;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	
	// Coefficients can be fetched from the Scene
	Scene* scene = getScene();

	// Find distance between the light source and P
	double dist = (position - P).length();
	// Find distance attenuation by formula
	double distAtten = minimum(1.0, 1.0 / (scene->distAttenConstCoeff + scene->distAttenLinearCoeff * dist + scene->distAttenQuadraticCoeff * dist * dist + 0.001));
	return distAtten;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.

	vec3f dir = getDirection(P).normalize();
	vec3f shadAtten;

	if (!scene->softShadow)
	{
		ray shadRay = ray(P, dir);
		isect i;
		shadAtten = getColor(P);

		while (scene->intersect(shadRay, i)) {
			// Non-transparent material
			if (i.getMaterial().kt.iszero())
				return vec3f(0.0, 0.0, 0.0);
			// Transparent material
			shadAtten = prod(shadAtten, i.getMaterial().kt);
			shadRay = ray(shadRay.at(i.t), dir);
		}
	}
	else
	{
		int n = 4;
		vector<vec3f> jitteredLights = jitteredSample3D(dir, n, 0.2);
		for (vec3f& l : jitteredLights) {
			ray shadRay = ray(P, l);
			isect i;
			vec3f newAtten = getColor(P);

			while (scene->intersect(shadRay, i)) {
				// Non-transparent material
				if (i.getMaterial().kt.iszero())
					newAtten = vec3f(0.0, 0.0, 0.0);
				// Transparent material
				newAtten = prod(newAtten, i.getMaterial().kt);
				shadRay = ray(shadRay.at(i.t), l);
			}

			shadAtten += newAtten;
		}
		shadAtten /= n * n * n;
	}

	return shadAtten;
}


vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
	vec3f L = getDirection(P).normalize();
	double angleBetween = acos((L.dot(direction)) / (L.length() * direction.length()));

	if (angleBetween < angle)
	{
		return color;
	}

	return vec3f(0.1, 0.1, 0.1);
}

double SpotLight::distanceAttenuation(const vec3f& P) const
{
	return 1.0;
}

vec3f SpotLight::getColor(const vec3f& P) const
{
	return color;
}

vec3f SpotLight::getDirection(const vec3f& P) const
{
	return (P - position).normalize();
}

