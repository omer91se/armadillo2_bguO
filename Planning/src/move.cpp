#include "ros/ros.h"
#include <armadillo2_bgu/OperationAction.h>
#include <actionlib/server/simple_action_server.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) {
    ROS_INFO("Moving");
    ros::Duration(0.7).sleep();
    ROS_INFO("5...");
    ros::Duration(0.7).sleep();
    ROS_INFO("4...");
    ros::Duration(0.7).sleep();
    ROS_INFO("3...");
    ros::Duration(0.7).sleep();
    ROS_INFO("2...");
    ros::Duration(0.7).sleep();
    ROS_INFO("1...");
    ros::Duration(0.7).sleep();
    ROS_INFO("Moved successfully\n");

    as->setSucceeded();
}




int main(int argc, char** argv)
{
  ros::init(argc, argv, "move");
  std::cout<<"Move online"<<std::endl;

  ros::NodeHandle n;
  Server server(n, "move", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
