#ifndef SPHERE_H
#define SPHERE_H


#include "vec3.h"
#include "hittable.h"

//Defining sphere class that derives from hittable. 
class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) 
            : center(center), radius(fmax(0, radius)) {}

        //Determines whether a sphere is hit by a ray and stores information on that collision in a hit_record
        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            vec3 c_q = center - r.origin();
            vec3 direction = r.direction();
            auto a = dot(direction, direction);
            auto h = dot(direction, c_q);
            auto c = dot(c_q, c_q) - radius * radius;
            auto discriminant = h*h - a*c;
            if (discriminant < 0) {
                return false;
            }
            auto t = (h - sqrt(discriminant)) / a;
            if (t <= ray_tmin || t >= ray_tmax) {
                t = (h + sqrt(discriminant)) / a;
                if (t <= ray_tmin || t >= ray_tmax) {
                    return false;
                }
            }
            
            //storing information in record
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    private:
        point3 center;
        double radius;
};

#endif