#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <armadillo2_bgu/OperationAction.h>



typedef actionlib::SimpleActionServer<armadillo2_bgu::OperationAction> Server;
typedef actionlib::SimpleActionClient<armadillo2_bgu::OperationAction> Client;

void doneSTT(const actionlib::SimpleClientGoalState& state,
            const armadillo2_bgu::OperationResultConstPtr& result)
{
    if(result->res == "open") {
        Client open_client("open", true);
        open_client.waitForServer();
        armadillo2_bgu::OperationGoal open_goal;
        open_client.sendGoal(open_goal);
    }
}

void feedbackSTT(const armadillo2_bgu::OperationFeedbackConstPtr& feedback)
{
    //ROS_INFO("Got Feedback of length %lu", feedback->feed.);
}

void activeCB()
{
    ROS_INFO("Goal just went active");
}
//Check if "open" in string
void doneCB(const actionlib::SimpleClientGoalState& state,
            const armadillo2_bgu::OperationResultConstPtr& result)
{
    Client STT_client("text_check",true);
    STT_client.waitForServer();

    armadillo2_bgu::OperationGoal STT_goal;
    STT_goal.data = result->res;
    STT_client.sendGoal(STT_goal,&doneSTT,&activeCB,&feedbackSTT);

}

void feedbackCB(const armadillo2_bgu::OperationFeedbackConstPtr& feedback)
{
    //ROS_INFO("Got Feedback of length %lu", feedback->feed.);
}

void execute(const armadillo2_bgu::OperationGoalConstPtr& goal, Server* as) 
{
	armadillo2_bgu::OperationResult result_;
    ROS_INFO("IN give: Starting give squence");
    //Extand server
    Client extand_client("extand", true);
    extand_client.waitForServer();

    armadillo2_bgu::OperationGoal extand_goal;
    extand_client.sendGoal(extand_goal);

    ros::Duration(2.5).sleep();

    //TTS server
    Client TTS_client("describe_wuc",true);
    TTS_client.waitForServer();

    armadillo2_bgu::OperationGoal TTS_goal;
    TTS_goal.data = "Please take the object";
    TTS_client.sendGoal(TTS_goal,&doneCB,&activeCB,&feedbackCB);


    if(true) {
        as->setSucceeded(result_);
    }
    else{ 
        as->setAborted();
    }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Give");
  std::cout<<"give ready"<<std::endl;
  ros::NodeHandle n;
  Server server(n, "give", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}
