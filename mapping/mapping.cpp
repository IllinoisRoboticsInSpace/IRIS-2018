#include "mapping.h"
#include <math.h>
#include <iostream>

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

cvec2f _get_angle(float x, float y){
    cvec2f angles;
    angles.x = ((half_dimX - x) / half_dimX) * half_fovX; //phi
    angles.y = pi2 - (((half_dimY - y) / half_dimY) * half_fovY);
    return angles;
}

cvec3f _get_cartesian(float distance, cvec2f azmuthPolar){
    //calculate phi and rho in spherical coord system
    //transform spherical system into cartesian system; assuming distance is approximately x
    float x = distance * cos(azmuthPolar.x);
    float y = distance * sin(azmuthPolar.x);
    float z = distance * tan(-azmuthPolar.y + M_PI / 2.0);
    return cvec3f(x, y, z);
}

iris_mapping::iris_mapping(void){
    _update_my_pos();
    width = dimX;
    height = dimY;
    for(size_t i = 0; i < 100; i++){
        vector<float> temp;
        true_map.push_back(temp);
        for(int j = 0; j < 100; j++){
            true_map[i].push_back(0);
        }
    }
}

iris_mapping::~iris_mapping(){
    //nothing
}

void iris_mapping::update_map(cv::Mat * cur_depth_frame){
    std::cout << "updating" << std::endl;
    _update_my_pos();
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            cvec2f azmuthPolar = _get_angle(x, y);
            //std::cout << "phi: " << azmuthPolar.x << std::endl;
            //std::cout << "rou: " << azmuthPolar.y << std::endl;
            float debug_p = cur_depth_frame->at<float>(x, y);
            float distance = _raw_2_millimeter(cur_depth_frame->at<float>(x, y));
            //std::cout << "distance calced: " << distance << std::endl;
            cvec3f cur_pixel = _get_cartesian(distance, azmuthPolar);
            //std::cout << "calced cartesian: " << cur_pixel << std::endl;
            cvec2i target_loc = calc_edge(distance, azmuthPolar.x);
            //calc_edge gonna return two -1s if it's out of bound (e.g. kinect is pointing towards sky)
            if(target_loc.x != -1){
                true_map[target_loc.y][target_loc.x] = cur_pixel.z();
            }
        }
    }
    std::cout << "updated" << std::endl;
}

vector<vector<float> > iris_mapping::return_entire_map(void){
    return true_map;
}

//TODO: may need to be fixed
cvec2i iris_mapping::calc_edge(float real_distance, float facing_direction_offset){
    //cart_coord.x is r; \theta is provided from localization
    int edge_x = cos(my_pos.theta_angle + facing_direction_offset) * real_distance / 5.0; //x offset vector in millimeter
    if(edge_x < 0 || edge_x >= 100){
        //std::cout << "overflowed with x: " << x << std::endl;
        return cvec2i(0, 0);
    }
    int edge_y = sin(my_pos.theta_angle + facing_direction_offset) * real_distance / 5.0;
    if(edge_y < 0 || edge_y >= 100){
        return cvec2i(0, 0);
    }
    return cvec2i(edge_x, edge_y);
}

void iris_mapping::_update_my_pos(void){
    pos new_pos;
    new_pos.x = 50;
    new_pos.y = 50;
    new_pos.theta_angle = 1;
    my_pos = new_pos;
}

float iris_mapping::return_height_of_a_point(int x, int y){
    return true_map[y][x];
}
