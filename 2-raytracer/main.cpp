#include <iostream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>  
#include <omp.h>
#include "dependency/ray.h"
#include "dependency/image.h"
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "hitable.h"
#include "hitablelist.h"
#include "aabb.h"
#include "plane.h"
#include "bvhnode.h"
#include "float.h"

using namespace std;

#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"

struct Material{
    vec3 ka, kd, ks, kr;
    float ior, ns;
};


bool dielectric(const vec3& l, const vec3& n, float ni, float nr){
    float sinr = nr * sqrt(1-dot(n, l)*dot(n, l)) / ni;
    return (sinr>=0 && sinr<1.0);
}

float schlick(float ni, float nr, float c){
    float r0 = ((ni-nr)/(ni+nr)) * ((ni-nr)/(ni+nr));
    return r0 + (1-r0) * pow((1-c), 5);
}

vec3 refract(const vec3& l, vec3& n, float ni, float nr){
    vec3 normal;
    if (dot(n, l)>=0) normal = -n;
    else normal = n;
    vec3 t = -normal * (ni*dot(normal, l)/nr + sqrt(1-ni*ni*(1-dot(normal, l)*dot(normal, l))/(nr*nr)))
            + l * ni/nr;
    return t;
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do{
        p = vec3(drand48(), drand48(), drand48())*2.0 - vec3(1, 1, 1);
    } while (p.squarted_length()>=1.0);
    return p;
}

vec3 color(const ray &ray_in, hitable *world, Material *matlist, Light *li, vec3 ambient, int depth, vec3 bgc)
{
    hit_record rec;
    vec3 col(.0, .0, .0);
    if (world->hit(ray_in, .001, MAXFLOAT, rec))
    {
        col = matlist[rec.material].ka * ambient;

        if (li->reach(rec.p, world))
        {
            // hit_record rec1;
            // if (!world->hit(ray(rec.p, -plight[i].direction(rec.p), 1.0), .001, MAXFLOAT, rec1)){
            // vec3 reflect_l = reflect(plight[i].direction(rec.p), rec.normal);
            col += li->diffuse_color(rec.p, rec.normal, matlist[rec.material].kd, world);
            if (matlist[rec.material].ns)
            {
                col += li->specular_color(rec.p, rec.normal, ray_in.direction(), matlist[rec.material].ks, matlist[rec.material].ns, world);
            }
        }

        if (depth>0) {
            if (dot(ray_in.direction(), rec.normal)<=0){
                ray ray_refract(rec.p, refract(ray_in.direction(), rec.normal, ray_in.ni(), matlist[rec.material].ior), matlist[rec.material].ior);
                hit_record rec2;
                world->hit(ray_refract, .001, MAXFLOAT, rec2);
                col += matlist[rec.material].kr*color(ray_refract, world, matlist, li, ambient, depth-1, bgc);
            }
            else {
                if (dielectric(ray_in.direction(), rec.normal, ray_in.ni(), matlist[rec.material].ior)){

                    ray ray_refract(rec.p, refract(ray_in.direction(), rec.normal, ray_in.ni(), 1), 1);
                    hit_record rec2;
                    world->hit(ray_refract, .001, MAXFLOAT, rec2);
                    col += matlist[rec.material].kr*color(ray_refract, world, matlist, li, ambient, depth-1,bgc);
                }

            }

            ray ray_reflect(rec.p, reflect(ray_in.direction(), rec.normal), ray_in.ni());
            hit_record rec1;
            world->hit(ray_reflect, .001, MAXFLOAT, rec1);
            col += matlist[rec.material].ks * color(ray_reflect, world, matlist, li, ambient, depth - 1,bgc);
            
        //////////////
        // beer law //
        //////////////
        // float R;
        // vec3 kr = mat_list[rec.material].kr;
        // if (dot(ray_in.direction(), rec.normal)<=0){
        //     ray ray_refract(rec.p, refract(ray_in.direction(), rec.normal, ray_in.ni(), mat_list[rec.material].ior), mat_list[rec.material].ior);
        //     kr = vec3(1.0, 1.0, 1.0);
        //     R = schlick(ray_in.ni(), mat_list[rec.material].ior, -dot(ray_in.direction(), rec.normal));

        //     hit_record rec2;
        //     if (world->hit(ray_reflect, .001, MAXFLOAT, rec2))
        //         col += mat_list[rec2.material].ks * color(ray_reflect, world, --depth) * R;

        //     hit_record rec3;
        //     if (world->hit(ray_refract, .001, MAXFLOAT, rec3))
        //         col += kr * color(ray_refract, world, --depth) * (1-R);
        // }
        // else{
        //     if (dielectric(ray_in.direction(), rec.normal, ray_in.ni(), mat_list[rec.material].ior)){
        //         ray ray_refract(rec.p, refract(ray_in.direction(), rec.normal, ray_in.ni(), mat_list[rec.material].ior), mat_list[rec.material].ior);
        //         R = schlick(ray_in.ni(), mat_list[rec.material].ior, dot(ray_in.direction(), rec.normal));

        //         hit_record rec2;
        //         if (world->hit(ray_reflect, .001, MAXFLOAT, rec2))
        //             col += mat_list[rec2.material].ks * color(ray_reflect, world, --depth) * R;

        //         hit_record rec3;
        //         if (world->hit(ray_refract, .001, MAXFLOAT, rec3))
        //             col += kr * color(ray_refract, world, --depth) * (1-R);
        //     }
        //     else {
        //         hit_record rec4;
        //         if (world->hit(ray_reflect, .001, MAXFLOAT, rec4))
        //             col += mat_list[rec4.material].ks*color(ray_reflect, world, --depth);
        //     }
        // }

        }
    }
    else{
        col = bgc;
    }
    return col;
}

