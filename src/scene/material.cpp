#include "ray.h"
#include "material.h"
#include "light.h"
#include <utility>

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

	auto emissionColor = ke;
	const auto* obj = dynamic_cast<const MaterialSceneObject*>(i.obj);
	if (obj != nullptr)
	{
		std::pair<double, double> p = obj->getUV(r, i);
		if (emissionTexturePtr != nullptr)
		{
			emissionColor = emissionTexturePtr->getColorOfSquare(p.first, p.second);
		}
	}

	// Add emissive and ambient light components
	vec3f I = ke;
	I += prod(prod(ka, scene->getAmbient()), vec3f(1.0, 1.0, 1.0) - kt); // multiplied by 1-kt for refraction

	// Texture mapping
	if (emissionTexturePtr != nullptr)
	{
		std::pair<double, double> p = obj->getUV(r, i);
		emissionColor = emissionTexturePtr->getColorOfSquare(p.first, p.second);
	}

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
		vec3f diffuseRef = prod(kd * maximum(i.N.dot(L), 0.0), vec3f(1.0, 1.0, 1.0) - kt); // multiplied by 1 - kt for refraction
		vec3f specularRef = ks* pow(maximum(V.dot(R), 0.0), shininess * 128);

		if ((*iterator)->type == LightType::SPOT) {
			specularRef = vec3f(0.1, 0.1, 0.1);
		}

		vec3f att = (*iterator)->distanceAttenuation(P) * (*iterator)->shadowAttenuation(P);

		I += prod(att, diffuseRef + specularRef);
	}

	I += emissionColor; // Adding background color, not sure this is the right place though, seems to work
	I.clamp();
	return I;
}

