#include "libfreenect.hpp"
#include "kinect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>

using namespace cv;
using namespace std; 


/**KINECT parameters**/
const int maxViewDist = 2500;//millimeters
const int minViewDist = 470;//millimeters
const int gradientHalfSizeX = 80;
const int gradientHalfSizeY = 80;
const int historicHalfSizeX = 80;
const int historicSizeY = 180;
int sizeHTTPimage =0;
const int sizeGradientMap = sizeof(int8_t)*((gradientHalfSizeX*2)+1)*((gradientHalfSizeY*2)+1);
//csk namespace represents CoordinateSystemKinect
int sizeDepth = 0; //FREENECT_DEPTH_11BIT_SIZE;//we need this much space to represent the depth data
int sizeVideo = 0; //FREENECT_VIDEO_RGB_SIZE;//we need this much for the video data
/**ROS**/
const string topicName = "iris_obstacles";//this is the name the listener will look for
const string myNodeName = "iris_obstacles_talker";

int main(int argc, char **argv) {
	bool die(false);
	string filename("snapshot");
	string suffix(".png");
	int i_snap(0),iter(0);
	Mat depthMat(Size(640,480), CV_8UC1);
	Mat depthf (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
	Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
	//Mat historic(-historicHalfSizeX, historicHalfSizeX, 0, historicSizeY);
	
	// The next two lines must be changed as Freenect::Freenect
	// isn't a template but the method createDevice:
	// Freenect::Freenect<MyFreenectDevice> freenect;
	// MyFreenectDevice& device = freenect.createDevice(0);
	// by these two lines:
	
	Freenect::Freenect freenect;
	MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
	
	cvNamedWindow("rgb",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("depth",CV_WINDOW_AUTOSIZE);
	device.startVideo();
	device.startDepth();
	while (!die) {

		device.getVideo(rgbMat);
		device.getDepth(depthMat);
		cv::imshow("rgb", rgbMat);
		depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
		cv::imshow("depth",depthf);
		char k = cvWaitKey(5);
		if( k == 27 ){
			cvDestroyWindow("rgb");
			cvDestroyWindow("depth");
			break;
		}
		if( k == 8 ) {
			std::ostringstream file;
			file << filename << i_snap << suffix;
			cv::imwrite(file.str(),rgbMat);
			i_snap++;
		}
		if(iter >= 1000) break;
		iter++;
	}
	
	device.stopVideo();
	device.stopDepth();
	return 0;
}

