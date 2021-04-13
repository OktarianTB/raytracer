#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

enum struct LightType { DIRECTIONAL, POINT, SPOT };

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

	LightType	type;
protected:
	Light( Scene *scene, const vec3f& col, LightType type )
		: SceneElement( scene ), color( col ), type(type) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color, LightType::DIRECTIONAL ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color/*, double coeff0 = 1.0, double coeff1 = 0.0, double coeff2 = 0.0 */)
		: Light( scene, color, LightType::POINT ), position( pos ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f position;
};

class SpotLight
	: public Light
{
public:
	SpotLight(Scene* scene, const vec3f& color, const vec3f& position, const vec3f& direction, double angle, double blend) :
		Light(scene, color, LightType::SPOT),
		position(position),
		direction(direction),
		angle(angle),
		blend(blend) {}

	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation(const vec3f& P) const;
	virtual vec3f getColor(const vec3f& P) const;
	virtual vec3f getDirection(const vec3f& P) const;

protected:
	vec3f position;
	vec3f direction;
	double angle;
	double blend;
};


#endif // __LIGHT_H__
