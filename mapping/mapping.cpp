#include "mapping.h"
#include <math.h>

int dimX = 640;
int dimY = 480;
int half_dimX = dimX / 2;
int half_dimY = dimY / 2;
float degree_2_rad = M_PI / 180.0;
float pi2 = M_PI / 2.0;
float fovX = 57 * degree_2_rad;
float fovY = 43 * degree_2_rad;
float half_fovX = fovX / 2.0;
float half_fovY = fovY / 2.0;

float _raw_2_millimeter(float raw_depth){
    float mystery1 = -0.0030711016; //copied from IRIS-2017; it said these magic num were obtained from the internet
    float mystery2 = 3.3309495161;
    return (1.0 / (raw_depth * mystery1 + mystery2)) * 1000;
}

Vec2f _get_angle(float x, float y){
    Vec2f angles;
    angles.x = ((half_dimX - x) / half_dimX) * half_fovX;
    angles.y = pi2 - (((half_dimY - y) / half_dimY) * half_fovY);
    return angles;
}

Vec3f _get_cartesian(float distance, Vec2f azmuthPolar){
    //calculate phi and rho in spherical coord system
    //transform spherical system into cartesian system; assuming distance is approximately x
    float x = distance * cos(azmuthPolar.x);
    float y = distance * sin(azmuthPolar.x);
    float z = distance * tan(-azmuthPolar.y + M_PI / 2.0);
    return Vec3f(x, y, z);
}

map::map(){
    //default constructor. Take constants from constant.h
}

void map::update_map(cv::Mat * cur_depth_frame){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            Vec2f azmuthPolar(get_angle(x, y));
            float distance = _raw_2_millimeter(cur_depth_frame->at<int>(x, y));
            Vec3f cur_pixel = _get_cartesian(distance, azmuthPolar);
            Vec2i target_loc = calc_edge(distance, azmuthPolar.x);
            //calc_edge gonna return two -1s if it's out of bound (e.g. pointing towards sky)
            if(target_loc.x != -1){
                true_map[target_loc.y][target_loc.x] = cur_pixel.z();
            }
        }
    }
}

float** map::return_entire_map(void){
    return true_map;
}

Vec2i map::calc_edge(float real_distance, float facing_direction_offset){
    _update_my_pos();
    //cart_coord.x is r; \theta is provided from localization
    int x = cos(my_pos.theta_angle + facing_direction_offset) * real_distance; //x offset vector in millimeter
    int y = sin(my_pos.theta_angle + facing_direction_offset) * real_distance;
    return Vec2i(x, y);
}

void map::_update_my_pos(void){
    secret_non_implemented_func_that_use_object_detection_2_get_cur_pos();
}

float map::return_height_of_a_point(int x, int y){
    return true_map[y][x];
}