#include "texture.h"
#include "../fileio/bitmap.h"
#include <FL/fl_ask.h>


Texture::Texture(char* filename)
{
	imagePtr = readBMP(filename, imageWidth, imageHeight);

	if (imageWidth != imageHeight)
	{
		fl_alert("Please select a square background to avoid unexpected behavior!");
	}
	
}

vec3f Texture::getColorOfSquare(const double u, const double v) const
{
	const auto pixel = imagePtr + (int(v * imageHeight) * imageWidth + int(u * imageWidth)) * 3;
	if (u > 0.0 && v > 0.0)
		return { pixel[0] / 255.0, pixel[1] / 255.0, pixel[2] / 255.0 };
	return { 1.0, 1.0, 1.0 };
}