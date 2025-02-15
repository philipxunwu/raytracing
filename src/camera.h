#ifndef CAMERA_H
#define CAMERA_H

#include "raytracing.h"
#include "hittable.h"
#include "material.h"

class camera {
    public: 
        double aspect_ratio = 1.0;              //width divided by height
        int image_width = 100;
        int samples_per_pixel = 10;             //count of random samples for each pixel square for anti-aliasing
        int max_depth = 10;                     //maximum depth for recursion
        double vfov = 90;                       //Vertical view angle
        point3 lookfrom = point3(0, 0, 0);      //Point camera is looking from;
        point3 lookat = point3(0, 0, -1);       //Point camera is looking at
        vec3 vup = vec3(0, 1, 0);               //Camera-relative "up" direction;

        void render(const hittable& world) {
            initialize();

            std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j <image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_color * pixel_samples_scale_factor);
                }
            }
        }

    private: 

        int image_height;                   //image height
        double pixel_samples_scale_factor;  //scaling factor for a sum of pixel samples
        point3 camera_center;               //camera center
        point3 pixel00;                     //location of pixel (0, 0)
        vec3 pixel_horizontal;              //distance between each pixel center horizontally
        vec3 pixel_vertical;                //distance between each pixel center vertically;
        vec3 u;                             //unit vector pointing to camera's right
        vec3 v;                             //unit vector pointing to camera's up
        vec3 w;                             //unit vector pointing in the opposition direction of the camera's view

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale_factor = 1.0 / samples_per_pixel;

            camera_center = lookfrom;

            auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focal_length;
            auto viewport_width = viewport_height * (double(image_width) / image_height);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = unit_vector(cross(w, u));

            auto viewport_horizontal = viewport_width * u;
            auto viewport_vertical = viewport_height * -v;

            pixel_horizontal = viewport_horizontal / image_width;
            pixel_vertical = viewport_vertical / image_height;

            auto viewport_upper_left = camera_center - focal_length * w - viewport_horizontal / 2 - viewport_vertical / 2;

            pixel00 = viewport_upper_left + 0.5 * pixel_horizontal + 0.5 * pixel_vertical;
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00 + ((i + offset.x()) * pixel_horizontal) + ((j + offset.y()) * pixel_vertical);

            auto ray_direction = pixel_sample - camera_center;

            return ray(camera_center, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) {
                return color(0, 0, 0);
            }
            hit_record rec;
            if (world.hit(r, interval(0.0001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat -> scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth - 1, world);
                }
                return color(0, 0, 0);
            }
            vec3 unit_ray = unit_vector(r.direction());
            auto y_coordinate = 0.5 * (unit_ray.y() + 1.0);
            return (1.0 - y_coordinate) * color(1.0, 1.0, 1.0) + y_coordinate * color(0.5, 0.7, 1.0);
        }
};

#endif