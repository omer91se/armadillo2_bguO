#include <map>
#include <iterator>
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/Data_baseAction.h>
#include <iostream>
#include <string>



typedef actionlib::SimpleActionServer<armadillo2_bgu::Data_baseAction> Server;
using namespace std;

map <string, vector<double>> data_base;


/*
 * Updates the database map with x,y,z at key
 * fail: if goal is null
 */
void update(const armadillo2_bgu::Data_baseGoalConstPtr& goal, Server* as)
{

    armadillo2_bgu::Data_baseResult result_;
    ROS_INFO("IN data_base_node: Updating database");

    if(goal == nullptr) {
        as->setAborted();
        ROS_INFO("[data_base (update)]: the goal is empty");
        return;
    }


    vector<double> location;
    location.push_back(goal->x);
    location.push_back(goal->y);
    location.push_back(goal->z);

    data_base[goal->key] = location;

    as->setSucceeded(result_);
}

/*
 * Extracts x,y,z from a given string key
 *fail:
 * 1) if goal is empty.
 * 2) if the key is not in the map.
 */
void extract(const armadillo2_bgu::Data_baseGoalConstPtr& goal, Server* as)
{
    armadillo2_bgu::Data_baseResult result_;
    ROS_INFO("IN data_base_node: Extracting");

    if(goal == nullptr) {
        as->setAborted();
        ROS_INFO("[data_base (update)]: the goal is empty");
        return;
    }


    if(data_base.count(goal->key)==0) {
        as->setAborted();
        ROS_INFO("[data_base (update)]: that object is not in the data base, please update the data base.");
    }

    vector<double> location;
    location = data_base[goal->key];
    result_.x = location[0];
    result_.x = location[1];
    result_.x = location[2];
    as->setSucceeded(result_);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "data_base");
    std::cout<<"data_base Online"<<std::endl;
    ros::NodeHandle n;

    Server server_update(n, "update_data_base", boost::bind(&update, _1, &server_update), false);
    Server server_extract(n, "extract_data_base", boost::bind(&extract, _1, &server_extract), false);
    server_update.start();
    server_extract.start();

    ros::spin();
    return 0;
}