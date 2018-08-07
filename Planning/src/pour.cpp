//
// Created by omer segal 2/8/2018
//

#include <ros/ros.h>
#include <control_msgs/GripperCommandAction.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ArmClient;
typedef actionlib::SimpleActionClient<control_msgs::GripperCommandAction> GripperClient;

void activeCb();
void feedbackCb(const control_msgs::FollowJointTrajectoryFeedbackConstPtr& feedback);
void doneCb(const actionlib::SimpleClientGoalState& state, const control_msgs::FollowJointTrajectoryResultConstPtr& result);

void buildGoal(control_msgs::FollowJointTrajectoryGoal &goal);

class ArmThrowNode {
private:
    ros::NodeHandle _nodeHandle;
    ros::AsyncSpinner _spinner;
    ros::Subscriber _feedbackListener;
    ArmClient _armClient;
    GripperClient _gripperClient;
    bool _needToOpen;
    bool _alreadyOpen;
    void doneCb(const actionlib::SimpleClientGoalState& state, const control_msgs::FollowJointTrajectoryResultConstPtr& result) {
        ROS_INFO("Finish in state [%s]", state.toString().c_str());
        _needToOpen = !_needToOpen;
    }
    void feedbackCb(const control_msgs::FollowJointTrajectoryFeedbackConstPtr& feedback) {
        if((-M_PI / 4)  < feedback->actual.positions[1] && _needToOpen && !_alreadyOpen) {
            _alreadyOpen = true;
            ROS_INFO("OPEN %f", feedback->actual.positions[1]);
            //openGripper();
        }
    }
    void activeCb() {
        ROS_INFO("Arm is moving");
    }

    bool setGripperCmd(double pos, double effort = 0) {
        control_msgs::GripperCommandGoal goal;
        goal.command.position = pos;
        goal.command.max_effort = effort;

        _gripperClient.sendGoal(goal);
        ROS_INFO("Gripper waiting");
        _gripperClient.waitForResult();

        return _gripperClient.getState() == actionlib::SimpleClientGoalState::SUCCEEDED;

    }

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

        point.time_from_start = ros::Duration(35.0);
        point.positions.resize(goal.trajectory.joint_names.size());
        point.positions[0] = 2.0;
        point.positions[1] = 1.5;
        point.positions[2] = -1.0;
        point.positions[3] = 0.0;
        point.positions[4] = 0.0;
        point.positions[5] = pos;

        point.velocities.resize(6);
        point.velocities[0] = 0.0;
        point.velocities[1] = 0.0;
        point.velocities[2] = 0.0;
        point.velocities[3] = 0.0;
        point.velocities[4] = 0.0;
        point.velocities[5] = 0.0;
        goal.trajectory.points.push_back(point);
        return goal;
    }

    bool setArmCmd(double pos) {
        _armClient.sendGoal(setGoalTja(pos),
                            boost::bind(&ArmThrowNode::doneCb, this, _1, _2),
                            boost::bind(&ArmThrowNode::activeCb, this),
                            ArmClient::SimpleFeedbackCallback());
        _armClient.waitForResult();
        return _armClient.getState() == actionlib::SimpleClientGoalState::SUCCEEDED;
    }

