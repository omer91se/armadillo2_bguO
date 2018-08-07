#! /usr/bin/env python

import rospy
import actionlib
from armadillo2_bgu.msg import *

def do_vocal(goal):
    print goal.data
    result = VocalCmdResult()
    result.res = 'Done'
    server.set_succeeded(result)
    
rospy.init_node('plan_initializer_server')
server = actionlib.SimpleActionServer('vocal_cmd_parsed', VocalCmdAction, do_vocal, False)
server.start()
rospy.spin()

