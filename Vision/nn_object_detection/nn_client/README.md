1. ssh to robot 
    roslaunch armadillo2 armadillo2.launch kinect:=true

2. run new docker :
    sudo nvidia-docker run -it -p 8383:8383 -v /home/armadillo2/catkin_ws/src/nn_object_detection:/root/mnt_ws efrat_shai bash

   or attach by docker ps (notice the port in nn_server)
   
   * cd mnt_ws
   * python3 nn_server.py
   #nn_server use helping methods in secondery_analysis.py 
 
3. run object_detection.py on robot or computer connected to the robot
   #points use methods in detection_marker (to view marker open rviz with pc and marker)
   
   
   TBD: 
   * organize code
   * check why maskrnn takes so much time- does it use the GPU?




 

prequisites:
    pip install textblob
    pip install -U textblob
    pip install textblob_aptagger
   pip install -U git+https://github.com/sloria/textblob-aptagger.git@dev
python3 -m textblob.download_corpora
 

add to .bashrc # use relevant server ip
export VISION_SERVER=http://132.73.199.239:8383
