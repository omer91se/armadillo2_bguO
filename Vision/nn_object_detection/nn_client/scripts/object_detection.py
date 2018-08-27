#!/usr/bin/env python

from __future__ import print_function
import rospy
import numpy as np
import cv2
from cv_bridge import CvBridge
from std_msgs.msg import Header
from sensor_msgs.msg import PointCloud2
from sensor_msgs.msg import Image
from geometry_msgs.msg import Vector3, PointStamped, Point
import sensor_msgs.point_cloud2 as pc2
import json
import requests
from StringIO import StringIO
import detection_marker


#_url = 'http://132.73.199.239:8383'
_url = 'http://132.72.44.66:8383'
_maxNumRetries = 10
#task='place'
task='analyze'

class ObjectDetector:

    def __init__(self):


        self.cv_bridge = CvBridge()
        self.frame_id = "kinect2_depth_optical_frame"
        self.server_busy = False
        self.analysis_str = '' 
        self.sub = rospy.Subscriber("/kinect2/qhd/points", PointCloud2, self.on_new_img)
        #rospy.Subscriber("/kinect2/hd/points", PointCloud2, self.on_new_img)

      
        print("I'm up")
        
        #rospy.spin()

    def on_new_img(self, msg):
        print('new image')
        if self.server_busy:
            print('already processing image')
            return
            
        self.server_busy = True
            
        if(task=='place'):
            self.place_on_table(msg)
        if(task=='analyze'):
            self.geometrical_analisys(msg)
    
        self.server_busy = False

    @staticmethod
    def processRequest(data, action):
        params = dict()

        headers = dict()
        headers['Content-Type'] = 'application/octet-stream'
        
    
        response = requests.request('post', _url + action, data=data, headers=headers, params=params)
        string_io = StringIO(response.content)
        json_result = json.load(string_io)
        print('json received- ', json_result)
        return json_result
        
    @staticmethod
    def place_on_table(msg):
        '''send image to nn_server
        recieve pixel by row and column
        get x,y,z cordinates
        mark position in 3d and 2d'''
          
        bgr = ObjectDetector.pc2_to_bgr(msg)
        #cv2.imshow('image', bgr)
        cv2.imwrite("../data/image.jpg",bgr)
        pathToFileInDisk = "../data/image.jpg"
        with open(pathToFileInDisk, 'rb') as f:
            data = f.read()

        points = ObjectDetector.processRequest(data,'/where_to_place')

        #for idx,val in enumerate(points):
        print('point - ', points[1],points[2])
        transform = ObjectDetector.get_transform(msg, (points[1],points[2]))
        print('transform - ', transform)
        detection_marker.new_marker(transform)
        #cv2.waitKey(10)
        cv2.circle(bgr,(points[1],points[2]), 5, (0,0,255), -1)
        cv2.imwrite("../data/image1.jpg",bgr)
        
        points = ObjectDetector.processRequest({'transform':transform},'/transform')
        
    
    def geometrical_analisys(self, msg):
        '''send image to nn_server'''
          
        bgr = ObjectDetector.pc2_to_bgr(msg)
        #cv2.imshow('image', bgr)
        cv2.imwrite("../data/image.jpg",bgr)
        pathToFileInDisk = "../data/image.jpg"
        with open(pathToFileInDisk, 'rb') as f:
            data = f.read()

        points = ObjectDetector.processRequest(data,'/geometrical_analysis')
        transform = []
        for point in points:
        #for idx,val in enumerate(points):
            print('point - ', point[0],point[1])
            transform.append(ObjectDetector.get_transform(msg, (point[0],point[1])))
        
        print('transform', transform)
        
        
        json_transform = json.dumps(transform)
        analysis = ObjectDetector.processRequest(json_transform,'/second_geometrical')
        print('************analysis************')        
        print(analysis)
        self.analysis_str= analysis
        print('************analysis************')
        self.sub.unregister()      
        
        #data = {'key':'value'}
        
        
        #detection_marker.new_marker(transform)
        #cv2.waitKey(10)
        #cv2.circle(bgr,(points[1],points[2]), 5, (0,0,255), -1)
        #cv2.imwrite("../data/image1.jpg",bgr)
    
    
    def get_analysis(self):
        return self.analysis_str

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
        ObjectDetector = ObjectDetector()
    except rospy.ROSInterruptException:
        pass





    





