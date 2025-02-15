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
            vec3 unit_direction = unit_vector(r_in.direction());
            double ri;
            vec3 normal;
            if (dot(r_in.direction(), rec.normal) < 0.0) {
                normal = rec.normal;
                ri = 1 / refraction_index;
            } else {
                normal = -1.0 * rec.normal;
                ri = refraction_index;
            }
            double cos_theta = std::fmin(dot(-unit_direction, normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) {     //Reflect
                scattered = ray(rec.p, reflect(unit_direction, normal));
            } else {                        //Refract
                scattered = ray(rec.p, refract(unit_direction, normal, ri));
            }
            return true;
        }

    private:

        double refraction_index;    //Refractive index of material over refractive index of vacuum/air

        //Helper function to acquire Schlick's Approximation of the Fresnel Factor
        static double reflectance(double cosine, double refraction_index) {
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);

        }

};

#endif