#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

//Important constants

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

//Utility

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

inline double radians_to_degrees(double radians) {
    return 180.0 * radians / PI;
}

//Returns within the range of [0, 1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

//Returns within the range of [min, max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

//Headers

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"


#endif