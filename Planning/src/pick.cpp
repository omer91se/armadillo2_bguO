#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;
void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as){

    armadillo2_bgu::OperationGoal sendGoal;
    ROS_INFO("Starting pick sequence");
    ros::Duration(0.5).sleep();
    Client client("observe", true);
    client.waitForServer();
    client.sendGoal(sendGoal);

    if(!client.waitForResult(ros::Duration(90.0))){
        ROS_INFO("Vision failed");
    }
    else{
        armadillo2_bgu::OperationResultConstPtr res = client.getResult();

        Client client2("arm",true);
        client2.waitForServer();

        sendGoal.data = res->res;
        client2.sendGoal(sendGoal);

        if(!client2.waitForResult(ros::Duration(90.0))){
            //what to do when vision fails.
        }
    }


    as->setSucceeded();
    ROS_INFO("Done Picking");


    /*
	//int num = (rand() % 100);
	int num = 100;
        std::cout<<"Executing!"<<std::endl;
	if(num > 50){
		std::cout<<"random:Succeses."<<std::endl;
        as->setSucceeded();
	}
	if(num <50 & num>25){
		std::cout<<"random:Failed1."<<std::endl;
		result_.res = "Failure 1: location not found/cannot be disambiguated";
        as->setAborted(result_);
	}
	if(num <10 & num<25){
		std::cout<<"random:Failed2."<<std::endl;
		result_.res = "Failure 2: unable to perform pick (move-it)";
        as->setAborted(result_);
	}
	if(num<10){
		std::cout<<"random:Failed3."<<std::endl;
		result_.res = "Failure 3: pick failed";
        as->setAborted(result_);
	}
    */
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pick");
  std::cout<<"Pick online"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "pick", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();

  return 0;
}
