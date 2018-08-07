

1 .ssh -X to robot , run launch with kinect
	ssh -X armadillo1@132.73.207.248
2. in new terminal ssh to ICVLDL server (this would change to docker on robot)
	a. ssh icvl@132.72.44.66 (pass : V!sL@b03)
	b. docker attach agitated_bose 
	c. cd ~/mnt_ws/maskRcnn
	d. python3 dlserver.py
	e. "wait for server started"
3. in new terminal
	a. cd ~/catkin_ws/src/nn_object_detection/nn_client/scripts/
	b. rosrun nn_client points.py
