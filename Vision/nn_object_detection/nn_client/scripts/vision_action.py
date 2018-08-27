#! /usr/bin/env python

import rospy
import actionlib
from armadillo2_bgu.msg import *
import object_detection


def vision_client():
    client = actionlib.SimpleActionClient('pour', OperationAction)
    print('waiting for server')
    client.wait_for_server()

    ObjectDetector = ObjectDetector()
    while (ObjectDetector.get_analysis() == '' ):
        pass

    analisys = ObjectDetector.get_analysis()

    goal = OperationActionGoal(analisys)

    client.send_goal(goal)

    client.wait_for_result()

    return client.get_result()

if __name__ == '__main__':
    try:
        rospy.init_node('DescribeWhatUC')
        result = vision_client()
      
    except rospy.ROSInterruptException:
        pass
        #print("program interrupted before completion", file=sys.stderr)

