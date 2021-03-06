#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/texture.h"
#include "scene/ray.h"
#include <stack>

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth, std::stack<Material> materials);

	vec3f calculate_refraction(vec3f i, vec3f n, double eta1, double eta2);

	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );

	Scene* getScene() const { return scene; }
	bool loadScene( char* fn );
	bool sceneLoaded();

	int max_depth = 0;

	Texture* backgroundTexturePtr = nullptr;
	MaterialSceneObject* backgroundSceneObjPtr = nullptr;
	bool useBackground = false;
	void setupBackgroundObject();

private:
	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	Scene *scene;

	bool m_bSceneLoaded;
};

#endif // __RAYTRACER_H__
