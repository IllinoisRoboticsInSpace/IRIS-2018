#include "libfreenect.hpp"
#include "kinectDevice.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include "mapping.h"

using namespace cv;
using namespace std;

template<class T>
void print_2d_vector(const vector<vector<T> > & some_vector){
    for(size_t row = 0; row < some_vector.size(); row++){
        for(size_t col = 0; col < some_vector[row].size(); col++){
            cout << some_vector[row][col] << " ";
        }
        cout << endl;
    }
}

int main(void){
    /* start initialization; declera variables */
    bool die(false);
    string filename("snapshot");
    string suffix(".png");
    int i_snap(0),iter(0);
    Mat depthMat(Size(640,480), CV_8UC1);
    Mat depthf (Size(640,480),CV_8UC1);
    Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
    Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
    Freenect::Freenect freenect;
    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
    /* end kinect initialization */
    /* start map instance initialization */
    map my_mapping;
    int counter = 0;
    while(!die){
        //device.getVideo(rgbMat);
        device.getDepth(depthMat);
        depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        map.update_map(&(depthf));
        vector<vector<float> > cur_map = map.return_entire_map();
        if(counter % 100 == 0){
            print_2d_vector(cur_map);
        }
        if(iter >= 1000){
            //die!
            die = true;
        }
        counter++;
        iter++;
    }
    return 0;
}
