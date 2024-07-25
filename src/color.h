#ifndef COLOR_H
#define COLOR_H

#include "raytracing.h"
#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream& out, const color& pixel) {

    //Extract rgb information from pixel

    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    //Converting form linear space to gamma 2 space

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //Translate from unit values to byte range

    static const interval intensity = interval(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    //Write out pixel

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif