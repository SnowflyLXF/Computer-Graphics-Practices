#ifndef PLANEH
#define PLANEH

#include "hitable.h"

class plane: public hitable
{
public:
    plane() {}
    plane(vec3 p_, vec3 n_, int m): p(p_), n(unit_vector(n_)), material(m){};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual bool bounding_box(AABB& box) const;

    vec3 p, n;
    int material;
};


bool plane::bounding_box(AABB& box) const{
}

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 ni(0, 0, 0);
    if (dot(r.direction(), n) > 0) ni = -n;
    else ni = n;
    float t = -(dot(r.origin(), ni) - dot(p, ni)) / dot(r.direction(), ni);
    if(t>t_max && t<t_min){
        vec3 p = r.origin() + r.direction() * t;
        rec.t = t;
        rec.p = p;
        rec.normal = ni;
        rec.material = material;
        return true;
    }
    return false;
}



#endif