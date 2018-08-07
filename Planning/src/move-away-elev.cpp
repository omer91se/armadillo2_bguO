#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) 
{
	armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN move-away-elev: Moving away from elevator");
    //put you'r code in here.
    ros::Duration(2.5).sleep();

    if(true) {
        as->setSucceeded(result_);
    }
    else{ 
        as->setAborted();
    }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "move-away-elev");
  std::cout<<"move-away-elev"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "move-away-elev", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
