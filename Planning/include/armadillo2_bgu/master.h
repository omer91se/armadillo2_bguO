#include <ros/ros.h>


class Master
{
private:
    ros::NodeHandle *n;
    ros::Subscriber sub;
    ros::Publisher pub;
    ros::AsyncSpinner spinner(4);


public:
};

