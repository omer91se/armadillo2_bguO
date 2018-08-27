#!/usr/bin/env python3
"""
Very simple HTTP server in python for logging requests
Usage::
    ./server.py [<port>]
    
    
requirements:

pip install -U textblob textblob-aptagger
python3 -m textblob.download_corpora
pip install -U git+https://github.com/sloria/textblob-aptagger.git@dev

"""
import os
#Disable gpu and enable cpu only
# os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"] = "0"

import sys
sys.path.insert(0,'../maskRcnn')

import numpy as np
import io
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import coco
import utils
import model as modellib
import visualize
from PIL import Image
import scipy.misc
import json
import pickle
import urllib

from textblob import TextBlob
from textblob_aptagger import PerceptronTagger

import secondery_analysis

# Root directory of the project
ROOT_DIR = os.getcwd()

# Directory to save logs and trained model
MODEL_DIR = os.path.join(ROOT_DIR + '/', "logs")

# Path to trained weights file
# Download this file and place in the root of your 
# project (See README file for details)
COCO_MODEL_PATH = os.path.join(ROOT_DIR + '/', "mask_rcnn_coco.h5")

# Directory of images to run detection on
IMAGE_DIR = os.path.join(ROOT_DIR + '/', "images")

class InferenceConfig(coco.CocoConfig):
    # Set batch size to 1 since we'll be running inference on
    # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1
    
config = InferenceConfig()

# Create model object in inference mode.
model = modellib.MaskRCNN(mode="inference", model_dir=MODEL_DIR, config=config)

# Load weights trained on MS-COCO
model.load_weights(COCO_MODEL_PATH, by_name=True)

# COCO Class names
# Index of the class in the list is its ID. For example, to get ID of
# the teddy bear class, use: class_names.index('teddy bear')
class_names = ['BG', 'person|persons', 'bicycle|bicycles', 'car|cars', 'motorcycle|motorcycles', 'airplane|airplanes',
               'bus|buses', 'train|trains', 'truck|trucks', 'boat|boats', 'traffic light|traffic lights',
               'fire hydrant|fire hydrants', 'stop sign|stop signs', 'parking meter|parking meters', 'bench|benches', 'bird|birds',
               'cat|cats', 'dog|dogs', 'horse|horses', 'sheep|sheep', 'cow|cows', 'elephant|elephants', 'bear|bears',
               'zebra|zebras', 'giraffe|giraffes', 'backpack|backpacks', 'umbrella|umbrellas', 'handbag|handbag', 'tie|ties',
               'suitcase|suitcases', 'frisbee|frisbees', 'skis|skis', 'snowboard|snowboards', 'sports ball|sports balls',
               'kite|kites', 'baseball bat|baseball bats', 'baseball glove|baseball gloves', 'skateboard|skateboards',
               'surfboard|surfboards', 'tennis racket|tennis rackets', 'bottle|bottles', 'wine glass|wine glasses', 'cup|cups',
               'fork|forks', 'knife|knives', 'spoon|spoons', 'bowl|bowls', 'banana|bananas', 'apple|apples',
               'sandwich|sandwiches', 'orange|oranges', 'broccoli|broccoli', 'carrot|carrots', 'hot dog|hot dogs', 'pizza',
               'donut|donuts', 'cake|cakes', 'chair|chairs', 'couch|couches', 'potted plant|potted plants', 'bed|beds',
               'dining table|dining tables', 'toilet|toilets', 'tv|tv', 'laptop|laptops', 'mouse|mice', 'remote|remotes',
               'keyboard|keyboards', 'cell phone|cell phones', 'microwave|microwaves', 'oven|ovens', 'toaster|toasters',
               'sink|sinks', 'refrigerator|refrigerators', 'book|books', 'clock|clocks', 'vase|vases', 'scissors|scissors',
               'teddy bear|teddy bears', 'hair drier|hair driers', 'toothbrush|toothbrushes']

with open('subject_labels_sorted.pickle', 'rb') as file:
    subject_labels_sorted = pickle.load(file)

filter_keywords = set([
                   'left', 'right', 'top', 'bottom', 
                   'middle', 'big', 'small', 'long', 'short', 'tall', 'huge', 'tiny',
                   'blue', 'green', 'red', 'yellow', 'pink', 'purple', 'orange', 'black', 'gray', 'white', 'brown'])




