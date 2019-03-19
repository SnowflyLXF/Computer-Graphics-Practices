#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable
{
public:
    sphere() {}
    sphere(vec3 cen, float ra, int m): center(cen), radius(ra), material(m){};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    // virtual vec3 min_pos() const {return center-vec3(radius, radius, radius);}
    // virtual vec3 max_pos() const {return center+vec3(radius, radius, radius);}
    virtual bool bounding_box(AABB& box) const;

    vec3 center;
    float radius;
    int material;

};

// bool sphere::inside(const ray& r, hit_record& rec) const {
//     return (dot(r.direction(), rec.normal)>0);
// }

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if(temp<t_max && temp>t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = unit_vector(rec.p - center);
            rec.material = material;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp<t_max && temp>t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.material = material;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(AABB& box) const{
    box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

#endif