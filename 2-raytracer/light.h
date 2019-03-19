#ifndef LIGHTH
#define LIGHTH

#include <algorithm>
#include "dependency/vec3.h"
#include "hitablelist.h"
#include "float.h"
#include <math.h> 

using namespace std;

vec3 reflect(const vec3& l, const vec3& n){
    return unit_vector(l-n*2*dot(l, n));
}

class Light
{
public:
    virtual bool reach(vec3 pos, hitable *world) const = 0;
    virtual vec3 diffuse_color(vec3 pos, vec3 normal, vec3 kd, hitable *world) const = 0;
    virtual vec3 specular_color(vec3 pos, vec3 normal, vec3 r, vec3 ks, int ns, hitable *world) const = 0;
};

class directionLight: public Light
{
public:
    directionLight(){}
    directionLight(vec3 rgb_, vec3 direction_) {
        rgb = rgb_;
        dir = unit_vector(direction_);
    }
    
    virtual bool reach(vec3 pos, hitable *world) const {
        ray backtrace(pos, -dir, 1.0f);
        hit_record rec;
        return (!world->hit(backtrace, .001, MAXFLOAT, rec));
    }
    virtual vec3 diffuse_color(vec3 pos, vec3 normal, vec3 kd, hitable *world) const
    {
        return kd * rgb * max(.0f, dot(normal, -dir));
    }

    virtual vec3 specular_color(vec3 pos, vec3 normal, vec3 r, vec3 ks, int ns, hitable *world) const
    {
        vec3 v = reflect(-dir, normal);
        return ks * rgb * pow(max(.0f, dot(v, r)),  ns);
    }

    vec3 position(vec3 pos) {return pos-dir;}
    vec3 direction(vec3 pos) {return dir;}

    vec3 rgb;
    vec3 dir;
    
};

class pointLight: public Light
{
public:
    pointLight() {}
    pointLight(vec3 rgb_, vec3 point_){
        rgb = rgb_;
        point = point_;
    }
    
    virtual bool reach(vec3 pos, hitable *world) const {
        ray backtrace(pos, (point-pos), 1.0f);
        hit_record rec;
        return (!world->hit(backtrace, .001, (point - pos).length(), rec));
    }

    virtual vec3 diffuse_color(vec3 pos, vec3 normal, vec3 kd, hitable *world) const
    {
        float dist = (pos-point).length();
        return kd * rgb * max(.0f, dot(normal, unit_vector(point-pos)))/(dist*dist);
    }

    virtual vec3 specular_color(vec3 pos, vec3 normal, vec3 r, vec3 ks, int ns, hitable *world) const
    {
        vec3 v = reflect(point-pos, normal);
        float dist = (pos-point).length();
        return ks * rgb * pow(max(.0f, dot(v, r)),  ns) / (dist*dist);
    }

    vec3 position(vec3 pos) {return point;}
    vec3 direction(vec3 pos) {return unit_vector(pos-point);} 

    vec3 rgb;
    vec3 point;
};

class spotLight: public Light
{
public:
    spotLight(){}
    spotLight(vec3 rgb_, vec3 point_, vec3 dir_, float a1, float a2){
        rgb = rgb_;
        point = point_;
        dir = dir_;
        angle1 = a1;
        angle2 = a2;
    }

    virtual bool reach(vec3 pos, hitable *world) const {
        if (dot(unit_vector(pos - point), dir) >= cos(angle2 * M_PI / 180.0f)){
            ray backtrace(pos, (point-pos), 1.0f);
            hit_record rec;
            return (!world->hit(backtrace, .001, MAXFLOAT, rec));
        }
        return false;
    }

    virtual vec3 diffuse_color(vec3 pos, vec3 normal, vec3 kd, hitable *world) const {
            float dist = (pos-point).length();
            if (dot(unit_vector(pos - point), dir) >= cos(angle1 * M_PI / 180.0f)) {
                return kd * rgb * max(.0f, dot(normal, unit_vector(point-pos)))/(dist*dist);
            }
            else if (dot(unit_vector(pos - point), dir) >= cos(angle2 * M_PI / 180.0f)){
                return kd * rgb * max(.0f, dot(normal, unit_vector(point-pos))) * (angle2 - acos(dot(unit_vector(pos - point), dir)*180.0f/M_PI)) / (angle2-angle1) / (dist*dist);
            }
        return vec3(.0, .0, .0);
    }

    virtual vec3 specular_color(vec3 pos, vec3 normal, vec3 r, vec3 ks, int ns, hitable *world) const{
        vec3 v = reflect(point-pos, normal);
        float dist = (pos-point).length();
        if (dot(unit_vector(pos - point), dir) >= cos(angle1 * M_PI / 180.0f)) {
            return ks * rgb * pow(max(.0f, dot(v, r)), ns) / (dist*dist);
        }
        else if (dot(unit_vector(pos - point), dir) >= cos(angle2 * M_PI / 180.0f)){
            return ks * rgb * pow(max(.0f, dot(v, r)), ns) * (angle2 - acos(dot(unit_vector(pos - point), dir)*180.0f/M_PI)) / (angle2-angle1) / (dist*dist);
        }
    }

    vec3 position(vec3 pos) {return point;}

    vec3 direction(vec3 pos) {return unit_vector(pos-point);}

    vec3 rgb, point, dir; 
    float angle1, angle2;
};

class Lightlist: public Light
{
public:
    Lightlist(){}
    Lightlist(Light **l, int n) {list = l, list_size = n;}

    virtual bool reach(vec3 pos, hitable *world) const {
        for (int i=0; i<list_size; i++){
            if (list[i]->reach(pos, world)) return true;
        }
        return false;
    }

    virtual vec3 diffuse_color(vec3 pos, vec3 normal, vec3 kd, hitable *world) const {
        vec3 col(.0, .0, .0);
        for (int i=0; i<list_size; i++){
            if(list[i]->reach(pos, world))
                col += list[i]->diffuse_color(pos, normal, kd, world);
        }
        return col;
    }

    virtual vec3 specular_color(vec3 pos, vec3 normal, vec3 r, vec3 ks, int ns, hitable *world) const
    {
        vec3 col(.0, .0, .0);
        for (int i=0; i<list_size; i++){
            if (list[i]->reach(pos, world))
                col += list[i]->specular_color(pos, normal, r, ks, ns, world);
        }
        return col;
    }

    Light **list;
    int list_size;

};


#endif