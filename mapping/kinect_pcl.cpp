#include "kinect_pcl.h"
#include "CoordSystemKinect.hpp"

std::vector<cvec3f> get_point_cloud(cv::Mat * cur_depth_frame){
    //uint8_t* pixelPtr = (uint8_t*)cur_depth_frame->data;
    int point_count = cur_depth_frame->cols * cur_depth_frame->rows;
    std::vector<cvec3f> ret;
    for(int row = 0; row < cur_depth_frame->rows; row++){
        for(int col = 0; col < cur_depth_frame->cols; col++){
            //float distance = CSK::RawDepthToMilli(pixelPtr[CSK::GetIndex(col, row)]);
            float distance = CSK::RawDepthToMilli(cur_depth_frame->at<uchar>(col, row));
            if(distance > maxViewDist || distance < minViewDist){
                //ret.push_back(cvec3f(0, 0, 0));
                //if the data is not valid, drop it (or edit the code to put some sentinel values)
            } else {
                ret.push_back(CSK::GetCartCoord(col, row, distance));
            }
        }
    }
    return ret;
}
