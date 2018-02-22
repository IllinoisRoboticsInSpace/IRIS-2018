#ifndef MAPPING_H_
#define MAPPING_H_

#include <math.h>
#include <opencv2/opencv.hpp>

float _raw_2_millimeter(float raw_depth);
float _get_angle(float x, float y);

class map{
    public:
        Map(void); //default constructor
        void update_map(float * cur_depth_frame);
        float** return_entire_map(void);
        float return_height_of_a_point(int x, int y);
        int calc_edge(Vec3f cart_coord);
        ~Map(void); //deconstructor that cleans pointer it uses (technically we don't need it)
    private:
        //float[100][100] true_map;
        float ** true_map;
        int width;
        int height;
};

#endif
