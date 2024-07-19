#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {

    private: 
        point3 orig;
        vec3 dir;

    public: 
        //Constructor
        ray(const point3& origin, const vec3& direction) 
            : orig(origin), dir(direction) {}

        //Access methods
        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        //Calculating current position of ray given a time 
        point3 at(double t) const {
            return orig + t * dir;
        } 
};


#endif