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

bool roughly(double value, double basis)
{
    if (value > basis - 0.02 && value < basis + 0.02)
        return true;
    return false;
}

void Box::intersect(const ray& r, double &tMin, BoundingBox bb) const
{
    vec3f R0 = r.getPosition();
    vec3f Rd = r.getDirection();

    tMin = -1.0e308;
    double ttemp;

    for (int currentaxis = 0; currentaxis < 3; currentaxis++)
    {
        double vd = Rd[currentaxis];

        // if the ray is parallel to the face's plane (=0.0)
        if (vd == 0.0)
            continue;

        double v1 = bb.min[currentaxis] - R0[currentaxis];
        double v2 = bb.max[currentaxis] - R0[currentaxis];

        // two slab intersections
        double t1 = v1 / vd;
        double t2 = v2 / vd;

        if (t1 > t2) { // swap t1 & t2
            ttemp = t1;
            t1 = t2;
            t2 = ttemp;
        }

        if (t1 > tMin)
            tMin = t1;
    }
}

std::pair<double, double> Box::getUV(const ray& r, const isect& i) const
{
    vec3f pos = transform->globalToLocalCoords(r.getPosition());
    vec3f dir = transform->globalToLocalCoords(r.getPosition() + r.getDirection()) - pos;
    dir = dir.normalize();

    ray localRay(pos, dir);

    BoundingBox bb;
    bb.max = vec3f(0.5, 0.5, 0.5);
    bb.min = vec3f(-0.5, -0.5, -0.5);

    double tMin = -999;
    intersect(localRay, tMin, bb);
    vec3f posOnBox = localRay.at(tMin);

    double p1 = 0;
    double p2 = 0;

    if (posOnBox[0] > -0.5 && posOnBox[0] < 0.5 && roughly(posOnBox[1], 0.5) && posOnBox[2] > -0.5 && posOnBox[2] < 0.5) {
        p1 = posOnBox[0];
        p2 = posOnBox[2];
    }
    else if (posOnBox[0] > -0.5 && posOnBox[0] < 0.5 && roughly(posOnBox[1], -0.5) && posOnBox[2] > -0.5 && posOnBox[2] < 0.5) {
        p1 = posOnBox[0];
        p2 = posOnBox[2];
    }
    else if (roughly(posOnBox[0], 0.5) && posOnBox[1] > -0.5 && posOnBox[1] < 0.5 && posOnBox[2] > -0.5 && posOnBox[2] < 0.5) {
        p1 = posOnBox[1];
        p2 = posOnBox[2];
    }
    else if (roughly(posOnBox[0], -0.5) && posOnBox[1] > -0.5 && posOnBox[1] < 0.5 && posOnBox[2] > -0.5 && posOnBox[2] < 0.5) {
        p1 = posOnBox[1];
        p2 = posOnBox[2];
    }
    else if (posOnBox[0] > -0.5 && posOnBox[0] < 0.5 && posOnBox[1] > -0.5 && posOnBox[1] < 0.5 && roughly(posOnBox[2], 0.5)) {
        p1 = posOnBox[0];
        p2 = posOnBox[1];
    }
    else if (posOnBox[0] > -0.5 && posOnBox[0] < 0.5 && posOnBox[1] > -0.5 && posOnBox[1] < 0.5 && roughly(posOnBox[2], -0.5)) {
        p1 = posOnBox[0];
        p2 = posOnBox[1];
    }
    
    return { p1 + 0.5, p2 + 0.5 };
}
