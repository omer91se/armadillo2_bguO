#!/usr/bin/env python

import rospy

import numpy as np
import cv2

from cv_bridge import CvBridge
#from cv_helper import CvHelper

from std_msgs.msg import Header
from sensor_msgs.msg import PointCloud2
from sensor_msgs.msg import Image
from geometry_msgs.msg import Vector3, PointStamped, Point

import sensor_msgs.point_cloud2 as pc2


import socket
import os
import json



class RedDetector:

    def __init__(self):

        rospy.init_node('red_detector', anonymous=True)

        self.cv_bridge = CvBridge()
        self.frame_id = "kinect2_depth_optical_frame"

        rospy.Subscriber("/kinect2/qhd/points", PointCloud2, self.on_new_img)
        rospy.Subscriber("/kinect2/hd/points", PointCloud2, self.on_new_img)

      
        print "I'm up"
        rospy.spin()

    def on_new_img(self, msg):
        bgr = RedDetector.pc2_to_bgr(msg)
        #cv2.imshow('image', bgr)
        cv2.imwrite("../data/image.jpg",bgr)
        
        points=RedDetector.cb()
        for idx,val in enumerate(points):
            print(points[idx])
            transform = RedDetector.get_transform(msg, points[idx]['coord'])
            print(transform)
        #cv2.waitKey(10)
		
    @staticmethod
    def cb():
        try:
	    host = '132.72.44.66'
	    port = 5005
	    s = socket.socket()
	    s.connect((host, port))
	    print 'connect'
	    filename='../data/image.jpg'
	    with open(filename, 'r') as f:
	        s.send(str(os.path.getsize(filename)))
	        print s.recv(1024)
	        bytesToSend = f.read(1024)
	        s.send(bytesToSend)
	        while bytesToSend != "":
	            bytesToSend = f.read(1024)
	            s.send(bytesToSend)
	        
	    print 'complete'

	    print s.recv(1024)
	    a=s.recv(1024)
	    b= json.loads(a)
	    
	    s.close()
	    return b
        except KeyboardInterrupt:
            print('^C received, shutting down the web server')
            s.close()

    
    @staticmethod
    def pc2_to_bgr(pc):
        x = np.frombuffer(pc.data, 'uint8').reshape(-1, 8, 4)
        bgr = x[:pc.height*pc.width, 4, :3].reshape(pc.height, pc.width, 3)

        return np.array(bgr)

    @staticmethod
    def get_transform(cloud, point):
        coordiante = [int(point[0]), int(point[1])]
        generator = pc2.read_points(cloud, field_names=['x', 'y', 'z'], uvs=[coordiante])

        return generator.next()

if __name__ == '__main__':
    try:
        RedDetector = RedDetector()
    except rospy.ROSInterruptException:
        pass

