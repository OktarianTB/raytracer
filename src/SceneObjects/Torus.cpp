#include <cmath>
#include <vector>
#include "quartic.h"
#include "Torus.h"

bool Torus::intersectLocal(const ray& r, isect& i) const
{
	vec3f origin = r.getPosition();
	vec3f direction = r.getDirection();

	double ox = origin[0];
	double oy = origin[1];
	double oz = origin[2];

	double dx = direction[0];
	double dy = direction[1];
	double dz = direction[2];

	double sweptRadius_sqrd = sweptRadius * sweptRadius;
	double tubeRadius_sqrd = tubeRadius * tubeRadius;

	double sum_d_sqrd = direction.length_squared();
	double f = origin.dot(direction);
	double l = origin.length_squared() + (sweptRadius_sqrd - tubeRadius_sqrd);
	double m = 4 * sweptRadius_sqrd * (ox * ox + oy * oy);
	double k = 2 * origin.dot(direction);
	double g = 4 * sweptRadius_sqrd * (dx * dx + dy * dy);
	double h = 8 * sweptRadius_sqrd * (ox * dx + oy * dy);

	double c4 = sum_d_sqrd * sum_d_sqrd;
	double c3 = 4 * sum_d_sqrd * f;
	double c2 = 2 * sum_d_sqrd * l + k * k - g;
	double c1 = 2 * k * l - h;
	double c0 = l * l - m;

	double A = c3 / c4;
	double B = c2 / c4;
	double C = c1 / c4;
	double D = c0 / c4;

	auto* solutions = solve_quartic(A, B, C, D);

	std::vector<double> real_solutions;
	for (int ti = 0; ti < 4; ti++)
	{
		if (solutions[ti].imag() != 0.0 || solutions[ti].real() < 0 || solutions[ti].real() < RAY_EPSILON)
		{
			continue;
		}
		real_solutions.push_back(solutions[ti].real());
	}

	delete[] solutions;

	if (real_solutions.empty()) { 
		return false; 
	}

	std::sort(real_solutions.begin(), real_solutions.end());

	double tmin = real_solutions[0];

	i.t = tmin;
	i.obj = this;

	vec3f pp = r.at(tmin);
	double alpha = 1.0 - sweptRadius / sqrt(pp[0] * pp[0] + pp[1] * pp[1]);
	i.N = vec3f{ alpha * pp[0], alpha * pp[1], pp[2] }.normalize();

	return true;
}

