#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list: public hitable
{
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) {list = l, list_size = n;}
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    // virtual vec3 min_pos() const;
    // virtual vec3 max_pos() const;
    virtual bool bounding_box(AABB& box) const;
    hitable **list;
    int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool hit_anything = false; 
    double closest_so_far = t_max;
    for (int i=0; i<list_size; i++){
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

// vec3 hitable_list::min_pos() const {
//     vec3 v = list[0]->min_pos();
//     for (int i=1; i<list_size; i++){
//         vec3 t = list[i]->min_pos();
//         v[0] = t[0]<v[0] ? t[0] : v[0];
//         v[1] = t[1]<v[1] ? t[1] : v[1];
//         v[2] = t[2]<v[2] ? t[2] : v[2];
//     }
//     return v;
// }

// vec3 hitable_list::max_pos() const {
//     vec3 v = list[0]->max_pos();
//     for (int i=1; i<list_size; i++){
//         vec3 t = list[i]->max_pos();
//         v[0] = t[0]>v[0] ? t[0] : v[0];
//         v[1] = t[1]>v[1] ? t[1] : v[1];
//         v[2] = t[2]>v[2] ? t[2] : v[2];
//     }
//     return v;
// }

bool hitable_list::bounding_box(AABB& box) const{
    if(list_size<1) return false;
    AABB temp_box;
    bool first_true = list[0]->bounding_box(temp_box);
    if(!first_true) return false;
    else box = temp_box;
    for(int i=1; i<list_size; i++){
        if(list[0]->bounding_box(temp_box)){
            box = surrounding_box(box, temp_box);
        }
        else return false;
    }
    return true;

}

#endif