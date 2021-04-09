#include "ray.h"
#include "material.h"
#include "light.h"

int Material::current_count = 0;

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade(Scene* scene, const ray& r, const isect& i) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
	// You will need to call both distanceAttenuation() and shadowAttenuation()
	// somewhere in your code in order to compute shadows and light falloff.

	// Add emissive and ambient light components
	vec3f I = ke + prod(ka, scene->getAmbient());

	// Find intersection point
	vec3f P = r.at(i.t);

	// Add diffuse + specular reflection
	for (list<Light*>::const_iterator iterator = scene->beginLights(); iterator != scene->endLights(); iterator++) {
		// Find the light direction
		vec3f L = (*iterator)->getDirection(P);
		L = L.normalize();

		// Find the direction of reflection
		vec3f R = (2 * (i.N * L) * i.N) - L;
		R = R.normalize();

		// Find the view direction
		vec3f V = -r.getDirection();
		V = V.normalize();

		// Find diffuse and specular components
		vec3f diffuseRef = kd * maximum(i.N.dot(L), 0.0);
		vec3f specularRef = ks * pow(maximum(V.dot(R), 0.0), shininess * 128);

		vec3f att = (*iterator)->distanceAttenuation(P) * (*iterator)->shadowAttenuation(P);

		I += prod(att, diffuseRef + specularRef);
	}

	I.clamp();
	return I;
}

