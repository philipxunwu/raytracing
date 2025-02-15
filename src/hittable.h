#ifndef HITTABLE_H
#define HITTABLE_H

#include "raytracing.h"

class material;

//A class to store information about a ray-object collision
class hit_record {
    public: 
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

//Base class for all objects that can be interacted with by rays
class hittable {
    public: 
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif