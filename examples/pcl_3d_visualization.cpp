#include "libfreenect.hpp"
#include "kinectDevice.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include "kinect_pcl.h"
#include "Linear.hpp"

using namespace std;

int main(void){
    /* start initialization; declera variables */
    bool die(false);
    string filename("snapshot");
    string suffix(".png");
    int i_snap(0),iter(0);
    cv::Mat depthMat(Size(640,480), CV_8UC1);
    cv::Mat depthf(Size(640,480),CV_8UC1);
    cv::Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
    cv::Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
    Freenect::Freenect freenect;
    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
    /* end kinect initialization */
    /* start map instance initialization */
    //iris_mapping my_mapping;
    int counter = 0;
    while(!die){
        //device.getVideo(rgbMat);
        device.getDepth(depthMat);
        device.getVideo(rgbMat);
        cv::imwrite("rgb_output.png", rgbMat);
        cv::imwrite("depth_output.png", depthMat);
        //depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        vector<cvec3f> cur_pcl = get_point_cloud(&(depthMat));
        for(size_t i = 0; i < cur_pcl.size(); i++){
            std::cout << cur_pcl[i].x << " ";
            std::cout << cur_pcl[i].y << " ";
            std::cout << cur_pcl[i].z;
            std::cout << std::endl;
        }
        die = true;
    }
    return 0;
}
