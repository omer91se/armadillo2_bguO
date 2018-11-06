#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>
#include <actionlib/client/simple_action_client.h>
#include <armadillo2_bgu/SimpleTargetAction.h>
#include <geometry_msgs/PoseStamped.h>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;
typedef actionlib::SimpleActionClient<armadillo2_bgu::SimpleTargetAction> target_client_t;


float x;
float y;
float z;
float h;
float w;
bool gotXYZ = false;
std::string frameId;

//With Dan's code

void observeDoneCB(const actionlib::SimpleClientGoalState& state,
                   const armadillo2_bgu::OperationResultConstPtr& res){
    std::string x1 = ((res->res).substr((res->res).find('x')+1,(res->res).find('y')));
    x = std::stoi (x1);

    std::string y1 = (res->res).substr((res->res).find('y')+1,(res->res).find('z'));
    y = std::stoi (y1);

    std::string z1 = (res->res).substr((res->res).find('z')+1,(res->res).find('w'));
    z = std::stoi (z1);

    std::string w1 = ((res->res).substr((res->res).find('w')+1,(res->res).find('h')));
    w = std::stoi (w1);

    std::string h1 = ((res->res).substr((res->res).find('h')+1,(res->res).find('\0')));
    h = std::stoi (w1);
}

void poseCB(const geometry_msgs::PoseStamped::ConstPtr& msg) {

    x = msg->pose.position.x;
    y = msg->pose.position.y;
    z = msg->pose.position.z;
    //x = -0.112948;
    //y = 0.0866848;
    //z = 0.754;


    frameId = msg->header.frame_id;
    gotXYZ =true;
}

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) {

    armadillo2_bgu::OperationGoal sendGoal;
    ROS_INFO("[moveNear]: Starting move sequence");

    while (!gotXYZ)
        ros::Duration(0.5).sleep();

//      Client client("observe", true);
//      client.waitForServer();
//      ROS_INFO("Sending goal");
//      client.sendGoal(sendGoal,&observeDoneCB);
//      //client.sendGoal(sendGoal);
//      if(!client.waitForResult()){
//          ROS_INFO("Vision failed");
//      }
//      else{

    //armadillo2_bgu::OperationResultConstPtr res = client.getResult();

    target_client_t target_client("move", true);
    target_client.waitForServer();

    armadillo2_bgu::SimpleTargetGoal goal_target;
    goal_target.frame_id = "kinect2_rgb_optical_frame";
    goal_target.obj_name = "target";
    goal_target.x = x;
    goal_target.y = y;
    goal_target.z = z;

    target_client.sendGoal(goal_target);
    target_client.waitForResult();

    as->setSucceeded();
    ROS_INFO("[moveNear]: Done moving");

//    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "moveNear");
    ros::NodeHandle n;

    //for simple vision
    ros::Subscriber sub = n.subscribe("object_pose", 100, poseCB);

    Server server(n, "pick", boost::bind(&execute, _1, &server), false);
    server.start();

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();

    return 0;
}
