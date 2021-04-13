#ifndef __HELPER_H__
#define __HELPER_H__

#include <vector>
#include <cstdlib>
#include <utility>

#include "vecmath/vecmath.h"
#include "scene/ray.h"

inline double random_double(); // returns random in [0, 1)
inline double random_double(double min, double max); // returns random in [min, max)

vector<vec3f> jitteredSample3D(vec3f d, int n, double range); // returns jittered sample around 3D Point 

#endif // __HELPER_H__