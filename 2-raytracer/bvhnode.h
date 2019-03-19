#ifndef BVHH
#define BVHH

#include "hitable.h"

int box_x_compare(const void * a, const void * b) {
    AABB box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min_pos()[0] - box_right.min_pos()[0] < 0.0)
        return -1;
    else return 1;
}

int box_y_compare(const void * a, const void * b) {
    AABB box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min_pos()[1] - box_right.min_pos()[1] < 0.0)
        return -1;
    else return 1;
}

int box_z_compare(const void * a, const void * b) {
    AABB box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min_pos()[2] - box_right.min_pos()[2] < 0.0)
        return -1;
    else return 1;
}

class bvh_node : public hitable
{
public:
    bvh_node(){}
    bvh_node(hitable **l, int n);
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual bool bounding_box(AABB& b) const;

    hitable *left;
    hitable *right;
    AABB box;
};

bvh_node::bvh_node(hitable **l, int n){
    int axis = int(3*drand48());
    if (axis == 0)
        qsort(l, n, sizeof(hitable *), box_x_compare);
    else if (axis == 1)
        qsort(l, n, sizeof(hitable *), box_y_compare);
    else 
        qsort(l, n, sizeof(hitable *), box_z_compare);
    
    if (n == 1) {
        left = l[0];
        right = l[0];
    }
    else if (n == 2){
        left = l[0];
        right = l[1];
    }
    else{
        left = new bvh_node(l, n/2);
        right = new bvh_node(l+n/2, n-n/2);
    }
    AABB box_left, box_right;
    if(!left->bounding_box(box_left) || !right->bounding_box(box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray& r, float tmin, float tmax, hit_record& rec) const{
    if(box.hit(r, tmin, tmax)){
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, tmin, tmax, left_rec);
        bool hit_right = right->hit(r, tmin, tmax, right_rec);
        if (hit_left && hit_right){
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else rec = right_rec;
            return true;
        }
        else if (hit_left){
            rec = left_rec;
            return true;
        }
        else if (hit_right){
            rec = right_rec;
            return true;
        }
        else return false;
    }
    else return false;
}

bool bvh_node::bounding_box(AABB& b) const{
    b = box;
    return true;
}

#endif