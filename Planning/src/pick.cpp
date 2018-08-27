#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>
#include <armadillo2_bgu/SimplePickAction.h>
#include <actionlib/client/simple_action_client.h>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

//not needed with dan's code
#include <geometry_msgs/PoseStamped.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;
typedef actionlib::SimpleActionClient<armadillo2_bgu::SimplePickAction> Client_pick;


float x;
float y;
float z;
float w;
float h;
bool gotXYZ = false;
std::string frameId;

//with Dan's vision
void observeDoneCB(const actionlib::SimpleClientGoalState& state,
        const armadillo2_bgu::OperationResultConstPtr& res){
    std::string x1 = ((res->res).substr((res->res).find('x')+1,(res->res).find('y')));
    x = std::stof (x1);

    std::string y1 = (res->res).substr((res->res).find('y')+1,(res->res).find('z'));
    y = std::stof (y1);

    std::string z1 = (res->res).substr((res->res).find('z')+1,(res->res).find('w'));
    z = std::stof (z1);

    std::string w1 = ((res->res).substr((res->res).find('w')+1,(res->res).find('h')));
    w = std::stof (w1);

    std::string h1 = ((res->res).substr((res->res).find('h')+1,(res->res).find('\0')));
    h = std::stof (w1);

    std::cout<<"[Pick]: (x,y,z): "<<x<<","<<y<<","<<z<<std::endl;

    w = 0.022;
    h = 0.1;
}

//simple vision
void poseCB(const geometry_msgs::PoseStamped::ConstPtr& msg) {

    x = msg->pose.position.x;
    y = msg->pose.position.y;
    z = msg->pose.position.z+0.035;
   //x = -0.112948;
   //y = 0.0866848;
   //z = 0.754;

    w = 0.022;
    h = 0.1;
    frameId = msg->header.frame_id;
    gotXYZ =true;
}

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as){

    armadillo2_bgu::OperationGoal sendGoal;
    ROS_INFO("Starting pick sequence");

   //with Dan's vision
    Client client("observe", true);
    client.waitForServer();
    client.sendGoal(sendGoal, &observeDoneCB);

    //client.sendGoal(sendGoal);
    if(!client.waitForResult()){
        ROS_INFO("Vision failed");
    }
    else {


//Simple vision
        //  while (!gotXYZ)
        //     ros::Duration(0.5).sleep();

        ROS_INFO("[pick]: Calling arm_pick");
        Client_pick client_pick("arm_pick", true);
        client_pick.waitForServer();

        armadillo2_bgu::SimplePickGoal goal_pick;

        goal_pick.frame_id = "kinect2_rgb_optical_frame";
        goal_pick.obj_name = "target";
        goal_pick.x = x;
        goal_pick.y = y;
        goal_pick.z = z;

        goal_pick.h = h;
        goal_pick.w = w;
        // goal.= res->res;
        client_pick.sendGoal(goal_pick);


        if (!client_pick.waitForResult()) {
            //what to do when pick fail
            ROS_INFO("Failed to pick");
        }
        ros::Duration(3).sleep();


        as->setSucceeded();
        ROS_INFO("Done Picking");
    }
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "pick");
  std::cout<<"Pick online"<<std::endl;
  ros::NodeHandle n;

  //for simple vision
 // ros::Subscriber sub = n.subscribe("object_pose", 100, poseCB);

  Server server(n, "pick", boost::bind(&execute, _1, &server), false);
  server.start();

  ros::AsyncSpinner spinner(2);
  spinner.start();
  ros::waitForShutdown();

  return 0;
}
