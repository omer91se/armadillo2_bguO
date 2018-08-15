
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

typedef actionlib::SimpleActionServer<armadillo2_bgu::SimpleTargetAction> target_server_t;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

tf::TransformListener *listener_ptr;

move_base_msgs::MoveBaseGoal get_pre_pick_pose();
bool base_cmd(move_base_msgs::MoveBaseGoal goal);

MoveBaseClient *moveBaseClient_ptr;

bool moving=false;
std::string object_name;
double base_distance_from_object=0.55;

float x;
float y;
float z;
float w;
float h;


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
}



move_base_msgs::MoveBaseGoal get_pre_pick_pose(geometry_msgs::Point point) {
    tf::Transform dest_transform;
    move_base_msgs::MoveBaseGoal goal;
    try{

        tf::StampedTransform transform_base;
        try {
            listener_ptr->lookupTransform("kinect2_link", "base_link", ros::Time(0), transform_base);

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
}



bool drive_go_cb(const armadillo2_bgu::SimpleTargetGoalConstPtr& goal, target_server_t* as) {

    ros::NodeHandle n;
    geometry_msgs::Point point;
    point.x = goal->x;
    point.y = goal->y;
    point.z = goal->z;

    move_base_msgs::MoveBaseGoal move_goal = get_pre_pick_pose(point);

    if (base_cmd(move_goal)) {
        ROS_INFO("Reached position");
        as->setSucceeded();
    }
    else
        as->setAborted();

}



/*call that this way:
move_base_msgs::MoveBaseGoal goal= get_pre_pick_pose(pose position);


if (base_cmd(goal)) {
ROS_INFO("Reached position");
res.message="Reached pre-picking position";
res.success=true;
}
 */

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

int main(int argc, char **argv) {

    ros::init(argc, argv, "move");
    ros::NodeHandle n;

    MoveBaseClient moveBaseClient("move_base", true);
    //wait for the action server to come up
    while(!moveBaseClient.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    moveBaseClient_ptr=&moveBaseClient;
    target_server_t server(n,"move",boost::bind(&drive_go_cb, _1, &server), false);
    server.start();
    tf::TransformListener listener;
    listener_ptr=&listener;

    ros::spin();

    return 0;
}

