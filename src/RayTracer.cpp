// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "SceneObjects/Square.h"
#include "helper.h"
#include <cmath>

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );

	auto materials = std::stack<Material>();
	materials.push(Material::air());

	return traceRay(scene, r, vec3f(1.0,1.0,1.0), 0, materials).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth, std::stack<Material> materials)
{
	if (depth > max_depth) {
		return vec3f(0, 0, 0);
	}

	isect i;

	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		const Material& m = i.getMaterial();
		vec3f phong = m.shade(scene, r, i);

		if (scene->threshold > phong.length())
		{
			return phong;
		}

		// Intersection values
		vec3f normal = i.N.normalize();
		vec3f direction = r.getDirection().normalize();
		vec3f position = r.at(i.t);

		if (materials.top().material_id == m.material_id)
		{
			normal = -normal;
		}

		// Calculate reflection
		vec3f reflection_color(0, 0, 0);

		if (m.kr.length() > 0) {
			vec3f reflection_direction = direction - 2 * direction.dot(normal) * normal;
			if (!scene->glossyReflection) {
				ray reflection_ray(position, reflection_direction.normalize());
				reflection_color = traceRay(scene, reflection_ray, thresh, depth + 1, materials);
				reflection_color = prod(reflection_color, m.kr); // multiply color by the reflective value
			}
			else {
				int n = 3;
				vector<vec3f> jitteredReflections = jitteredSample3D(reflection_direction, n, 0.2);

				for (vec3f& j : jitteredReflections) {
					ray reflection_ray(position, j);
					int new_depth = max(depth + 1, max_depth - 1);
					vec3f new_color = traceRay(scene, reflection_ray, thresh, new_depth, materials);
					new_color = prod(new_color, m.kr);

					reflection_color += new_color;
				}
				reflection_color /= n * n * n;
			}
		}

		// Calculate refraction
		vec3f refraction_color(0, 0, 0);

		if (m.kt.length() > 0) // Check the material has some transparency
		{
			double sourceRefIndex = materials.top().index;
			double targetRefIndex;

			if (materials.top().material_id == m.material_id)
			{
				materials.pop();
				targetRefIndex = materials.top().index;
			}
			else
			{
				materials.push(m);
				targetRefIndex = m.index;
			}

			vec3f T = calculate_refraction(direction, normal, sourceRefIndex, targetRefIndex);
			if (!T.iszero()) {
				ray refraction_ray(position, T.normalize());
				refraction_color = traceRay(scene, refraction_ray, thresh, depth + 1, materials);
				refraction_color = prod(refraction_color, m.kt); // multiply color by the transmissive value
			}
		}

		// Return contributions of phong shading + reflection + refraction (todo)
		return phong + reflection_color + refraction_color;
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

// i: incidence vector, n: normal, sourceRefIndex: source refraction index, targetRefIndex: target refraction index
vec3f RayTracer::calculate_refraction(vec3f i, vec3f n, double eta1, double eta2)
{
	// I don't know if this is correct!!!
	/*double theta1 = i.dot(n);
	double theta2 = asin((eta1 * sin(theta1)) / eta2);
	vec3f T = (eta1 / eta2) * (i + n * cos(theta1)) - n * cos(theta2);
	return T;*/

	vec3f I = -i; // incident ray
	double eta = eta1 / eta2;
	double cosi = n.dot(I);
	double tir = 1 - eta * eta * (1 - cosi * cosi); // Eq 16.32 Foley

	// Taking care of total internal reflection which occurs when angle theta_t is from 90 to 180 degrees, i.e. cos(theta_i) < 0
	if (tir < 0)
		return vec3f(0.0, 0.0, 0.0); // no refraction, only reflection

	// Eq 16.33 Foley
	vec3f T = (eta * cosi - sqrt(tir)) * n - eta * I;
	return T;
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];

	// Some info in the console
	cout << boolalpha << "Use background: " << useBackground << endl;
	cout << boolalpha << "Is background set: " << (backgroundTexturePtr != nullptr) << endl;

	// Background
	if (useBackground) {
		setupBackgroundObject();

		auto* backgroundMaterialPtr = new Material();
		backgroundMaterialPtr->emissionTexturePtr = backgroundTexturePtr;
		backgroundSceneObjPtr->setMaterial(backgroundMaterialPtr);

		const auto backgroundIt = std::find(scene->objects.begin(), scene->objects.end(), backgroundSceneObjPtr);
		if (useBackground && backgroundIt == scene->objects.end())
		{
			// Add background object to scene.
			scene->add(backgroundSceneObjPtr);
		}
	}
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::setupBackgroundObject()
{
	static const vec3f ONES = { 1.0, 1.0, 1.0 };
	auto* backgroundMaterialPtr = new Material();
	backgroundMaterialPtr->ke = ONES;
	backgroundMaterialPtr->kd = ONES;
	backgroundMaterialPtr->ks = ONES;
	backgroundMaterialPtr->shininess = 0.6;
	backgroundSceneObjPtr = new Square(scene, backgroundMaterialPtr);
	auto* transformPtr = &scene->transformRoot;
	auto& m = scene->getCamera()->m;
	const mat4f rot = {
		vec4f{m[0][0], m[0][1], m[0][2], 0.0},
		vec4f{m[1][0], m[1][1], m[1][2], 0.0},
		vec4f{m[2][0], m[2][1], m[2][2], 0.0},
		vec4f{0.0, 0.0, 0.0, 1.0}
	};
	const auto& eye = scene->getCamera()->eye;
	static const auto BG_DISTANCE = 20.0;
	backgroundSceneObjPtr->setTransform(
		transformPtr->createChild(mat4f::translate(eye * 0.1))
		->createChild(rot)
		->createChild(mat4f::translate(vec3f{ 0.0, 0.0, -1.0 } *BG_DISTANCE))
		->createChild(mat4f::scale(ONES * pow(BG_DISTANCE * 0.3, 2)))
	);
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}
