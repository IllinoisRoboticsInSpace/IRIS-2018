#include "mapping.h"

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

Vec3f _get_cartesian(int x, int y, float * cur_depth_frame){
    Vec3f ret_cartesian;
    //magic!
    float distance = _raw_2_millimeter(cur_depth_frame[y][x]);
    //calculate phi and rho in spherical coord system
    Vec2f azmuthPolar(get_angle(x, y));
    //transform spherical system into cartesian system; assuming distance is approximately x
    ret_cartesian.x = distance;;
    ret_cartesian.y = distance * math.tan(angle_x);
    ret_cartesian.z = distance * math.tan(-angle_y + math.pi / 2.0);
    return ret_cartesian;
}

void map::update_map(float * cur_depth_frame){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            Vec3f cur_pixel = _get_cartesian(x, y, cur_depth_frame);
            Vec2f target_loc = calc_edge(cur_pixel);
            //calc_edge gonna return two -1s if it's out of bound (e.g. pointing towards sky)
            if(target_loc.x != -1){
                true_map[target_loc.y][target_loc.x] = cur_pixel.z;
            }
        }
    }
}

float** map::return_entire_map(void){
    return true_map;
}

float map::return_height_of_a_point(int x, int y){
    return true_map[y][x];
}
