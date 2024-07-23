#ifndef HITTABLE_H
#define HITTABLE_H

#include "raytracing.h"

//A class to store information about a ray-object collision
class hit_record {
    public: 
        point3 p;
        vec3 normal;
        double t;
};

//Base class for all objects that can be interacted with by rays
class hittable {
    public: 
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif