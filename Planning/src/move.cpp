
/*******************************************************************************
* Copyright (c) 2018, RoboTICan, LTD.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of RoboTICan nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
/* Author: Elchay Rauper*/

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PointStamped.h>
#include <std_srvs/Trigger.h>
#include <tf/transform_listener.h>
#include <actionlib/client/simple_action_client.h>
#include <armadillo2_bgu/SimpleTargetAction.h>

#include <armadillo2_bgu/OperationAction.h>
#include <actionlib/server/simple_action_server.h>
#include <visualization_msgs/Marker.h>


typedef actionlib::SimpleActionServer<armadillo2_bgu::SimpleTargetAction> target_server_t;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;


tf::TransformListener *listener_ptr;

//move_base_msgs::MoveBaseGoal get_pre_pick_pose(geometry_msgs::Point point);
//bool base_cmd(move_base_msgs::MoveBaseGoal goal);

bool build_goal(move_base_msgs::MoveBaseGoal &target, geometry_msgs::Pose &object);
void markRVIZ(move_base_msgs::MoveBaseGoal target);

MoveBaseClient *moveBaseClient_ptr;

bool moving=false;
std::string object_name;

float x;
float y;
float z;
bool gotXYZ = false;
std::string frameId;
ros::Publisher marker_pub;

//With Dan's code.
/*
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
    w = std::stoi (w1);
}*/


/*move_base_msgs::MoveBaseGoal get_pre_pick_pose(geometry_msgs::Point point) {
    tf::Transform dest_transform;
    move_base_msgs::MoveBaseGoal goal;
    try{

        tf::StampedTransform transform_base;
        try {
            listener_ptr->lookupTransform(frameId, "base_link", ros::Time(0), transform_base);

        }
        catch (tf::TransformException ex){
            ROS_ERROR("%s",ex.what());
            return goal;
        }
        tf::Vector3 v_obj(point.x,point.y,point.z);

        tf::Vector3 v_base =transform_base.getOrigin();

        tf::Vector3 v=v_obj-v_base;
        double yaw=atan2(v.y(),v.x());
        double away=base_distance_from_object/sqrt(v.x()*v.x()+v.y()*v.y());
        tf::Vector3 dest=v_base+v*(1-away);

        dest_transform.setOrigin( dest );
        dest.setZ(0);
        tf::Quaternion q;
        q.setRPY(0.0, 0, yaw);
        dest_transform.setRotation(q);

        //std::cout<< map_object_pose.pose.position<<std::endl;

    }

    catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        return goal;
    }

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x=dest_transform.getOrigin().x();
    goal.target_pose.pose.position.y=dest_transform.getOrigin().y();
    goal.target_pose.pose.position.z=0;
    goal.target_pose.pose.orientation.x=dest_transform.getRotation().x();
    goal.target_pose.pose.orientation.y=dest_transform.getRotation().y();
    goal.target_pose.pose.orientation.z=dest_transform.getRotation().z();
    goal.target_pose.pose.orientation.w=dest_transform.getRotation().w();
    return goal;
}*/


/*
bool drive_go_cb(const armadillo2_bgu::SimpleTargetGoalConstPtr& goal, target_server_t* as) {
    //Client client();
   // ros::NodeHandle n;

   std::cout<<"goal->x: "<<goal->x<<std::endl;
    std::cout<<"FrameID: "<<goal->frame_id<<std::endl;

    geometry_msgs::Point point;
    frameId = goal->frame_id;
    point.x = goal->x;
    point.y = goal->y;
    point.z = goal->z;

    move_base_msgs::MoveBaseGoal move_goal = get_pre_pick_pose(point);

    if (base_cmd(move_goal)) {
        ROS_INFO("Reached position");
        as->setSucceeded();
    }
    else {
        ROS_INFO("[move]: Couldn't move to goal");
        as->setAborted();
    }

}*/



/*call that this way:
move_base_msgs::MoveBaseGoal goal= get_pre_pick_pose(pose position);


if (base_cmd(goal)) {
ROS_INFO("Reached position");
res.message="Reached pre-picking position";
res.success=true;
}
 */
