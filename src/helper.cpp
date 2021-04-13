#include "helper.h"

inline double random_double() {
    return rand() / RAND_MAX;
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}
vector<vec3f> jitteredSample3D(vec3f d, int n, double range) {
    vector<vec3f> res;
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
            for (int r = 0; r < n; r++) {
                vec3f jitteredPoint = vec3f((p + random_double()) / n - 0.5,
                    (q + random_double()) / n - 0.5,
                    (r + random_double()) / n - 0.5);
                jitteredPoint = (jitteredPoint * range + d).normalize();
                res.push_back(jitteredPoint);
            }
        }
    }
    return res;
}