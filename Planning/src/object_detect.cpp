#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h> 
#include <boost/bind.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include "std_msgs/String.h"


#include <opencv2/opencv.hpp>
//#include <highgui.h>
#include <cv_bridge/cv_bridge.h>
using namespace std;
typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
ros::Publisher * pPub;


void onIncomingImage(const sensor_msgs::Image &msg){
	char s;
	cv::Mat frame_;
    	cv_bridge::CvImagePtr img = cv_bridge::toCvCopy(msg);
   	img->image.copyTo(frame_);
	cout<<"Writing image\n\n\n"<<endl;

	imwrite( "/home/armadillo2/catkin_ws/src/armadillo2_bgu/Speech/object_recognition/src/test.jpg", frame_ );
	
	system("python2 /home/armadillo2/catkin_ws/src/armadillo2_bgu/Speech/object_recognition/src/run_object_rec.py");
	
	cout<<"Reading the file."<<endl;
  	ifstream is("/home/armadillo2/catkin_ws/src/armadillo2_bgu/Speech/object_recognition/src/object_rec_result.txt");
    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        // allocate memory:
        char * buffer = new char [length];

        // read data as a block:
        is.read (buffer,length);

        vector<string> tokens{};
        istringstream f(buffer);
        string s;
		//split to tokens by spacebars and push into a vector.
        while (getline(f, s, '\n')) 
            tokens.push_back(s);

 
		//cut off the \n in all the tokens
	 std_msgs::String msg;
	msg.data= "";
        for(int i = 0; i<5;i++) 
	    msg.data =  msg.data + "-"+ tokens[i];
       
        is.close();
        cout.write (buffer,length);

        delete[] buffer;
	cout<<"msg:"<<msg.data<<endl;
	while(ros::ok()){
		ros::Duration(0.5).sleep();
		cout<<"in ros::ok()"<<endl;
		pPub->publish(msg);
	}
    }



}
int main(int argc, char** argv)
{	
	ros::init(argc, argv, "object_detect");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<std_msgs::String>("object_detect", 10);
	pPub = &pub;
	ros::Subscriber img_sub_ = n.subscribe("/kinect2/hd/image_color", 2,onIncomingImage);
	ros::spin();
	return 0;
}
