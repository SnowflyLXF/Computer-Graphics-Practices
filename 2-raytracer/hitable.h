#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
#include "aabb.h"

using namespace std;

AABB surrounding_box(AABB a, AABB b){
	vec3 small(ffmin(a.min_pos()[0], b.min_pos()[0]), 
				ffmin(a.min_pos()[1], b.min_pos()[1]),
				ffmin(a.min_pos()[2], b.min_pos()[2]));
	vec3 big(ffmax(a.max_pos()[0], b.max_pos()[0]), 
			ffmax(a.max_pos()[1], b.max_pos()[1]),
			ffmax(a.max_pos()[2], b.max_pos()[2]));
	return AABB(small, big);
}


struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    int material;
};

class hitable 
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    // virtual vec3 min_pos() const =0;
    // virtual vec3 max_pos() const = 0;
    virtual bool bounding_box(AABB& box) const = 0;
};


#endif