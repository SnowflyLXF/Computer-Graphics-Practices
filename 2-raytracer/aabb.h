#ifndef AABBH
#define AABBH

#include "ray.h"

inline float ffmin(float a, float b){return a<b?a:b;}
inline float ffmax(float a, float b){return a>b?a:b;}


class AABB
{
public:
	AABB(){}
	AABB(const vec3& a, const vec3& b){
		_min = a;
		_max=b;
	}

	vec3 min_pos() const{return _min;}
	vec3 max_pos() const{return _max;}


	// void add_child(AABB c){
	// 	AABB *new_child = new AABB[child_num];
	// 	for (int i=0; i<child_num; i++)
	// 		new_child[i] = child[i];
	// 	new_child[child_num++] = c;
	// 	child = new_child;
	// }

	virtual bool hit(const ray& r, float t_min, float t_max) const {
		float t0, t1;
		for(int i=0; i<3; i++){ 
			t0 = ffmin((_min[i] - r.origin()[i]) / r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);
			t1 = ffmax((_min[i] - r.origin()[i]) / r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);
			
			t_min = ffmax(t0, t_min);
			t_max = ffmin(t1, t_max);
			// if(r.direction()[a]>-0.00000001 && r.direction()[a] < 0.00000001)
			// 	return false;
			// else{
			// float invD = 1.0f/r.direction()[a];
			// float t0 = (min()[a]-r.origin()[a])*invD;
			// float t1 = (max()[a]-r.origin()[a])*invD;
			// if(invD<.0f) std::swap(t0, t1);
			// 	t_min = t0>t_min?t0:t_min;
			// 	t_max = t1<t_max?t1:t_max;
			if(t_max<=t_min) return false;
				// if(child[1]!=NULL && child[1]->hit(r, t_min, t_max, rec))return true;
		}
		// hit_record rec1;
		// if (child->hit(r, t_min, t_max, rec1)) {
		// 	rec = rec1;
		// 	return true;
		// }
		// else return false;
		return true;
	}

	// bool intersect(AABB* box){
	// 	vec3 v = _min - box->max_pos();
	// 	if (v[0]>=0 && v[1]>=0 && v[2]>=0) return true;
	// 	v = box->min_pos() - _max;
	// 	if (v[0]>=0 && v[1]>=0 && v[2]>=0) return true;
	// 	return false;
	// }

	// void append(AABB* box){
	// 	vec3 v_max, v_min;
	// 	if ((_min - box->max_pos())[0]>=0) {
	// 		v_min = _min - box->max_pos();
	// 		v_max = _max - box->min_pos();
	// 		_min = box->min_pos();
	// 	}
	// 	else {
	// 		v_min = box->min_pos() - _max;
	// 		v_max = box->max_pos() - _min;
	// 		_max = box->max_pos();
	// 	}

	// 	if (v_min[0]*v_min[1]*v_min[2] > v_max[0]*v_max[1]*v_max[2]*0.7)
	// 		child[1] = box->getchild();
	// 	else child[1] = box;
	// 	hitable *newobj = new hitable_list(child, 2);
	// 	child[0] = newobj;
	// 	child[1] = NULL;
	// }

	vec3 _min, _max;
};

#endif