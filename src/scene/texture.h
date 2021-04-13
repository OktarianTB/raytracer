#pragma once
#include "../vecmath/vecmath.h"

class Texture
{
public:
	Texture(char* filename);

	unsigned char* imagePtr = nullptr;
	int imageWidth = 0;
	int imageHeight = 0;

	vec3f getColorOfSquare(const double u, const double v) const;
};