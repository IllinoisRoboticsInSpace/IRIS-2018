#include "libfreenect.hpp"
#include "kinectDevice.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include "kinect_pcl.h"
#include "Linear.hpp"

using namespace std;

float get_2d_vector_min(const vector<vector<float> > & some_vector){
    float ret = some_vector[0][0];
    for(size_t row = 0; row < some_vector.size(); row++){
        for(size_t col = 0; col < some_vector[row].size(); col++){
            if(some_vector[row][col] < ret){
                ret = some_vector[row][col];
            }
        }
    }
    return ret;
}

float get_2d_vector_max(const vector<vector<float> > & some_vector){
    float ret = some_vector[0][0];
    for(size_t row = 0; row < some_vector.size(); row++){
        for(size_t col = 0; col < some_vector[row].size(); col++){
            if(some_vector[row][col] > ret){
                ret = some_vector[row][col];
            }
        }
    }
    return ret;
}

vector<float> get_uchar_cut(float lowest_value, float highest_value){
    vector<float> ret;
    ret.push_back(lowest_value);
    float interval = (highest_value - lowest_value) / 255;
    float cur_count = lowest_value + interval;
    for(int i = 0; i < 254; i++){
        ret.push_back(cur_count);
        cur_count += interval;
    }
    ret.push_back(highest_value);
    return ret;
}

uchar get_pos(const vector<float> & my_cut, float my_value){
    unsigned int ret = 0;
    while(ret < 255){
        if(my_value <= my_cut[ret]){
            return (uchar)ret;
        }
        ret++;
    }
    return (uchar)(255);
}

vector<vector<uchar> > normalize_2d_vector(const vector<vector<float> > & some_vector){
    float min_value = get_2d_vector_min(some_vector);
    float max_value = get_2d_vector_max(some_vector);
    //std::cout << "Min: " << min_value << std::endl;
    //std::cout << "Max: " << max_value << std::endl;
    vector<float> my_cut = get_uchar_cut(min_value, max_value); //length is 256
    //std::cout << "cur got" << std::endl;
    vector<vector<uchar> > ret;
    for(int i = 0; i < some_vector.size(); i++){
        vector<uchar> temp;
        ret.push_back(temp);
        for(int j = 0; j < some_vector[i].size(); j++){
            uchar temp = get_pos(my_cut, some_vector[i][j]);
            //std::cout << "got: " << (int)temp << std::endl;
            ret[i].push_back(temp);
        }
    }
    return ret;
}

template<class T>
void print_2d_vector(const vector<vector<T> > & some_vector){
    cout.flush();
    std::cout << "===========================================" << std::endl;
    for(size_t row = 0; row < some_vector.size(); row++){
        for(size_t col = 0; col < some_vector[row].size(); col++){
            cout << some_vector[row][col] << " ";
        }
        cout << endl;
    }
}

//template <class T>
void print_vector_head(const vector<cvec3f> & some_vector){
    std::cout << "============================================" << std::endl;
    for(size_t i = 0; i < 10; i++){
        std::cout << some_vector[i].x << " " << some_vector[i].y << " " << some_vector[i].z << std::endl;
    }
    std::cout << std::endl;
}

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
        //depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        vector<cvec3f> cur_pcl = get_point_cloud(&(depthMat));
        print_vector_head(cur_pcl);
        break;
    }
    return 0;
}
