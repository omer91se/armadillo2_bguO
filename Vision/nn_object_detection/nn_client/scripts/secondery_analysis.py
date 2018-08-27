# coding: utf-8
# Efrat: In this script I will run the MASK R-CNN on different images. 
# #and will return the center points of the biggest rectangle on a table

# In[1]:

# Detect
import os
import sys
import random
import math
import numpy as np
import skimage.io
import matplotlib
import matplotlib.pyplot as plt

import utils
import model as modellib
#import visualize



class ImageAnalsys():
    def place_on_desk(r,image,class_names):
        results=[]
        results.append(r)
        #Parameters 
        
        table_ind = 61;
        chair_ind = 57
        person_ind = 1
        laptop_ind = 64
        cup_ind = 42
        Table_detect_flage=0
        x_table = []
        y_table = []
        all_x_table = []
        xlim_2_objects = 50 #a threshold for object  proximity to tables
        ylim_2_objects = 50 #a threshold for object  proximity to tables
        Detected_cups_counter =0 #we will use this to detect cups that are not on tablse 
        v=0 # object color 
        #Detection Flages:
        flage_found_table = 0
        # Initial Mat construction:
        object_counter = np.zeros((len(class_names)),float)
        number_of_tables = np.count_nonzero(r['class_ids'] == table_ind)

        def Center_of_mass(box):
            Center_point_w_h = [round(box[1]+(box[3] - box[1])/2),round(box[0]+(box[2] - box[0])/2)]
            return Center_point_w_h
           
           
        ############################################################
        # Analysis  of tables and objects
        ############################################################

        '''The analysis is based on detected tables.
        # detects objects: cup, laptop ON tables --take the table mask and look for other objects that overlap with the table.
        # person chair couch NEXT to a table  --take the table center of mask  and look for adjacent objects'''


        for i in range(0,len(r['class_ids'])):
            print(str(class_names[r['class_ids'][i]]))
            print(r['class_ids'][i])
            if r['class_ids'][i]==table_ind: # only when a table is detected 
                flage_found_table = flage_found_table+1 # number of detected tables
                Table_detect_flage=1
                box  = results[0]['rois'][i] # the box around the detected table
                [x_table,y_table] = Center_of_mass(box)
                place_to_put_x = x_table
                place_to_put_x = x_table
                all_x_table.append(round(x_table))# this will help us detect the left most table and start the anlysis from the LEFT tables 
                original_mask = r['masks'][:,:,i]# Detect table mask:
                mask = np.zeros(image.shape, dtype=np.uint8)# create the mask in the image size 
                for j in range(mask.shape[0]):# again, detect table mask:
                    for k in range(mask.shape[1]):
                        mask[j,k] = original_mask[j,k] 
                masked_table_image = np.multiply(mask,image)  # multiplication of the original image and the mask of table
                # Go over all detected objects, and look for object that are on the table (their Center_of_mass id on the table mask)
                for obj in range(0,len(r['class_ids'])): 
                    #1-15 rode clases , 57-62 furnitures, 39- 56 + 62- end : food/electroniks : things that can be on a table.  
                    if r['class_ids'][obj]>39 and r['class_ids'][obj]<57 or r['class_ids'][obj]>63: # for clases that can be on  table
                        box  = results[0]['rois'][obj] # bax for each detected object
                        [x,y] = Center_of_mass(box)    # bax for each detected object
                        proximity_threshold = 50 # since we look for containing ratio in the semantic segmentation - there might be occlusions so we need this proximity_threshold to evaluate the "ON" ratio 
                        val = masked_table_image[int(y),int(x)] #check the value of the tables mask in the location of the center of each detected object 
                        val_lower = masked_table_image[int(y)+proximity_threshold,int(x)]
                        # if the values of  'val' or 'val_lower'  is 0 meaning black, then the object is not ON the table
                        if val[0]!=0 and val[1]!=0 and val[2]!=0 or val_lower[0]!=0 and val_lower[1]!=0 and val_lower[2]!=0:
                        # not black mask -  this means that the object is ON the table and there is  YAHAS HACALA (Subset)
                            object_counter[r['class_ids'][obj]] = object_counter[r['class_ids'][obj]]+1
                            object_mask = r['masks'][:,:,obj]# Detect object mask:
                            for ii in range(object_mask.shape[0]):
                                for jj in range(object_mask.shape[1]):
                                    if object_mask[ii,jj]>0: 
                                        masked_table_image[ii,jj,0]= 0
                                        masked_table_image[ii,jj,1]= 0
                                        masked_table_image[ii,jj,2]= 0

                                        
                        
        ############################################################
        # Analysis  ON tables and objects
        ############################################################

        if Table_detect_flage==1:
            ccc=100
            for ii in range(masked_table_image.shape[0]):
                for jj in range(masked_table_image.shape[1]):
                    if masked_table_image[ii,jj,0]>0 or masked_table_image[ii,jj,1]>0 or masked_table_image[ii,jj,2]>0:
                        masked_table_image[ii,jj,0]= ccc
                        masked_table_image[ii,jj,1]= ccc
                        masked_table_image[ii,jj,2]= ccc
            # at this point, all the relevant spaces are in the color ccc
            s = masked_table_image[:,:,1]
            nrows = s.shape[0]
            ncols = s.shape[1]
            area_max=0
            counter =-1
            r_center = 0
            c_center = 0
            area_max_former =0
            w = np.zeros(dtype=int, shape=s.shape)
            h = np.zeros(dtype=int, shape=s.shape)
            for r in range(nrows):
                for c in range(ncols):
                    if math.fmod(r,5)==0 and math.fmod(c,5)==0:
                        if s[r][c] == ccc:
                            flag = 0
                            for dh in range(min(nrows-r-5,ncols-c-5)):
                                if math.fmod(dh,4)==0:
                                    if flag ==0:
                                        area = (dh)*(dh)/2
                                        ddh = round(dh/2)
                                        if s[r][c+dh] == ccc and s[r+dh][c] == ccc and s[r+dh][c+dh] == ccc and s[r][c+ddh] == ccc and s[r+ddh][c] == ccc and s[r+dh][c+ddh] == ccc and s[r+ddh][c+dh] == ccc:
                                            if  area>area_max:
                                                r_center = r+round(dh/2)
                                                c_center = c+round(dh/2)
                                                final_dh = dh
                                                final_r = r
                                                final_c = c
                                                area_max = area
                                        if s[r][c+dh] == 0 or s[r+dh][c] == 0 or  s[r+dh][c+dh] == 0 or  s[r][c+ddh] == 0 or  s[r+ddh][c] == 0 or  s[r+dh][c+ddh] == 0 or  s[r+ddh][c+dh] == 0:
                                            flag = 1

            print('area_max', area_max)
            print('r_center', r_center)
            print('c_center', c_center)

            # The output is r_center ,c_center
            
            return(area_max, c_center, r_center)
            
        else:
            return('no','no','no')

    def geometrical_analysis(r,image,class_names):
        results=[]
        results.append(r)

        boxes = results[0]['rois']

        # The center of mass is formulated as:Center_point_w_h= [x1+BB_width/2, y1+BB_hight/2]
        def Center_of_mass(box):
            """Center_of_mass is the center feauter of a detected object
                Arguments
                input: box  = results[0]['rois'][i], This is the samnatic segmantion aroun an detected object. Output if the NN. 
                Output: the center of a NN detected bject. In pixels."""
            Center_point_w_h = [round(box[1]+(box[3] - box[1])/2),round(box[0]+(box[2] - box[0])/2)]
            return Center_point_w_h

        # for box in boxes:
         # Center_of_mass(box) # the first coor is x (width) and the second coor is y (hight) for center of mass
         # print(Center_of_mass(box))# print all detected objects centers of mass


        ############################################################
        # Neighbors matrix
        ############################################################
        '''
        If there are N objects from the 
        class in column i  in the right side of a
         class in the j row ,  neighbors_sides[i,j]= N 
         '''
         
        neighbors_right = np.zeros((len(class_names),len(class_names)),float)
        neighbors_right_distance = np.zeros((len(class_names),len(class_names)),float)
        neighbors_left = np.zeros((len(class_names),len(class_names)),float)
        neighbors_left_distance = np.zeros((len(class_names),len(class_names)),float)

        neighbors_up = np.zeros((len(class_names),len(class_names)),float)
        neighbors_up_distance = np.zeros((len(class_names),len(class_names)),float)

        neighbors_down = np.zeros((len(class_names),len(class_names)),float)
        neighbors_down_distance = np.zeros((len(class_names),len(class_names)),float)

        # _f stands for final
        neighbors_right_f = np.zeros((len(class_names),len(class_names)),float)
        neighbors_left_f = np.zeros((len(class_names),len(class_names)),float)
        neighbors_up_f = np.zeros((len(class_names),len(class_names)),float)
        neighbors_down_f = np.zeros((len(class_names),len(class_names)),float)

        '''Go over all detected classes, for each detected object -  calculate the center of mass and 
        analyse the geometric ratio between each 2 detected objects in terms of left and right (sides).
        Evaluate  the distacne between the 2 detected objects, if the distacne is smaller then TH,  avaluate their 
        geometric relation. We assume that analysing two object located in different sides of the room is irrelevant
          '''
        # Parameters
        number_of_detectet_obj = np.zeros((len(class_names)),float)
        TH = 50 # minimal distacne between the 2 detected objects

        for i in range(0,len(r['class_ids'])): # Go over all detected classes
            box  = results[0]['rois'][i] 
            [xi,yi] = Center_of_mass(box) # for each detected object -  calculate the center of mass
            for j in range(0,len(r['class_ids'])):
                box  = results[0]['rois'][j] # for each detected object -  calculate the center of masss
                [xj,yj] = Center_of_mass(box)
                #analyse the geometric ratio between each 2 detected objects in terms of left and right 
                if xi>xj:
                    neighbors_right[r['class_ids'][i],r['class_ids'][j]] = neighbors_right[r['class_ids'][i],r['class_ids'][j]]+1
                    neighbors_right_distance[r['class_ids'][i],r['class_ids'][j]]= abs(xi-xj)
                if xi<xj:
                    neighbors_left[r['class_ids'][i],r['class_ids'][j]] = neighbors_left[r['class_ids'][i],r['class_ids'][j]]+1
                #analyse the geometric ratio between each 2 detected objects in terms of up and down
                if yi<yj and abs(xi<xj)<TH:
                    neighbors_up[r['class_ids'][i],r['class_ids'][j]] = neighbors_up[r['class_ids'][i],r['class_ids'][j]]+1    
                if yi<yj and abs(xi<xj)<TH:
                    neighbors_down[r['class_ids'][i],r['class_ids'][j]] = neighbors_down[r['class_ids'][i],r['class_ids'][j]]+1    
                    neighbors_down_distance[r['class_ids'][i],r['class_ids'][j]] = abs(yi-yj)# the distacne between the 2 detected objects
        for i in range(0,len(r['class_ids'])):  
            for j in range(0,len(r['class_ids'])):    
                if neighbors_down[r['class_ids'][i],r['class_ids'][j]]>1:
                    if neighbors_down[r['class_ids'][i],r['class_ids'][j]]%2==0: #normalise the amount of detected objects
                        neighbors_down_f[r['class_ids'][i],r['class_ids'][j]] = neighbors_down[r['class_ids'][i],r['class_ids'][j]]/2
                        neighbors_up_f[r['class_ids'][i],r['class_ids'][j]] = neighbors_up[r['class_ids'][i],r['class_ids'][j]]/2
                    else:
                        neighbors_down_f[r['class_ids'][i],r['class_ids'][j]] = ((neighbors_down[r['class_ids'][i],r['class_ids'][j]])-1)/2+1
                        neighbors_up_f[r['class_ids'][i],r['class_ids'][j]] = ((neighbors_up[r['class_ids'][i],r['class_ids'][j]])-1)/2+1
                if neighbors_right[r['class_ids'][i],r['class_ids'][j]]>1:
                    if neighbors_right[r['class_ids'][i],r['class_ids'][j]]%2==0:
                        neighbors_right_f[r['class_ids'][i],r['class_ids'][j]] = neighbors_right[r['class_ids'][i],r['class_ids'][j]]/2
                        neighbors_left_f[r['class_ids'][i],r['class_ids'][j]] = neighbors_left[r['class_ids'][i],r['class_ids'][j]]/2
                    else:
                        neighbors_right_f[r['class_ids'][i],r['class_ids'][j]] = ((neighbors_right[r['class_ids'][i],r['class_ids'][j]])-1)/2+1
                        neighbors_left_f[r['class_ids'][i],r['class_ids'][j]] = ((neighbors_left[r['class_ids'][i],r['class_ids'][j]])-1)/2+1                


        ############################################################
        # Initial naive objects detectection in the room
        ############################################################

        print(' ')
        print('Naive counting of all detected objects:')
        for i in range(0,len(class_names)):
            object_count = np.count_nonzero(r['class_ids'] == i)
            number_of_detectet_obj[i] = object_count
            if object_count > 1:
                print('we detected '+str(object_count)+' '+ str(class_names[i])+'s')
            if object_count == 1:
                print('we detected one '+ str(class_names[i]))
        print(' ')        

        #Parameter: 
        # All relevant clases for geometric analysis:
        table_ind = 61;
        chair_ind = 57
        person_ind = 1
        laptop_ind = 64
        cup_ind = 42
        xlim_2_objects = 50 #a threshold for object  proximity to tables
        ylim_2_objects = 50 #a threshold for object  proximity to tables
        # initialization of mat: 
        object_counter = np.zeros((len(class_names)),float)
        number_of_tables = np.count_nonzero(r['class_ids'] == table_ind)
        Detected_cups_counter =0 #we will use this to detect cups that are not on tablse 
        v=0 # for object color 
        ind=0 
        x_table = []
        y_table = []
        all_x_table = []
        # All relevant flages:
        flage_found_table = 0
        Flage_print_later =0
        Flage_MoreThan1 = 0

        ############################################################
        # Analysis  around tables
        ############################################################

        '''The next analysis is based on detected tables.
        the method find  the table mask and look for other objects (semantic "smaller") . For examplea cup, laptop, book...
        that overlap with the table. For example,  a person, a chair or a couch are NEXT  to a table and are not "ON" a table. 
        In these cases, we find the table center of mask  and look for adjacent objects'''


        for i in range(0,len(r['class_ids'])):
        #Detect cups and chaires around tables 
            if r['class_ids'][i]==table_ind: # only when a table is detected 
                flage_found_table = flage_found_table+1 # number of detected tables
                box  = results[0]['rois'][i] # the box around the detected table
                [x_table,y_table] = Center_of_mass(box)
                all_x_table.append(round(x_table))# this will help us detect the left most table and start the anlysis from the LEFT tables 
                original_mask = r['masks'][:,:,i]# Detect table mask:
                mask = np.zeros(image.shape, dtype=np.uint8)# create the mask in the image size 
                for j in range(mask.shape[0]):# again, detect table mask:
                    for k in range(mask.shape[1]):
                        mask[j,k] = original_mask[j,k] 
                masked_table_image = np.multiply(mask,image)  # multiplication of the original image and the mask of table
                # Go over all detected objects, and look for object that are on the table (their Center_of_mass id on the table mask)
                for obj in range(0,len(r['class_ids'])): 
                    #1-15 rode clases , 57-62 furnitures, 39- 56 + 62- end : food/electroniks : things that can be on a table.  
                    if r['class_ids'][obj]>39 and r['class_ids'][obj]<57 or r['class_ids'][obj]>63: # for clases that can be on  table
                    # THIS CAN BE MUCH MORE SPECIFIC IF NEDDED 
                        box  = results[0]['rois'][obj] # box for each detected object
                        [x,y] = Center_of_mass(box)    # box for each detected object
                        proximity_threshold = 50 # since we look for containing ratio in the semantic segmentation - there might be occlusions so we need this proximity_threshold to evaluate the "ON" ratio 
                        val = masked_table_image[int(y),int(x)] #check the value of the tables mask in the location of the center of each detected object 
                        val_lower = masked_table_image[int(y)+proximity_threshold,int(x)]
                        # if the values of  'val' or 'val_lower'  is 0 meaning black, then the object is not ON the table
                        if val[0]!=0 and val[1]!=0 and val[2]!=0 or val_lower[0]!=0 and val_lower[1]!=0 and val_lower[2]!=0:
                        # not black mask -  this means that the object is ON the table and there is  YAHAS HACALA (Subset)
                            object_counter[r['class_ids'][obj]] = object_counter[r['class_ids'][obj]]+1
                        # verify cups detection 
                            if r['class_ids'][obj]==cup_ind: #if the detected object is a cup -  we can determin the color 
                                Detected_cups_counter = Detected_cups_counter+1
                            # Try to detect color 
                            Color_detection_flage = 0
                            ColorTH = 80
                            v_max = val.max(0)
                            ind = val.argmax(0)
                            v_min = val.min(0)
                            if flage_found_table>1:
                                if v_min>300:
                                    Color_detection_flage=1;
                                    print('there is a white '+ str(class_names[r['class_ids'][obj]])+' on   the ' +str(flage_found_table)+'\'st table from the left' ) 
                                else:
                                    if ind ==0 and (val[0]-val[1])>ColorTH and (val[0]-val[2])>ColorTH:
                                        Color_detection_flage=1; # "red": (255, 0, 0),
                                        print('there is a red '+ str(class_names[r['class_ids'][obj]])+' on   the ' +str(flage_found_table)+'\'st table from the left' ) 
                                    if ind ==1 and abs(val[1]-val[0])>ColorTH and abs(val[1]-val[2])>ColorTH:
                                        Color_detection_flage=1; # "green": (0, 255, 0),
                                        print('there is a green '+ str(class_names[r['class_ids'][obj]])+' on   the ' +str(flage_found_table)+'\'st table from the left' ) 
                                    if ind ==2 and abs(val[2]-val[0])>ColorTH and abs(val[2]-val[1])>ColorTH:
                                        Color_detection_flage=1; # "blue": (0, 0, 255)})
                                        print('there is a blue '+ str(class_names[r['class_ids'][obj]])+' on   the ' +str(flage_found_table)+'\'st table from the left' )     
                                    if  v_max<40:
                                        Color_detection_flage=1;
                                        print('there is a black '+ str(class_names[r['class_ids'][obj]])+' on   the ' +str(flage_found_table)+'\'st table from the left' )         
                            if flage_found_table==1:
                                if v_min>300:
                                    Color_detection_flage=1;
                                    print('there is a white '+ str(class_names[r['class_ids'][obj]])+' on   the  table ' ) 
                                else:
                                    if ind ==0 and (val[0]-val[1])>ColorTH and (val[0]-val[2])>ColorTH:
                                        Color_detection_flage=1; # "red": (255, 0, 0),
                                        print('there is a red '+ str(class_names[r['class_ids'][obj]])+' on   the  table ' ) 
                                    if ind ==1 and abs(val[1]-val[0])>ColorTH and abs(val[1]-val[2])>ColorTH:
                                        Color_detection_flage=1; # "green": (0, 255, 0),
                                        print('there is a green '+ str(class_names[r['class_ids'][obj]])+' on   the  table ' ) 
                                    if ind ==2 and abs(val[2]-val[0])>ColorTH and abs(val[2]-val[1])>ColorTH:
                                        Color_detection_flage=1; # "blue": (0, 0, 255)})
                                        print('there is a blue '+ str(class_names[r['class_ids'][obj]])+' on   the  table ' ) 
                                    if  v_max<40:
                                        Color_detection_flage=1;
                                        print('there is a black '+ str(class_names[r['class_ids'][obj]])+' on   the  table ' ) 

                            if Color_detection_flage==0: # there were no detection for spesific color
                                if r['class_ids'][obj]==chair_ind:# # if a chair is  close enough to a tableis
                                    if abs(x-x_table)<xlim_2_objects or abs(y-y_table)<ylim_2_objects:
                                        object_counter[r['class_ids'][obj]] = object_counter[r['class_ids'][obj]]+1
                                if r['class_ids'][obj]==person_ind:# # if a person  is  close enough to a tables
                                    if abs(x-x_table)<xlim_2_objects or abs(y-y_table)<ylim_2_objects:
                                        object_counter[r['class_ids'][obj]] = object_counter[r['class_ids'][obj]]+1
                                for ii in range(0,len(object_counter)):
                                    if flage_found_table>1:
                                        if object_counter[ii]>0:
                                            if ii==chair_ind or ii==person_ind: #if the detected object is a chair or a person 
                                                if  object_counter[ii]>1:
                                                    print('there are '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+'\'s next to  the ' +str(flage_found_table)+'\'st table from the left' ) 
                                                else:
                                                    print('there is '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+' next to  the ' +str(flage_found_table)+'\'st table from the left' ) 
                                            else:
                                                if object_counter[ii]==1:
                                                    Flage_print_later = 1
                                                    t1 = object_counter[ii]
                                                    t2 = class_names[ii]
                                                    t3 = flage_found_table
                                                else:
                                                    Flage_MoreThan1 = 1
                                                    print(' there  are '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+' on the ' +str(flage_found_table)+'\'st table from the left' ) 
                                    if flage_found_table==1:
                                        if object_counter[ii]>0:
                                            if ii==chair_ind or ii==person_ind: #if the detected object is a chair or a person 
                                                if  object_counter[ii]>1:
                                                    print('there are '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+'\'s next to  the  table ' ) 
                                                else:
                                                    print('there is '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+' next to  the  table ' ) 
                                            else:
                                                if object_counter[ii]==1:
                                                    Flage_print_later = 1
                                                    t1 = object_counter[ii]
                                                    t2 = class_names[ii]
                                                    t3 = flage_found_table
                                                else:
                                                    Flage_MoreThan1 = 1
                                                    print(' there  are '+str(round(object_counter[ii]))+' ' + str(class_names[ii])+' on the  table ' ) 

        if Flage_print_later==1 and Flage_MoreThan1==0 and flage_found_table>1    :
            print(' there is '+str(t1)+' ' + str(t2)+'\'s on the ' +str(t3)+'\'st table from the left' ) 
        if Flage_print_later==1 and Flage_MoreThan1==0 and flage_found_table==1    :
            print(' there is a '+' ' + str(t2)+' on the table ' ) 
            
            
            
        ############################################################
        # Analysis NOT around tables
        ############################################################

        cup_count = np.count_nonzero(r['class_ids'] == cup_ind)# specific for CUPS

        flage_found_cup = 0
        for i in range(0,len(r['class_ids'])):
            if r['class_ids'][i]==cup_ind: 
                if Detected_cups_counter==0: #no cups were detected on tables 
                    flage_found_cup = flage_found_cup+1
                    box  = results[0]['rois'][i] 
                    [x,y] = Center_of_mass(box)
                    val = image[int(y),int(x)]
                    v = val.max(0)
                    ind = val.argmax(0)            
                    v_min = val.min(0)
                    # print(val)
                    TH = 80
                    if cup_count>1:
                        if ind ==0 and (val[0]-val[1])>TH and (val[0]-val[2])>TH:
                            print('the '  +str(flage_found_cup)+'\'st cup from the right is red ')
                        if ind ==1 and abs(val[1]-val[0])>TH and abs(val[1]-val[2])>TH:
                            print('the '  +str(flage_found_cup)+'\'st cup from the right is green ')
                        if ind ==2 and abs(val[2]-val[0])>TH and abs(val[2]-val[1])>TH:
                            print('the '  +str(flage_found_cup)+'\'st cup from the right is  blue ')
                        if v<50:
                            print('the '  +str(flage_found_cup)+'\'st cup from the right is black  ')    
                        if v_min>200:
                            print('the '  +str(flage_found_cup)+'\'st cup from the right is  white  ')    
                    if cup_count==1:
                        if ind ==0 and (val[0]-val[1])>TH and (val[0]-val[2])>TH:
                            print('the  cup  is red ')
                        if ind ==1 and abs(val[1]-val[0])>TH and abs(val[1]-val[2])>TH:
                            print('the  cup  is green ')
                        if ind ==2 and abs(val[2]-val[0])>TH and abs(val[2]-val[1])>TH:
                            print('the  cup  is  blue ')
                        if v<50:
                            print('the  cup is black  ')    
                        if v_min>200:
                            print('the cup is  white  ')    


        ############################################################
        # Hierarchical object semantic 
        ############################################################
        '''
        claases hierarchicy:
        1-15 rode clases 
        57-62 furnitures
        39- 56 + 62- end : food/electroniks : things that can be on a table.  
        For example: we detect objects on  oven, microwave, sink and look for adjacency between them  and other object such as 
        electronics and kitchen products
         '''
         
        #Parameter:
        flage_object_douwn_neighbors = np.zeros((len(class_names),len(class_names)),float)
        flage_object_right_neighbors = np.zeros((len(class_names),len(class_names)),float)
        microwave_ind=69
        oven_ind = 70  
        toaster_ind = 71
        sink_ind=72
        refrigerator_ind= 73
        distance_TH = 130

        for i in range(0,len(r['class_ids'])):
            if r['class_ids'][i]>68 and r['class_ids'][i]<74:
                for j in range(0,len(r['class_ids'])):
                    # print([r['class_ids'][i],r['class_ids'][j]])
                    # print([neighbors_left_f[r['class_ids'][i],r['class_ids'][j]],neighbors_right_f[r['class_ids'][i],r['class_ids'][j]]])
                    # print([neighbors_down_f[r['class_ids'][i],r['class_ids'][j]],neighbors_up_f[r['class_ids'][i],r['class_ids'][j]]])
                    ###1-15 rode clases , 57-62 furnitures, 39- 56 + 62- end : food/electroniks : things that can be on a table.  
                    if r['class_ids'][j]>39 and r['class_ids'][j]<57 or r['class_ids'][j]>63: # for clases that can be on  table
                        if neighbors_right_f[r['class_ids'][i],r['class_ids'][j]]==1 and i!=table_ind and j!=table_ind: 
                            if neighbors_right_distance[r['class_ids'][i],r['class_ids'][j]]< distance_TH:
                                if flage_object_right_neighbors[r['class_ids'][i],r['class_ids'][j]]==0:
                                    flage_object_right_neighbors[r['class_ids'][i],r['class_ids'][j]] = 1
                                    # print(neighbors_right_distance[r['class_ids'][i],r['class_ids'][j]])
                                    print('there is a '+' '+ str(class_names[r['class_ids'][j]])+' right to a ' + str(class_names[r['class_ids'][i]]) )
                        if neighbors_down_f[r['class_ids'][i],r['class_ids'][j]]==1 and i!=table_ind and j!=table_ind: 
                            if  neighbors_down_distance[r['class_ids'][i],r['class_ids'][j]]< distance_TH:
                                if flage_object_douwn_neighbors[r['class_ids'][i],r['class_ids'][j]]==0:
                                    flage_object_douwn_neighbors[r['class_ids'][i],r['class_ids'][j]] = 1
                                    print('there is a '+ ''+ str(class_names[r['class_ids'][j]])+' on top of the ' + str(class_names[r['class_ids'][i]]) )
                                    # print(neighbors_down_distance[r['class_ids'][i],r['class_ids'][j]])
                    




        ############################################################
        # Detect person and the color of thier shirts
        ############################################################


        object_count = np.count_nonzero(r['class_ids'] == person_ind)
        Shirts_x = np.zeros(object_count,float)
        Shirts_y = np.zeros(object_count,float)

        flage_found_person =0
        for i in range(0,len(r['class_ids'])):
            if r['class_ids'][i]==person_ind: #if the detected object is a person
                flage_found_person = flage_found_person+1
                # print('person  '  +str(flage_found_person))        
                box  = results[0]['rois'][i] 
                [x,y] = Center_of_mass(box)
                [x_shirt_person,y_shirt_person] = [x,(2/3)*y] #if standing 
                # [x_shirt_person,y_shirt_person] = [x,y] # if sitting 
                # print([x_shirt_person,y_shirt_person])
                val = image[int(y_shirt_person),int(x_shirt_person)]
                v = val.max(0)
                v_min = val.min(0)
                ind = val.argmax(0)
                TH = 80
                colors_proximity = 20 
                if object_count>1:
                    if ind ==0 and (val[0]-val[1])>TH and (val[0]-val[2])>TH: # "red": (255, 0, 0),
                        print('the '  +str(flage_found_person)+'\'st person from the right is wearing a red shirt')
                    if ind ==1 and abs(val[1]-val[0])>TH and abs(val[1]-val[2])>TH: # "green": (0, 255, 0),
                        print('the '  +str(flage_found_person)+'\'st person from the right is wearing a green shirt')
                    if ind ==2 and abs(val[2]-val[0])>TH and abs(val[2]-val[1])>TH:# "blue": (0, 0, 255)})
                        print('the '  +str(flage_found_table)+'\'st person from the right is wearing a blue shirt')
                    if v<50:
                        print('the '  +str(flage_found_person)+'\'st person from the right is wearing a dark  shirt')    
                    # print(v_min-v)
                    # print(v-v_min)
                    if abs(v-v_min)<colors_proximity :
                        print('the '  +str(flage_found_person)+'\'st person from the right is wearing a dark  shirt')
                    if v_min>200:
                        print('the '  +str(flage_found_person)+'\'st person from the right is wearing a white  shirt')        
                if object_count==1:
                    if ind ==0 and (val[0]-val[1])>TH and (val[0]-val[2])>TH: # "red": (255, 0, 0),
                        print('the  person is wearing a red shirt')
                    if ind ==1 and abs(val[1]-val[0])>TH and abs(val[1]-val[2])>TH: # "green": (0, 255, 0),
                        print('the  person  is wearing a green shirt')
                    if ind ==2 and abs(val[2]-val[0])>TH and abs(val[2]-val[1])>TH:# "blue": (0, 0, 255)})
                        print('the person  is wearing a blue shirt')
                    if v<50:
                        print('the person  is wearing a dark  shirt')    
                    # print(v_min-v)
                    # print(v-v_min)
                    if abs(v-v_min)<colors_proximity :
                        print('the person  is wearing a dark  shirt')
                    if v_min>200:
                        print('the person is wearing a white  shirt')        
        ############################################################
        # return params of Depth Analysis
        
        return ({'detection_objects_amount':len(class_names),'detected_object_index':r['class_ids'].tolist(),'detected_objects_BB':results[0]['rois'].tolist(),'image_zise_1':image.shape[0],'image_zsize_2':image.shape[1]})
        ############################################################



