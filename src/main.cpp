#include "raytracing.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

//Determines the color for a pixel depending on its corresponding ray
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_ray = unit_vector(r.direction());
    auto y_coordinate = 0.5 * (unit_ray.y() + 1.0);
    return (1.0 - y_coordinate) * color(1.0, 1.0, 1.0) + y_coordinate * color(0.5, 0.7, 1.0);

}

int main() {

    //Image

    int image_width = 400;
    auto aspect_ratio = 16.0 / 9.0;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;


    //World

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5,-1), 100));

    //Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    auto viewport_horizontal = vec3(viewport_width, 0, 0);
    auto viewport_vertical = vec3(0, -viewport_height, 0);

    auto pixel_horizontal = viewport_horizontal / image_width;
    auto pixel_vertical = viewport_vertical / image_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_horizontal / 2 - viewport_vertical / 2;

    auto pixel00 = viewport_upper_left + 0.5 * pixel_horizontal + 0.5 * pixel_vertical;

    //Render

    std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j <image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00 + i * pixel_horizontal + j * pixel_vertical;
            auto ray_direction = pixel_center - camera_center;
            auto r = ray(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
}


