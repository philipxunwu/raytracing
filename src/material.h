#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracing.h"

class hit_record;

class material {
    public: 
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian: public material {
    public: 
        lambertian(const color& albedo) 
            : albedo(albedo) {}
        
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }            
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    private: 
        color albedo;
};

class metal: public material {
    public:
        metal(const color& albedo, double fuzz) 
            : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        color albedo;
        double fuzz;
};

class dielectric: public material {
    public:
        dielectric(double refraction_index)  
            : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            if (dot(r_in.direction(), rec.normal) < 0.0) { //normal is against incoming ray
                scattered = ray(rec.p, refract(unit_vector(r_in.direction()), rec.normal, 1.0 / refraction_index));
                return true;
            } else {
                scattered = ray(rec.p, refract(unit_vector(r_in.direction()), -1.0 * rec.normal, refraction_index));
                return true;
            }
        }

    private:

        double refraction_index;

};

#endif