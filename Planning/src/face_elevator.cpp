#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>



typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) 
{
	armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN face_elevator: Facing elevator");
    //put you'r code in here.
    static const std::string PLANNING_GROUP = "arm";
    moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

    const robot_state::JointModelGroup* joint_model_group =
        move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    ROS_INFO_NAMED("Pour", "Reference frame: %s", move_group.getPlanningFrame().c_str());
    ROS_INFO_NAMED("Pour", "End effector link: %s", move_group.getEndEffectorLink().c_str());

    move_group.setStartState(*move_group.getCurrentState());
    geometry_msgs::Pose another_pose;



/*
    geometry_msgs::PoseStamped target_pose1 = move_group.getCurrentPose();
    target_pose1.pose.orientation.y -= 0.03;
    //target_pose1.pose.position.x -= 0.03;
    //target_pose1.pose.position.z -= 0.03;
    move_group.setPoseTarget(target_pose1);

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
*/
    //move_group.execute(my_plan);

    

    if(true) {
        as->setSucceeded(result_);
    }
    else{ 
        as->setAborted();
    }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "face_elevator");
  std::cout<<"face_elevator online"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "face_elevator", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