globalr=[]
globalimage=[]
globalr.append(0)
globalimage.append(0)

############# code start######################################



class S(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        #logging.info("GET request,\nPath: %s\nHeaders:\n%s\n", str(self.path), str(self.headers))
        #self._set_response()
        #self.wfile.write("GET request for {}".format(self.path).encode('utf-8'))
        
        if self.path.startswith('/parse_query'):
            self.parse_query()

    def do_POST(self):
        if self.path.startswith('/maskrcnn'):
            self.dan_maskrcnn()
        if self.path.startswith('/where_to_place'):
            self.where_to_place()
        if self.path.startswith('/geometrical_analysis'):
            self.geometrical_analysis()
        if self.path.startswith('/second_geometrical'):
            self.geometrical_analysis2()
        if self.path.startswith('/transform'):
            pass
            # maybe I need to transfer this to get, need to see how to extract data.
            #print(transform)
            #self.transform()
    
    
    def parse_query(self):
        s = self.path.split('?')[1]
        params = urllib.parse.parse_qs(s)
        query = params['query'][0]
        
        blob = TextBlob(query)
        tags = blob.tags
        subject = None
        for token, tag in tags:
            if token not in filter_keywords and tag == 'NN':
                subject = token
                break
        
        label = None
        
        if subject and subject in subject_labels_sorted:
            label = subject_labels_sorted[subject][0][0]
            
        response = {'subject': subject, 'label': label}
        
        json_string_response = json.dumps(response)
        
        self._set_response()
        self.wfile.write(json_string_response.encode('utf-8'))
    
    
    
    
    
    
    
    def geometrical_analysis(self):
        globalr[0],globalimage[0] = self.maskrcnn()
        
        res= secondery_analysis.ImageAnalsys.get_centers(globalr[0],globalimage[0],class_names)
        print(res)
        # analayze
        # return row and column of pixel to place object
        # create json from result and send response
        json_string_response = json.dumps(res)
        self._set_response()
        self.wfile.write(json_string_response.encode('utf-8'))
        
    def geometrical_analysis2(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        transform = self.rfile.read(content_length) # <--- Gets the data itself
        transform=json.loads(transform.decode('utf-8'))
        res= secondery_analysis.ImageAnalsys.geometrical_analysis(globalr[0],globalimage[0],class_names,transform)
        print(res)
        # analayze
        # return row and column of pixel to place object
        # create json from result and send response
        json_string_response = json.dumps(res)
        self._set_response()
        self.wfile.write(json_string_response.encode('utf-8'))
   
     
    def where_to_place(self):
        r,image = self.maskrcnn()
        res= secondery_analysis.ImageAnalsys.place_on_desk(r,image,class_names)
        print(res)
        # analayze
        # return row and column of pixel to place object
        # create json from result and send response
        json_string_response = json.dumps(res)
        self._set_response()
        self.wfile.write(json_string_response.encode('utf-8'))

    def maskrcnn(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data_image_bytes = self.rfile.read(content_length) # <--- Gets the data itself
        
        image_path = 'temp.jpg'
        
        image = Image.open(io.BytesIO(post_data_image_bytes))
        image.save(image_path)
        
        image = scipy.misc.imread(image_path)
        if len(image.shape) != 3:
            return

        # Run detection
        results = model.detect([image], verbose=1)
        r = results[0]
        return r,image

    def dan_maskrcnn(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data_image_bytes = self.rfile.read(content_length) # <--- Gets the data itself
        
        image_path = 'temp.jpg'
        
        image = Image.open(io.BytesIO(post_data_image_bytes))
        image.save(image_path)
        
        image = scipy.misc.imread(image_path)
        if len(image.shape) != 3:
            return

        # Run detection
        results = model.detect([image], verbose=1)

        # Visualize results
        r = results[0]

        class_ids = r['class_ids'].tolist()
        yx_boxes = r['rois'].tolist()
        
        current_class_names = [class_names[class_id] for class_id in class_ids]   
        
        response = {'class_names': current_class_names, 'yx_boxes': yx_boxes}#, 'image_colors': image_colors}
        
        json_string_response = json.dumps(response)

        self._set_response()
        self.wfile.write(json_string_response.encode('utf-8'))

def run(server_class=HTTPServer, handler_class=S, port=8383):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
        
