#include "ros/ros.h"
#include <armadillo2_bgu/OperationAction.h>
#include <rosplan_dispatch_msgs/ActionDispatch.h>
#include <rosplan_dispatch_msgs/ActionFeedback.h>
#include <actionlib/client/simple_action_client.h>
#include <boost/bind.hpp>
#include "diagnostic_msgs/KeyValue.h"
#include <rosplan_knowledge_msgs/DispatchActionToReflectTheEffect.h>

#include <iostream>
#include <string>
#include <typeinfo> 

using namespace std;

bool updateKB(std::string name, std::string effecttype,int paramSize,diagnostic_msgs::KeyValue parametersP[] );

typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;

ros::Publisher pub;
ros::ServiceClient KBclient;


bool publishStatus_succ = false;

void controlCB(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg) {
    publishStatus_succ = false;
    ROS_INFO("MASTER: Got the dispatch");
	armadillo2_bgu::OperationGoal goal;
    bool action = true;
    int id = msg->action_id;
    int paramSize = (msg->parameters).size();

    std::string name = msg->name;
    std::string effecttype = "effect";
    // It is important but we will bother about this effect-type later


    diagnostic_msgs::KeyValue parametersP[10];
    for(int i=0; i < paramSize; i++ )  {
        diagnostic_msgs::KeyValue pair;
        pair.key = msg->parameters[i].key;
        pair.value = msg->parameters[i].value;
        parametersP[i] = pair;
    }
    // End: Shashank 8.8.18

    Client client(msg->name, true);
    client.waitForServer();

	while (ros::ok() && action) {
		ros::Duration(2).sleep();
		std::cout<<"[master]: Sending goal to: "<<msg->name<<std::endl;
        client.sendGoal(goal);
        bool finished = client.waitForResult(ros::Duration());

  		if (finished){
            actionlib::SimpleClientGoalState state = client.getState();
            action = false;
		}
		else
			ROS_INFO("Action did not finish");
	}
	     std::cout<<"[master]: Got the result ["<<msg->name<<"]: "<<client.getResult()->res<<std::endl;

	rosplan_dispatch_msgs::ActionFeedback Remsg;
    Remsg.action_id = id;
    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED ) {
        Remsg.status = "action achieved";
        publishStatus_succ = true;
    }
    else
        Remsg.status = "action failed";

    if(updateKB(name,effecttype,paramSize,parametersP))
        pub.publish(Remsg);
}


bool updateKB(std::string name, std::string effecttype,int paramSize,diagnostic_msgs::KeyValue parametersP[] ){
    bool ans = true;
    rosplan_knowledge_msgs::DispatchActionToReflectTheEffect srv;
    KBclient.waitForExistence();

    if(publishStatus_succ) {
        srv.request.name = name;
        for (int i = 0; i < paramSize; i++) {
            srv.request.parameters.push_back(parametersP[i]);
        }
        srv.request.effecttype = effecttype;
        if (KBclient.call(srv)) {
            if(!(srv.response.status == "successfully updated!")) {
                ROS_INFO("\nFAILED!\n");
                ans = false;
            }
            else
                ROS_INFO("Updates the KB with latest action effect!");
        }
    }
    return ans;
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "master");

    ros::NodeHandle n;


    pub = n.advertise<rosplan_dispatch_msgs::ActionFeedback>("/kcl_rosplan/action_feedback", 1000);

    ros::Subscriber sub = n.subscribe("/kcl_rosplan/action_dispatch",10,controlCB);

    KBclient = n.serviceClient<rosplan_knowledge_msgs::DispatchActionToReflectTheEffect>("apply_action_effect");

    ROS_INFO("Master online");
    ros::AsyncSpinner spinner(2);
    spinner.start();


    ros::waitForShutdown();
    return 0;
}

