#!/usr/bin/env python

import rospy
import roslib
roslib.load_manifest('armadillo2_bgu')
import armadillo2_bgu
import actionlib
from PIL import Image, ImageTk, ImageDraw
from sensor_msgs.msg import Image as SensorImage
import sensor_msgs.point_cloud2 as pc2
from armadillo2_bgu.msg import OperationAction
from object_recognition.objects_detection import processRequest, parse_query
from object_recognition.tts import tts

from object_recognition.voice_recognition import SpeechDetector

from Tkinter import Tk, Label, Button

from object_recognition import scene_description

from time import sleep

import os

done = False
image_file = os.path.dirname(os.path.abspath(__file__)) + '/two_cups.jpg'
my_gui = None
image_data = None
server = None
answer = None

def image_callback(data):
    global my_gui
    global done
    global image_file
    if not done:
        done = True
        print('Processing image...')

        imgSize = (data.width, data.height)
        rawData = data.data
        img = Image.frombytes('RGB', imgSize, rawData)
        image_file = 'robot_image.png'
        img.save(image_file)
        image_data = data

        image = Image.open(image_file)
        photo = ImageTk.PhotoImage(image)
        my_gui.label.configure(image=photo)
        my_gui.label.image = photo  # keep a reference!

def caption_image():
    #Armadillo 1 and 2:
    rospy.Subscriber("kinect2/qhd/image_color", SensorImage, image_callback)
    #Old:
    #rospy.Subscriber("/front_camera/image_raw", SensorImage, image_callback)

    print "Demo is ready"
    #rospy.spin()


class MyFirstGUI:
    def __init__(self, master, image_file):
        self.master = master
        master.title("Demo")

        self.start_demo_button = Button(master, text="Start", command=self.start_demo)
        self.start_demo_button.pack()

        image = Image.open(image_file)
        photo = ImageTk.PhotoImage(image)
        self.label = Label(image=photo)
        self.label.image = photo  # keep a reference!
        self.label.pack()

    def draw_box(self, y1, x1, y2, x2):
        global image_file

        image = Image.open(image_file)

        draw = ImageDraw.Draw(image)

        draw.rectangle((x1, y1, x2, y2), outline=(0, 0, 200))

        del draw

        photo = ImageTk.PhotoImage(image)
        self.label.configure(image=photo)
        self.label.image = photo  # keep a reference!

    def query_callback(self, query, ignore_params=None):
        global image_file
        global answer
        query = query.replace('cap', 'cup')
        print(query)

        if query == '<unrecognized speech>':
            #query='Show me the person'
            return

        parsed_query = parse_query(query)

        subject = parsed_query['subject']
        label = parsed_query['label']
        if label is None:
            return

        print(label)

        with open(image_file, 'rb') as f:
            data = f.read()

        response = processRequest(data)
        print(response)

        candidate_indices = []

        for i, class_name in enumerate(response['result']['class_names']):
            if class_name.startswith(label + '|'):
                candidate_indices.append(i)
        if len(candidate_indices) == 0:
            tts("I see no " + subject)
            answer = 'object_not_found'
        elif len(candidate_indices) == 1:
            y1, x1, y2, x2 = response['result']['yx_boxes'][candidate_indices[0]]
            self.draw_box(y1, x1, y2, x2)
            tts("Here is the " + subject)
            self.return_point((y1, x1, y2, x2))
        elif len(candidate_indices) == 2:
            candidate_boxes = []

            for i in candidate_indices:
                y1, x1, y2, x2 = response['result']['yx_boxes'][i]
                x_center = (x1 + x2) / 2

                candidate_boxes.append((y1, x1, y2, x2, x_center))

            candidate_boxes.sort(key=lambda box: box[4])

            tts("Do you mean the " + subject + " on the left or the " + subject + " on the right?")
            sd = SpeechDetector()
            sd.run(self.answer_callback, (candidate_boxes, subject))
        else:
            answer = 'object_not_found' 

    def answer_callback(self, answer, candidate_boxes_and_subject):
        print(answer)
        if answer=='<unrecognized speech>':
            tts('I didn\'t understand. Please try again.')
            sd = SpeechDetector()
            sd.run(self.answer_callback, candidate_boxes_and_subject)
            return

        if 'left' in answer:
            box = candidate_boxes_and_subject[0][0]
        elif 'right' in answer:
            box = candidate_boxes_and_subject[0][1]
	else:
            tts('I didn\'t understand. Please try again.')
            sd = SpeechDetector()
            sd.run(self.answer_callback, candidate_boxes_and_subject)
            return

        self.draw_box(box[0], box[1], box[2], box[3])
        tts("Here is the " + candidate_boxes_and_subject[1])
        self.return_point(box)

    def return_point(self, box):
        global answer

        point = ((box[1] + box[3]) / 2, (box[0] + box[2]) / 2)
        width = (box[3] - box[1]) / 2
        height = (box[2] - box[0]) / 2

        #point_xyz = self.get_transform(point)
        #print(point_xyz)
        #point_xyz['z']

        answer = ('x' + str(point[0]) + 'y' + str(point[1]) + 'z' + str(0) + \
                 'w' + str(width) + 'h' + str(height), \
                 'IN PICK: Object placed in: x:' + str(point[0]) + ' y:' + str(point[1]) + ' z:' + str(0) + \
                 ' w:' + str(width) + ' h:' + str(height))


    def get_transform(self, point):
        global image_data

        coordinate = [int(point[0]), int(point[1])]
        generator = pc2.read_points(image_data, field_names=['x', 'y', 'z'], uvs=[coordinate])

        return generator.next()


    def start_demo(self):
        global image_file

        with open(image_file, 'rb') as f:
            data = f.read()

        #desc = scene_description.processRequest(data)
        #desc = desc['description']['captions'][0]['text']
        #print(desc)
        #tts(desc)

        #response = processRequest(data)
        #print(response)
        #tts('I see ' + response['objects_string'])
        
        tts('Hi there! How can I help?')

        sd = SpeechDetector()
        sd.run(self.query_callback)

def execute(goal):

    _result = armadillo2_bgu.msg.OperationResult()
    rospy.loginfo("IN PICK: Observing...")
    
    global image_file
    global my_gui
    global server
    global answer

    root = Tk()

    my_gui = MyFirstGUI(root, image_file)

    caption_image()
    
    my_gui.start_demo()    

    #root.mainloop()
    
    while answer is None:
        sleep(0.05)

    if answer != 'object_not_found': #object found
        _result.res = answer[0]
        rospy.loginfo(answer[1])
        server.set_succeeded(_result)
    
    else: #could not find object
        server.set_aborted()
    


	

def main():
    rospy.loginfo("***************")

    global server
    rospy.init_node('observe')
    server = actionlib.SimpleActionServer('observe', OperationAction,execute, False)
    server.start()
    rospy.spin()



if __name__ == "__main__":
    main()
    # caption_image()