int main(int argc, char **argv){
    time_t t1, t2;
    t1 = time(NULL);
    Image *img = NULL;
    float nx = 640.0, ny = 480.0;
    int depth = 5;

    vec3 dir(.0, .0, 1.0);
    vec3 up(.0, 1.0, .0);
    vec3 origin(.0, .0, .0);
    float angle = 45;

    int num_l = 1;
    vec3 bgc(0.05, 0.05, 0.05);
    vec3 ambient(0.25, 0.25, 0.25);

    string line;
    // string fileName = argv[1];
    string fileName = "/Users/lixuefei/Desktop/raytracing/raytracer/scenefile/outdoor.scn";

    // open the file containing the scene description
    ifstream input;
    input.open(fileName);

    // check for errors in opening the file
    if(input.fail()){
        cout << "Can't open file '" << fileName << "'" << endl;
        return 0;
    }
    
    // determine the file size (this is optional -- feel free to delete the 6 lines below)
    streampos begin,end;
    begin = input.tellg();
    input.seekg(0, ios::end);
    end = input.tellg();
    cout << "File '" << fileName << "' is: " << (end-begin) << " bytes long.\n\n";
    input.seekg(0, ios::beg);

    int obj_num=0, material_num=0, normal_num, vertice_num, light_num=0, v_id=0, n_id=0;
    vec3 *v_list = NULL, *n_list = NULL;

    //Loop through reading each line
    string command;
    while(input >> command) { //Read first word in the line (i.e., the command type)
        
        if (command[0] == '#'){
        getline(input, line); //skip rest of line
        cout << "Skipping comment: " << command  << line <<  endl;
        continue;
        }
        
        if (command == "camera"){
            input >> origin >> dir >> up >> angle;
            cout << "Camera position: (" << origin[0] << ", "  << origin[1] << ", "  << origin[2] << ")." << endl;
        }
        else if (command == "film_resolution"){
            input >> nx >> ny;
        }
        else if (command == "ambient_light"){
            input >> ambient;
        }
        else if (command == "directional_light" || command == "point_light" || command == "spot_light"){
            light_num++;
        }
        else if (command == "background"){
            input >> bgc;
        }
        else if (command == "material"){
            material_num ++;
        }
        else if (command == "max_vertices"){
            input >> vertice_num;
            v_list = new vec3[vertice_num];
            v_id=0;
        }
        else if (command == "max_normals"){
            input >> normal_num;
            n_list = new vec3[normal_num];
            n_id=0;
        }
        else if(command == "vertex" && vertice_num != 0){
            vec3 v;
            input >> v;
            v_list[v_id++] = v;
        }
        else if(command == "normal" && normal_num != 0){
            input >> n_list[n_id++];
        }
        else if (command == "triangle" || command == "sphere" || command == "plane"){
            obj_num++;
        }
        else if (command == "background"){ //If the command is a background command
        float r,g,b;
        input >> r >> g >> b;
        printf("Background color of (%f,%f,%f)\n",r,g,b);
        }
        else if (command == "output_image"){ //If the command is an output_image command
        string outFile;
        input >> outFile;
        printf("Render to file named: %s\n", outFile.c_str());
        }
        else if (command == "max_depth"){
            input >> depth;
        }
        else {
        getline(input, line); //skip rest of line
        cout << "WARNING. Do not know command: " << command << endl;
        }
    }

    float dist = ny / (2 * tan(angle * M_PI / 180.0f));
    vec3 s = origin + dir * dist / dir.length();
    vec3 x_axis = unit_vector(cross(up, s));
    vec3 y_axis = -up;


    hitable *list[obj_num];

    Material *mat_list = new Material[material_num];
    Light *lightlist[light_num];

    int obj_id=0, m_id=0, l_id=0;

    input.close();
    input.open(fileName);
    while(input >> command) {
        if (command == "directional_light"){
            vec3 rgb, xyz;
            input >> rgb >> xyz;
            lightlist[l_id++] = new directionLight(rgb, xyz);
        }
        else if (command == "point_light"){
            vec3 rgb, xyz;
            input >> rgb >> xyz;
            lightlist[l_id++] = new pointLight(rgb, xyz);
        }
        else if (command == "spot_light"){
            vec3 rgb, xyz, dir;
            float a1,a2;
            input >> rgb >> xyz >> dir >> a1 >> a2;
            lightlist[l_id] = new spotLight(rgb, xyz, dir, a1, a2);
        }
        else if(command == "material"){
            vec3 ka,kd,ks,kr;
            float ns, ior;
            input >> ka >> kd >> ks >> ns >> kr >> ior;
            mat_list[m_id].ka = ka;
            mat_list[m_id].kd = kd;
            mat_list[m_id].ks = ks;
            mat_list[m_id].kr = kr;
            mat_list[m_id].ns = ns;
            mat_list[m_id].ior = ior;
            m_id++;

        }
        else if(command == "vertex"){
            float a, b, c;
            input>>a>>b>>c;
        }
        else if (command == "sphere"){
            vec3 pos;
            float radius;
            input >> pos >> radius;
            list[obj_id] = new sphere(pos, radius, m_id-1);
            obj_id++;
        }
        else if (command == "triangle"){
            int v1, v2, v3;
            input >> v1 >> v2 >> v3;
            list[obj_id] =  new triangle(v_list[v1], v_list[v2], v_list[v3], m_id-1);
            obj_id++;

        }
        else if (command == "plane"){
            vec3 v1, v2;
            input >> v1 >> v2;
            list[obj_id] =  new plane(v1, v2, m_id-1);
            obj_id++;
        }
        else {
            getline(input, line); //skip rest of line
            cout << "WARNING. Do not know command: " << command << endl;
        }
    }
    
    // hitable *world = new hitable_list(list, obj_num);
    bvh_node *world = new bvh_node(list, obj_num);
    Light *light = new Lightlist(lightlist, light_num);

    int sn = 1;
    img = new Image(nx, ny);
    #pragma omp parallel for collapse(2)
    for (int j=0; j<ny; j++){
        for (int i=0; i<nx; i++){
            vec3 col(.0, .0, .0);
            for (int s=0; s<sn; s++){
                vec3 u = x_axis * (-nx/2 + float(i));
                vec3 v = y_axis * (-ny/2 + float(j));
                vec3 p = dir*(dist) + u + v;
                // vec3 p = dir*(dist) + u + v + vec3(drand48()-.5, drand48()-.5, drand48()-.5);
                ray r(origin, p, 1.0);
                
                col += color(r, world, mat_list, light, ambient, depth, bgc);
            }
            col /= sn;
            Pixel pix;
            pix.SetClamp(255.0*(col[0]), 255.0*(col[1]), 255.0*col[2]);
            img->SetPixel(i, j, pix);
        }
    }

    img->Write("output/test.bmp");
    delete img;
    t2 = time(NULL);
    cout << t2-t1 << 's' << endl;
	return EXIT_SUCCESS;
}