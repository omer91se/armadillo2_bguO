#! /usr/bin/env python

import rospy
import actionlib
from armadillo2_bgu.msg import *
import object_detection

server = None

def vision_callback(msg):
    print("hi")
    ObjectDetector = object_detection.ObjectDetector()
    print("hi2")
    while (ObjectDetector.get_analysis() == '' ):
        print('while')

    analisys = ObjectDetector.get_analysis()
    
    # TBD - clean string 

    _result = armadillo2_bgu.msg.OperationResult()

    _result.res = analisys
    rospy.loginfo('goal set')
    global server
    server.set_succeeded(_result)

def main(): 
    rospy.init_node('DescribeWhatUC')
    global server
    server = actionlib.SimpleActionServer('pick', OperationAction,vision_callback, False)
    server.start()
    rospy.spin()

if __name__ == "__main__":
    main()
