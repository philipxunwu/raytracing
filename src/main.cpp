#include "raytracing.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

//Determines the color for a pixel depending on its corresponding ray
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
    }
    vec3 unit_ray = unit_vector(r.direction());
    auto y_coordinate = 0.5 * (unit_ray.y() + 1.0);
    return (1.0 - y_coordinate) * color(1.0, 1.0, 1.0) + y_coordinate * color(0.5, 0.7, 1.0);

}

int main() {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0 , -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);
}