#include "ros/ros.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/kinematics_metrics/kinematics_metrics.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <std_srvs/SetBool.h>
#include <moveit_msgs/PickupAction.h>
#include <moveit_msgs/PlaceAction.h>
#include <geometry_msgs/PointStamped.h>
#include <control_msgs/GripperCommandAction.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>

control_msgs::FollowJointTrajectoryGoal setGoalTja(double pos);

typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ArmClient;

control_msgs::FollowJointTrajectoryGoal setGoalTja(double pos) {
     control_msgs::FollowJointTrajectoryGoal goal;
     trajectory_msgs::JointTrajectoryPoint point;

     goal.trajectory.header.frame_id = "/map";
     goal.trajectory.header.stamp = ros::Time::now();
     goal.trajectory.joint_names.push_back("rotation1_joint");
     goal.trajectory.joint_names.push_back("shoulder1_joint");
     goal.trajectory.joint_names.push_back("shoulder2_joint");
     goal.trajectory.joint_names.push_back("rotation2_joint");
     goal.trajectory.joint_names.push_back("shoulder3_joint");
     goal.trajectory.joint_names.push_back("wrist_joint");

     point.time_from_start = ros::Duration(2.0);
     point.positions.resize(goal.trajectory.joint_names.size());
     point.positions[5] = pos;
     goal.trajectory.points.push_back(point);
     return goal;
 }

int main(int argc, char** argv)
{
      ros::init(argc, argv, "pick");
      std::cout<<"Pour online"<<std::endl;
      ros::NodeHandle n;
      ros::AsyncSpinner spinner(2);
      spinner.start();

    //  moveit::planning_interface::MoveGroupInterface group("arm");
      ArmClient _armClient("arm_trajectory_controller/follow_joint_trajectory");

     // _armClient.sendGoal(setGoalTja(1.3),
      //                    boost::bind(&ArmThrowNode::doneCb, this, _1, _2),
     //                    boost::bind(&ArmThrowNode::activeCb, this),
     //                     ArmClient::SimpleFeedbackCallback());
      _armClient.waitForResult();
      sleep(5.0);

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
