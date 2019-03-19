#ifndef TRIANGLEH
#define TRIANGLEH

#include "hitable.h"

bool same_side(vec3 p1, vec3 p2, vec3 a, vec3 b){
    vec3 cp1 = cross(b-a, p1-a);
    vec3 cp2 = cross(b-a, p2-a);
    return dot(cp1, cp2)>=0;
}

class triangle: public hitable
{
public:
    triangle() {}
    triangle(vec3 v1_, vec3 v2_, vec3 v3_, int m): v1(v1_), v2(v2_), v3(v3_), material(m){};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual bool bounding_box(AABB& box) const;

    vec3 v1, v2, v3;
    int material;
};


bool triangle::bounding_box(AABB& box) const{
    vec3 min = v1, max = v1;
    for(int i=0; i<3; i++){
        min[i] = ffmin(min[i], v2[i]);
        min[i] = ffmin(min[i], v3[i]);
        max[i] = ffmax(max[i], v2[i]);
        max[i] = ffmax(max[i], v3[i]);
    }
    box = AABB(min, max);
    return true;
}

bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // vec3 n = unit_vector(cross(v3-v1, v2-v1));
    // if (dot(r.direction(), n) > 0) n = -n;
    // float t = -(dot(r.origin(), n) - dot(v1, n)) / dot(r.direction(), n);
    // if(t>t_max || t<t_min){
    //     vec3 p = r.origin() + r.direction() * t;
    //     if (same_side(p, v1, v2, v3) && same_side(p, v2, v1, v3) && same_side(p, v3, v1, v2)){
    //         rec.t = t;
    //         rec.p = p;
    //         rec.normal = n;
    //         rec.material = material;
    //         return true;
    //     }
    // }
    // else {return false;}
    vec3 e1 = v2 - v1;
    vec3 e2 = v3 - v1;
    vec3 q = cross(r.direction(), e2);
    float a = dot(e1, q);
    if(a>-0.000000001 && a < 0.000000001) return false;
    // float f = 1/a;
    vec3 s = r.origin() - v1;
    float u = dot(q, s)/a;
    if(u<0.0) return false;
    vec3 rt = cross(s, e1);
    float v = dot(rt, r.direction())/a;
    if(v<0.0 || u+v > 1.0) return false;
    float t = dot(rt, e2)/a;
    if(t>t_max || t<t_min) return false;
    rec.t = t;
    vec3 p = r.origin() + r.direction() * t;
    rec.p = p;
    vec3 n = unit_vector(cross(v1 - v3, v2 - v3));
    if(dot(r.direction(), n)>0 )
        rec.normal = -n;
    else 
        rec.normal = n;
    rec.material = material;
    
    return true;
}



#endif