public:
    ArmThrowNode() : _nodeHandle(), _spinner(1),_armClient( "arm_trajectory_controller/follow_joint_trajectory"), _gripperClient("gripper_controller/gripper_cmd") {
        _feedbackListener = _nodeHandle.subscribe("arm_trajectory_controller/state", 10, &ArmThrowNode::feedbackCb, this);

        _spinner.start();
        _armClient.waitForServer();
        _gripperClient.waitForServer();

        _needToOpen = false;
        _alreadyOpen = false;


    }

    void run() {
        char choice;
        //do {
            //std::cout << "Please type 'g' to execute a throw command or 'q' to quit: ";
            //std::cin >> choice;
            //if(choice == 'g') throwProcess();
            //else if(choice == 'q') ROS_INFO("bye bye");
            //else ROS_WARN("Unknown syntax");
            throwProcess();
           // choice = 'q';

        //} while (ros::ok() && choice != 'q');
       // ros::shutdown();
    }

    void throwProcess() {
        int i = 7;
        while(ros::ok() & i != 0){
            std::cout<<"1 = open\n2 = close \n3 = pour"<<std::endl;
            std::cin>>i;
        //_alreadyOpen = false;
        if(i==1)
            openGripper();
        if(i==3)
            preThrowCmd();
        //ros::Duration(10.0).sleep();
        if(i == 2)
            closeGripper();
        //ros::Duration(3.0).sleep();
        //throwCmd();
        }

    }

    void openGripper() {
        setGripperCmd(0.08,0.4);
    }

    void closeGripper() {
        setGripperCmd(0.0,0.3);

    }


    void preThrowCmd() {
        ROS_INFO("POUR:Twisting");
        ros::Duration(5).sleep();
        setArmCmd(-1.3);
	ros::Duration(2).sleep();
	setArmCmd(-1.5);
	ros::Duration(2).sleep();
	setArmCmd(-1.7);
	ros::Duration(3).sleep();
	setArmCmd(0);

    }

    void throwCmd() {
        setArmCmd(1.0);
    }

};
void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as){
        ROS_INFO("POUR:Pouring");
        ArmThrowNode armThrowNode;
        armThrowNode.run();
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "pour");
    std::cout<<"Pour online"<<std::endl;
    ros::NodeHandle n;

    Server server(n, "pour", boost::bind(&execute, _1, &server), false);
    server.start();
    ros::AsyncSpinner spinner(4);
    spinner.start();
    ros::waitForShutdown();



    //armClient.sendGoal(goal, &doneCb, &activeCb, ArmClient::SimpleFeedbackCallback());

    return 0;
}

void buildGoal(control_msgs::FollowJointTrajectoryGoal &goal) {
    trajectory_msgs::JointTrajectoryPoint final;

    goal.trajectory.header.frame_id = "/map";
    goal.trajectory.header.stamp = ros::Time::now();
    goal.trajectory.joint_names.push_back("rotation1_joint");
    goal.trajectory.joint_names.push_back("shoulder1_joint");
    goal.trajectory.joint_names.push_back("shoulder2_joint");
    goal.trajectory.joint_names.push_back("rotation2_joint");
    goal.trajectory.joint_names.push_back("shoulder3_joint");
    goal.trajectory.joint_names.push_back("wrist_joint");

    final.time_from_start = ros::Duration(10.0);
    final.positions.resize(goal.trajectory.joint_names.size());
    final.positions[1] = 1.57;
    goal.trajectory.points.push_back(final);

}

void doneCb(const actionlib::SimpleClientGoalState& state, const control_msgs::FollowJointTrajectoryResultConstPtr& result) {
    ROS_INFO("Finish in state [%s]", state.toString().c_str());
//    ROS_INFO_STREAM((*result));
    ros::shutdown();
}

void feedbackCb(const control_msgs::FollowJointTrajectoryFeedbackConstPtr& feedback) {
    //ROS_INFO("%f", feedback->actual.positions[1]);
    static bool isOpen = false;
   if((-M_PI / 4)  < feedback->actual.positions[1] && !isOpen) {
       isOpen = true;
       ROS_INFO("OPEN %f", feedback->actual.positions[1]);
       GripperClient gripperClient("gripper_controller/gripper_cmd", true);
       gripperClient.waitForServer();
       control_msgs::GripperCommandGoal openGoal;
       openGoal.command.position = 0.14;
       gripperClient.sendGoal(openGoal);
       ROS_INFO("SEND TO GRIPPER");
       gripperClient.waitForResult();
       ROS_INFO("GRIPPER IS OPEN");

   }
}

void activeCb() {
    ROS_INFO("Arm is moving");
}
