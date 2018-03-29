#ifndef MAPPING_H_
#define MAPPING_H_

#include "Vec.h"
#include <opencv2/opencv.hpp>

/* struct that represents current position of the robot */
struct pos{
    int x; //with respect to the map matrix
    int y;
    float theta_angle; //theta given by beacons
};

/* static helper functions */
float _raw_2_millimeter(float raw_depth);
Vec2f _get_angle(float x, float y);
Vec3f _get_cartesian(float distance, Vec2f azmuthPolar);

class map{
public:
    Map(); //default constructor
    ~Map(); //deconstructor that cleans pointer it uses (technically we never need it)
    void update_map(cv::Mat * cur_depth_frame);
    float** return_entire_map(void);
    float return_height_of_a_point(int x, int y);
    Vec2i calc_edge(float real_distance, float facing_direction_offset);
private:
    //float[100][100] true_map;
    /* private member variables */
    float ** true_map;
    int width;
    int height;
    pos my_pos;
    float cur_angle; //in radians
    /* private helper functions */
    void _update_my_pos(void);
};

#endif