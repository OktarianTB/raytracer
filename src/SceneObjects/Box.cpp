#include <cmath>
#include <assert.h>
#include <iostream>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// Bottom corner is (-0.5, -0.5, -0.5) and top corner is (0.5, 0.5, 0.5)
    BoundingBox bb;
    bb.max = vec3f(0.5, 0.5, 0.5);
    bb.min = vec3f(-0.5, -0.5, -0.5);

    double tmin, tmax;
    if (!bb.intersect(r, tmin, tmax))
    {
        return false;
    }

    if (tmin < RAY_EPSILON || tmax < RAY_EPSILON || abs(tmax - tmin) < RAY_EPSILON)
    {
        return false;
    }

    i.t = tmin;
    i.obj = this;
    i.N = r.at(tmin).normalize();

	return true;
}

/*
    vec3f Pn(0.5, 0.5, 0.5);

    double Vd = Pn.dot(D);
    double v0 = -(Pn.dot(P0) + 0.5);

    if (abs(Vd) < 0.001)
    {
        cout << "False" << endl;
        return false;
    }


    double t = v0 / Vd;

    if (t < 0)
    {
        //cout << "False: " << t << endl;
        return false;
    }

    i.obj = this;
    i.t = t;
    i.N = P0 + D * t;

    cout << "True: " << t << endl;
* 
* ---
double txmin = (B0[0] - origin[0]) / direction[0];
    double txmax = (B1[0] - origin[0]) / direction[0];

    if (txmin > txmax) {
        double temp = txmin;
        txmin = txmax;
        txmax = temp;
    }

    double tymin = (B0[1] - origin[1]) / direction[1];
    double tymax = (B1[1] - origin[1]) / direction[1];

    if (tymin > tymax) {
        double temp = tymin;
        tymin = tymax;
        tymax = temp;
    }

    if ((txmin > tymax) || (tymin > txmax))
        return false;

    if (tymin > txmin)
        txmin = tymin;

    if (tymax < txmax)
        txmax = tymax;

    double tzmin = (B0[2] - origin[2]) / direction[2];
    double tzmax = (B1[2] - origin[2]) / direction[2];

    if (tzmin > tzmax) {
        double temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }

    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    if (tzmin > txmin)
        txmin = tzmin;

    if (tzmax < txmax)
        txmax = tzmax;

    i.t = txmax < txmin ? txmax : txmin;
    i.N = r.at(i.t).normalize();
*/