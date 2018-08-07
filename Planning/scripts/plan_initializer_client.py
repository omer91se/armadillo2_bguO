#! /usr/bin/env python

import rospy
import actionlib
from armadillo2_bgu.msg import *

def vocal_client():
    client = actionlib.SimpleActionClient('vocal_cmd_parsed', VocalCmdAction)

    client.wait_for_server()
    
    vocal_cmd = 'get me coffee' # TBD - Dan set this variable from mic parser

    goal = VocalCmdGoal(vocal_cmd)

    client.send_goal(goal)

    client.wait_for_result()

    return client.get_result()  

if __name__ == '__main__':
    try:
        rospy.init_node('plan_initializer_client')
        result = vocal_client()
      
    except rospy.ROSInterruptException:
        pass
        #print("program interrupted before completion", file=sys.stderr)

