#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel) {
    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    int rbyte = int(r * 255.999);
    int gbyte = int(g * 255.999);
    int bbyte = int(g * 255.999);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif