#include "ros/ros.h"
#include <armadillo2_bgu/OperationAction.h>
#include <armadillo2_bgu/ActionDispatch.h>
#include <armadillo2_bgu/ActionFeedback.h>
#include <actionlib/client/simple_action_client.h>
#include <boost/bind.hpp>

#include <iostream>
#include <string>

using namespace std;

typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;

ros::Publisher pub;

void controlCB(const armadillo2_bgu::ActionDispatch::ConstPtr& msg){
    ROS_INFO("MASTER: Starting");
	armadillo2_bgu::OperationGoal goal;
    bool action = true;

    Client client(msg->name, true);
    client.waitForServer();

	while (ros::ok() && action){
		ros::Duration(2).sleep();
        client.sendGoal(goal);
        bool finished = client.waitForResult(ros::Duration(30.0));
		 
  		if (finished){
            actionlib::SimpleClientGoalState state = client.getState();
            action = false;
		}
		else
			ROS_INFO("Action did not finish before the time out.");
  		
		
	}

	armadillo2_bgu::ActionFeedback Remsg;
	//if(!client.getState())
   // cout<<"status:"<<client.getState().toString().c_str()<<endl;
   // Remsg.status = client.getState().toString().c_str();
    Remsg.action_id= msg->action_id;
    if(client.getResult())
        Remsg.status = "action achieved";
    else
        Remsg.status = "action failed";
	pub.publish(Remsg);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "master");
 
    ros::NodeHandle n;

     pub = n.advertise<armadillo2_bgu::ActionFeedback>("/kcl_rosplan/action_feedback", 1000);

    ros::Subscriber sub = n.subscribe("/kcl_rosplan/action_dispatch",10,controlCB);
    ROS_INFO("At master");
    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();
     //ros::spin();
    return 0;
}

