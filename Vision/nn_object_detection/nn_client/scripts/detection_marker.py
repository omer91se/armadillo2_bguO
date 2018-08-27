#!/usr/bin/env python

from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import rospy

topic = 'visualization_marker'
publisher = rospy.Publisher(topic, Marker, queue_size=10)


def new_marker(position):

    marker = Marker()
    marker.header.frame_id = "/kinect2_depth_optical_frame"
    marker.type = marker.SPHERE
    marker.action = marker.ADD
    marker.scale.x = 0.1
    marker.scale.y = 0.1
    marker.scale.z = 0.1
    marker.color.a = 1.0
    marker.color.r = 1.0
    marker.color.g = 1.0
    marker.color.b = 0.0
    marker.pose.position.x = position[0]
    marker.pose.position.y = position[1]
    marker.pose.position.z = position[2]
    marker.pose.orientation.w = 1.0

    # Publish
    publisher.publish(marker)


   
   
