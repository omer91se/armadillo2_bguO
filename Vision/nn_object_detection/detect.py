
# coding: utf-8
# Efrat: In this scrpt I will run the MASK R-CNN on different images. 
# # Mask R-CNN Demo
# 
# A quick intro to using the pre-trained model to detect and segment objects.

# In[1]:


import os
import sys
import random
import math
import numpy as np
import skimage.io
import matplotlib
import matplotlib.pyplot as plt

import coco
import utils
import model as modellib
#import visualize

# Root directory of the project
ROOT_DIR = os.getcwd()

# Directory to save logs and trained model
MODEL_DIR = os.path.join(ROOT_DIR, "maskRcnn/logs")

# Path to   trained weights file
# Download this file and place in the root of your 
# project (See README file for details)
COCO_MODEL_PATH = os.path.join(ROOT_DIR, "mask_rcnn_coco.h5")

class InferenceConfig(coco.CocoConfig):
    # Set batch size to 1 since we'll be running inference on
    # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1

config = InferenceConfig()
config.display()

#create model object in inference mode.
model = modellib.MaskRCNN(mode="inference", model_dir=MODEL_DIR, config=config)

# Load weights trained on MS-COCO
model.load_weights(COCO_MODEL_PATH, by_name=True)



# COCO Class names
# Index of the class in the list is its ID. For example, to get ID of
# the teddy bear class, use: class_names.index('teddy bear')
class_names = ['BG', 'person', 'bicycle', 'car', 'motorcycle', 'airplane',
               'bus', 'train', 'truck', 'boat', 'traffic light',
               'fire hydrant', 'stop sign', 'parking meter', 'bench', 'bird',
               'cat', 'dog', 'horse', 'sheep', 'cow', 'elephant', 'bear',
               'zebra', 'giraffe', 'backpack', 'umbrella', 'handbag', 'tie',
               'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball',
               'kite', 'baseball bat', 'baseball glove', 'skateboard',
               'surfboard', 'tennis racket', 'bottle', 'wine glass', 'cup',
               'fork', 'knife', 'spoon', 'bowl', 'banana', 'apple',
               'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza',
               'donut', 'cake', 'chair', 'couch', 'potted plant', 'bed',
               'dining table', 'toilet', 'tv', 'laptop', 'mouse', 'remote',
               'keyboard', 'cell phone', 'microwave', 'oven', 'toaster',
               'sink', 'refrigerator', 'book', 'clock', 'vase', 'scissors',
               'teddy bear', 'hair drier', 'toothbrush']

               
def detect_center(filename):               
               
    #load image for test: 
    image = skimage.io.imread(filename)

    ### Run detection
    results = model.detect([image], verbose=1)

    # Detection analysis
    r = results[0] # results from a single image

    boxes = results[0]['rois']

    # The center of mass is formulated as:Center_point_w_h= [x1+BB_width/2, y1+BB_hight/2]
    print('Center of mass:')
    def Center_of_mass(box):
       Center_point_w_h = [round(box[1]+(box[3] - box[1])/2),round(box[0]+(box[2] - box[0])/2)]
       return Center_point_w_h


    result = []  
    for idx,box in enumerate(boxes):
        Center_of_mass(box) # the first coor is x (width) and the second coor is y (hight) for center of mass
        result.append({'coord':Center_of_mass(box),'label_name':class_names[r['class_ids'][idx]]})
    
    #print(result)

	## Image analysis: # cup = 42  # dining table = 61  # chair = 57  # TV = 63
	# assumptions:
	# We assume that THERE IS  1 Table
	# We assume that if there is a cup -  its on the table
    print('************************************************************')
    print('Image analysis:')
    numbersOfCups = 0
    numbersOfscreens = 0
    numbersOfchairs = 0
    numbersOfpersons = 0
    for i in range(0,len(r['class_ids'])):
        if r['class_ids'][i]==61:
            print('a dining table was detected ')
    for i in range(0,len(r['class_ids'])):
        if r['class_ids'][i]==42:
            numbersOfCups = numbersOfCups+1
        if r['class_ids'][i]==63:
            numbersOfscreens = numbersOfscreens+1
        if r['class_ids'][i]==57:
            numbersOfchairs = numbersOfchairs+1
    if numbersOfCups>0:
        cup_to_print = "We detected " + str(numbersOfCups)+ " cups on the table"
        print(cup_to_print)
    if numbersOfscreens>0:
        screens_to_print = "We detected " + str(numbersOfscreens)+ " screens on the table"
        print(screens_to_print)
    if numbersOfchairs>0:
        chairs_to_print = "We detected " + str(numbersOfchairs)+ " chairs around the table"
        print(chairs_to_print)
    for i in range(0,len(r['class_ids'])):
        if r['class_ids'][i]==1:
            numbersOfpersons = numbersOfpersons+1
    if numbersOfpersons>0:
        persons_to_print = "We detected " + str(numbersOfpersons)+ " people"
        print(persons_to_print)
    print('************************************************************')
    return result

     
     
     
     
     
     
     
     
     
