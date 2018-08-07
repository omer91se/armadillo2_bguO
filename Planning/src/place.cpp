#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <armadillo2_bgu/OperationAction.h>

typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) 
{
	armadillo2_bgu::OperationResult result_;
	int num = (rand() % 100);
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
		result_.res = "Failure 2: unable to perform place (move-it)";
		as->setAborted(result_);
	}
	if(num<10){
		std::cout<<"random:Failed3."<<std::endl;
		result_.res = "Failure 3: place failed";
		as->setAborted(result_);
	}
	if(num<10){
		std::cout<<"random:Failed4."<<std::endl;
		result_.res = "Failure 4: Hand is empty";
		as->setAborted(result_);
	}

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "place");
  std::cout<<"At pick"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "place", boost::bind(&execute, _1, &server), false);
  server.start();
  std::cout<<"Spining..."<<std::endl;
  ros::spin();
  return 0;
}
