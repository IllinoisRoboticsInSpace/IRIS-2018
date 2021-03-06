#ifndef KINECT_PCL_
#define KINECT_PCL_

//#include "constant.h"
#include "Linear.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include "CoordSystemKinect.hpp"

#define PITCH 0.0
#define ROLL 0.0
#define maxViewDist 2500
#define minViewDist 470

std::vector<cvec3f> get_point_cloud(cv::Mat * cur_depth_frame);
std::vector<cvec3f> get_point_cloud(const uint16_t cur_depth_frame);

#endif
