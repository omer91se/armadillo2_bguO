#include "ros/ros.h"
#include <armadillo2_bgu/ActionDispatch.h>
#include <armadillo2_bgu/ActionFeedback.h>

using namespace std;

bool picked;
ros::Publisher pub;

void controlCB(const armadillo2_bgu::ActionFeedback::ConstPtr& msg){
    armadillo2_bgu::ActionDispatch reMsg;
    if(msg->status == "action achieved"){
        ROS_INFO("Action2!");
        reMsg.name = "moveNear";
        pub.publish(reMsg);
    }
   /* if(msg->status == "pour"){
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
    reMsg.name = "moveNear";


    ros::Subscriber sub = n.subscribe("/kcl_rosplan/action_feedback",10,controlCB);
    ROS_INFO("Publishing!");
    ros::Duration(5).sleep();
    pub.publish(reMsg);
    ROS_INFO("done Publishing!");
    ros::Duration(7).sleep();

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();


    return 0;
 }



