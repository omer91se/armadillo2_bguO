#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as)
{
    int x,y,z;

    std::string x1 = ((goal->data).substr((goal->data).find('x')+1,(goal->data).find('y')));
    x = std::stoi (x1);

    std::string y1 = ((goal->data).substr((goal->data).find('y')+1,(goal->data).find('z')));
    y = std::stoi (y1);

    std::string z1 = ((goal->data).substr((goal->data).find('z')+1,(goal->data).find('w')));
    z = std::stoi (z1);
    std::cout<<"x:"<<x<<"y:"<<y<<"z:"<<z<<std::endl;

    armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN PICK: Actaully picking!");
    ros::Duration(2.5).sleep();
    if(true) //object found
        as->setSucceeded(result_);

    else //could not find object
        as->setAborted();

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "arm");
  std::cout<<"Arm online"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "arm", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
