#include "ros/ros.h"
#include "armadillo2_bgu/ReturnJointStates.h"

bool add(beginner_tutorials::AddTwoInts::Request  &req,
         beginner_tutorials::AddTwoInts::Response &res)
{
  res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "JointState_service");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("JointState", add);
  ROS_INFO("Getting joint states");
  ros::spin();

  return 0;
}
