#include "ros/ros.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/kinematics_metrics/kinematics_metrics.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <geometry_msgs/PointStamped.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

control_msgs::FollowJointTrajectoryGoal setGoalTja(double pos);



void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as)
{
    armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN give: Extending hand");
    //put you'r code in here.
    ros::Duration(2.5).sleep();


    moveit::planning_interface::MoveGroupInterface group("arm");

    group.setPlannerId("RRTConnectkConfigDefault");
    group.setPlanningTime(5);
    group.setPoseReferenceFrame("base_footprint");
    group.setStartStateToCurrentState();
    geometry_msgs::PoseStamped pose = group.getCurrentPose();
    //std::cout<<"x,y,z,"<<pose.pose.position.x<<","<<pose.pose.position.y<<","<<pose.pose.position.z<<std::endl;
   // std::cout<<"orient"<<pose.pose.orientation.x<<","<<pose.pose.orientation.y<<","<<pose.pose.orientation.z<<","<<pose.pose.orientation.w<<std::endl;
    pose.pose.position.x = 0.603619;
    pose.pose.position.y = 0.0940128;
    pose.pose.position.z = 1.0573;
    pose.pose.orientation.x = -0.000795239;
    pose.pose.orientation.y = -0.382703;
    pose.pose.orientation.z = -0.00179859;
    pose.pose.orientation.w = 0.923869;
    group.setPoseTarget(pose);
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(success)
        group.move();

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
      ros::init(argc, argv, "give");
      std::cout<<"give online"<<std::endl;
      ros::NodeHandle n;

      Server server(n,"give", boost::bind(&execute, _1, &server), false);
      server.start();

      ros::AsyncSpinner spinner(2);
      spinner.start();
      ros::waitForShutdown();

    //  moveit::planning_interface::MoveGroupInterface group("arm");
    //  ArmClient _armClient("arm_trajectory_controller/follow_joint_trajectory");

     // _armClient.sendGoal(setGoalTja(1.3),
      //                    boost::bind(&ArmThrowNode::doneCb, this, _1, _2),
     //                    boost::bind(&ArmThrowNode::activeCb, this),
     //                     ArmClient::SimpleFeedbackCallback());
      //_armClient.waitForResult();
      //sleep(5.0);

      /*
      group.setPlannerId("RRTConnectkConfigDefault");
      group.setPlanningTime(5);
      group.setPoseReferenceFrame("base_footprint");
      group.setStartStateToCurrentState();

      moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

   //   ros::Publisher display_publisher = n.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);
  //    moveit_msgs::DisplayTrajectory display_trajectory;

      ROS_INFO("Reference frame: %s", group.getPlanningFrame().c_str());

      ROS_INFO("Reference frame: %s", group.getEndEffectorLink().c_str());





/*
      //pose.pose.orientation.w = 0;
      //pose.pose.position.x = 0;
      //pose.pose.position.y = 0;
      //pose.pose.position.z = 1;
      //group.setPoseTarget(pose);
      //group.setPositionTarget(0.01,0.01,0.3, "gripper_link");

      geometry_msgs::PoseStamped pose = group.getCurrentPose();
    pose.pose.orientation.w -= 0.1;
  //  pose.pose.orientation.x += 0.700134;
  //  pose.pose.orientation.y -= 0.10;
  //  pose.pose.orientation.z -= 0.300308;
    //pose.pose.position.x = 0.5;
      group.setPoseTarget(pose);
      std::cout<<"pose x:"<<pose.pose.position.x<<std::endl;
      std::cout<<"pose y:"<<pose.pose.position.y<<std::endl;
      std::cout<<"pose z:"<<pose.pose.position.z<<std::endl;
      std::cout<<"orient w:"<<pose.pose.orientation.w<<std::endl;
      std::cout<<"orient x:"<<pose.pose.orientation.x<<std::endl;
      std::cout<<"orient y:"<<pose.pose.orientation.y<<std::endl;
      std::cout<<"orient z:"<<pose.pose.orientation.z<<std::endl;
      moveit::planning_interface::MoveGroupInterface::Plan my_plan;

      bool success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
      if(success)
          group.move();
      sleep(2.0);


      group.setPositionTarget(0.5,0.2,0.4, "gripper_link");
       pose = group.getPoseTarget();
       std::cout<<"pose x:"<<pose.pose.position.x<<std::endl;
       std::cout<<"pose y:"<<pose.pose.position.y<<std::endl;
      std::cout<<"pose z:"<<pose.pose.position.z<<std::endl;

      success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
      sleep(2.0);


      group.setPositionTarget(0.2,0.3,0.1, "gripper_link");
      pose = group.getPoseTarget();
      std::cout<<"pose x:"<<pose.pose.position.x<<std::endl;
      std::cout<<"pose y:"<<pose.pose.position.y<<std::endl;
      std::cout<<"pose z:"<<pose.pose.position.z<<std::endl;

     success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
     sleep(2.0);*/



      //Server server(n, "pick", boost::bind(&execute, _1, &server), false);
      //server.start();
      //ros::spin();

      return 0;
}
