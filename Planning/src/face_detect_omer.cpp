
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

#include <opencv2/opencv.hpp>
//#include <highgui.h>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
using namespace std;

void detectAndDisplay(CvPoint& face_point, cv::Rect& frame_data);
void onIncomingImage(const sensor_msgs::Image &msg);



void onIncomingImage(const sensor_msgs::Image &msg)
{
	//ros::Duration(5).sleep();
	cout<<"incomingImage"<<endl;
	cv::Mat frame_;
    cv_bridge::CvImagePtr img = cv_bridge::toCvCopy(msg);
    img->image.copyTo(frame_);
	cout<<"Writing image\n\n\n"<<endl;
	imwrite( "/home/armadillo2/omer.jpg", frame_ );

	CvPoint face_xy;
	cv::Rect frame;
	detectAndDisplay(face_xy,frame);
	
}
void detectAndDisplay(CvPoint& face_point, cv::Rect& frame_data){
	cout<<"Calling face reco.."<<endl;
    system("/home/armadillo2/Face_detection/openface/demos/classifier.py infer --multi /home/armadillo2/Face_detection/openface/generated-embeddings/classifier.pkl /home/armadillo2/omer.jpg > /home/armadillo2/s.txt");
    // open a file in read mode.
    cout<<"Reading the file."<<endl;
    ifstream is("/home/armadillo2/s.txt");

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
        while (getline(f, s, ' ')) {
            tokens.push_back(s);
        }
		//cut off the \n in all the tokens
        for(int i = 0; i<tokens.size();i++) {
            tokens[i] = tokens[i].substr(tokens[i].find('\n')+1,tokens[i].length() );
        }
        vector<string> names((tokens.size()/8)+1);

        int j = 0;
		string toFind = "Predicted";
        for(int i = 0; i<tokens.size(); i++) {
			cout << tokens[i] << endl;
            if (tokens[i].find(toFind) != std::string::npos) {
				cout<<"Found Predicted"<<endl;
                names[j] = tokens[i + 1];
                j++;
            }
        }
		cout<<"Names size:"<<names.size()<<endl;
        is.close();
        cout.write (buffer,length);
		int i =0;
        while(names[i]!=""){
            cout<<"Names"<<i<<": "<<names[i]<<endl;
			i++;
		}
        delete[] buffer;
	}
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "face_detect_omer");
    ros::NodeHandle nh("~");
	ros::Subscriber img_sub_ = nh.subscribe("/kinect2/qhd/image_color", 2,onIncomingImage); 
	ros::spin();
	
}