/*
bool base_cmd(move_base_msgs::MoveBaseGoal goal) {

    ROS_INFO("[%s]: Sending goal", ros::this_node::getName().c_str());

    moveBaseClient_ptr->sendGoal(goal);
    moveBaseClient_ptr->waitForResult();

    if(moveBaseClient_ptr->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
        return true;
    }
    else {
        ROS_ERROR("[%s]: Navigation failed ", ros::this_node::getName().c_str());
        return false;
    }
}
*/
void markRVIZ(move_base_msgs::MoveBaseGoal target, int id,std::string frame_id){
    std::cout<<"goal->x:"<<target.target_pose.pose.position.x<<"\ngoal->y"<<target.target_pose.pose.position.y<<"\ngoal->z:"<<target.target_pose.pose.position.z<<std::endl;
    uint32_t shape = visualization_msgs::Marker::ARROW;
    bool saw_arrow = false;
    char saw;
    ros::Rate r(1);

    while(!saw_arrow) {
        visualization_msgs::Marker marker;
        marker.header.frame_id = frame_id;
        marker.header.stamp = ros::Time::now();
        marker.ns = "basic_shapes";
        marker.id = 0;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;

        //position
        marker.pose.position.x = target.target_pose.pose.position.x;
        marker.pose.position.y = target.target_pose.pose.position.y;
        // marker.pose.position.z = target.target_pose.pose.position.z;
        marker.pose.orientation.x = target.target_pose.pose.orientation.x;
        marker.pose.orientation.y = target.target_pose.pose.orientation.y;
        marker.pose.orientation.z = target.target_pose.pose.orientation.z;
        marker.pose.orientation.w = target.target_pose.pose.orientation.w;

        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker.scale.x = 0.3;
        marker.scale.y = 0.3;
        marker.scale.z = 0.2;

        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 0.0f +id;
        marker.color.g = 1.0f;
        marker.color.b = 0.0f;
        marker.color.a = 1.0;

        marker.lifetime = ros::Duration();

        while (marker_pub.getNumSubscribers() < 1) {
            if (!ros::ok()) {
                return;
            }
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            sleep(1);
        }
        marker_pub.publish(marker);
        std::cout<<"Did you see the arrow?[y/n] "<<std::endl;
        std::cin>>saw;
        if (saw == 'y')
            saw_arrow = true;

        r.sleep();
    }

}

bool build_goal(move_base_msgs::MoveBaseGoal &target, geometry_msgs::Pose &object){

    std::cout<<"object.position.x:"<<object.position.x<<"\ngoal->y"<<object.position.y<<"\ngoal->z:"<<object.position.z<<std::endl;
    tf::TransformListener _tf_listener;

    _tf_listener.waitForTransform(frameId, "base_link", ros::Time(0), ros::Duration(5.0));
    geometry_msgs::PointStamped origin_goal;

    origin_goal.header.frame_id = frameId;

    tf::StampedTransform transform;

    _tf_listener.lookupTransform("base_link", "base_link", ros::Time(0), transform);
    origin_goal.point.x = object.position.x;
    origin_goal.point.y = object.position.y;
    origin_goal.point.z = object.position.z;
    std::cout<<"frameId: "<<frameId<<std::endl;


    geometry_msgs::PointStamped transformed_goal;
    _tf_listener.transformPoint("/base_link", origin_goal, transformed_goal);


    // build goal and return
    target.target_pose.header.frame_id = "base_link";

    target.target_pose.header.stamp = ros::Time::now();

    target.target_pose.pose.position.x = transformed_goal.point.x-0.4;
    target.target_pose.pose.position.y = transformed_goal.point.y;
    //std::cout<<"target.target_pose.pose.position.z: "<<target.target_pose.pose.position.z;
    //target.target_pose.pose.position.z = transformed_goal.point.z;


//    tf::Vector3 v_obj(object.position.x,object.position.y,object.position.z);
//    tf::Vector3 v_base =transform.getOrigin();
//    tf::Vector3 v=v_obj-v_base;
//    double yaw=atan2(v.y(),v.x());
//
//    tf::Quaternion q;
//    q.setRPY(0.0, 0, yaw);
//    transform.setRotation(q);
//    target.target_pose.pose.orientation.x = transform.getRotation().x();
//    target.target_pose.pose.orientation.y = transform.getRotation().y();
//    target.target_pose.pose.orientation.z = transform.getRotation().z();
//    target.target_pose.pose.orientation.w = transform.getRotation().w();
    tf::Quaternion q = tf::createQuaternionFromRPY(0, 0, 0);
    q.normalize();


    target.target_pose.pose.orientation.x = q.x();
    target.target_pose.pose.orientation.y = q.y();
    target.target_pose.pose.orientation.z = q.z();
    target.target_pose.pose.orientation.w = q.w();
    markRVIZ(target,0,target.target_pose.header.frame_id);

    return true;
}

void driveCB(const armadillo2_bgu::SimpleTargetGoalConstPtr& goal, target_server_t* as){
    ROS_INFO("[move]: In driveCB");
    move_base_msgs::MoveBaseGoal MB_goal;

    geometry_msgs::Pose point;
    frameId = goal->frame_id;
    point.position.x = goal->x;
    point.position.y = goal->y;
    point.position.z = goal->z;
    if(!build_goal(MB_goal,point)){
        ROS_INFO("couldn't create goal.");
        as->setAborted();
        return;
    }
    MoveBaseClient _mb_client("move_base", true);

//    //for saftey
//    char ready;
//    std::cout<<"[move]: enter any char to move"<<std::endl;
//    std::cin>>ready;

    _mb_client.sendGoalAndWait(MB_goal);
    if(_mb_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        as->setSucceeded();
    else
        as->setAborted();


}
int main(int argc, char **argv) {

    ros::init(argc, argv, "move");
    ros::NodeHandle n;

    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    MoveBaseClient moveBaseClient("move_base", true);
    //wait for the action server to come up
    while(!moveBaseClient.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    moveBaseClient_ptr=&moveBaseClient;
    target_server_t server(n,"move",boost::bind(&driveCB, _1, &server), false);
    server.start();
    tf::TransformListener listener;
    listener_ptr=&listener;

    ros::spin();

    return 0;
}

