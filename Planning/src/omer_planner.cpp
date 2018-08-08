#include "ros/ros.h"
#include <armadillo2_bgu/ActionDispatch.h>
#include <armadillo2_bgu/ActionFeedback.h>

using namespace std;

bool picked;
ros::Publisher pub;

void controlCB(const armadillo2_bgu::ActionFeedback::ConstPtr& msg){
    armadillo2_bgu::ActionDispatch reMsg;
    if(msg->status == "action achieved"){
        reMsg.name = "pour";
        pub.publish(reMsg);
    }/*
    if(msg->status == "pick"){
        ROS_INFO("I have the cup");
        picked = true;
    }*/
        
}

int main(int argc, char** argv)
{

    ros::init(argc, argv, "omer_planner");
 
    ros::NodeHandle n;

    pub = n.advertise<armadillo2_bgu::ActionDispatch>("/kcl_rosplan/action_dispatch", 1000);

    armadillo2_bgu::ActionDispatch reMsg;


    picked = false;
    reMsg.name = "pour";


    ros::Subscriber sub = n.subscribe("/kcl_rosplan/action_feedback",10,controlCB);
    ros::Duration(3).sleep();
    ROS_INFO("Publishing!");
    pub.publish(reMsg);
    ros::Duration(7).sleep();

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();


    return 0;
 }



