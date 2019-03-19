#ifndef RAYH
#define RAYH
#include "vec3.h"
#include "pixel.h"

class ray
{
public:
    ray(){}
    ray(const vec3& a, const vec3& b, const float c) {A=a; B=unit_vector(b); ior=c;}

    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    float ni() const {return ior;}
    vec3 point_at_parameter(float t) const {return A+B*t;}
    // Pixel color_at_point(float radius=1.0, ){
    //     if (tp==0) return rgb;
    //     if (tp==1) return rgb*(1/(radius*radius));
    //     if (tp==2) return 
    // }
    // int light_type() const{return tp;}

    vec3 A;
    vec3 B;
    float ior;
};

#endif