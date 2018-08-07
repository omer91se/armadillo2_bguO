#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) 
{
	armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN PICK: Observing...");
    //put you'r code in here.
    ros::Duration(2.5).sleep();

    result_.res = "x12y232z25";  //this is the format x(num)y(num)z(num)
    ROS_INFO("IN PICK: Object placed in: x:12, y:232, z:25\n");   // insert acctual x,y,z (not hardcoded)

    if(true) {//object found
        as->setSucceeded(result_);
    }
    else{ //could not find object
        as->setAborted();
    }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "observe");
  std::cout<<"Observe online"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "observe", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
