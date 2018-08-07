# armadillo2_bgu



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
	
	
	
	
	

Documenting Robotic Code

What does this code do: an explanation of what is the purpose of the code.

Link to the code

Requirements for executing
Hardware requirements
Robot type
External computer — general specifications
External peripheral devices
GPU
Software requirements
OS type and version
ROS version
Dependencies: required ROS packages, C++/Python libraries
Internal Dependencies: capabilities we programed that are used within the code
Other software, and in particular, other packages from the project
Compiler version, if relevant
CMakeList and package.xml required, when relevant
Requirements on the environment: things that need to be true for the code to work. For example: no moving obstacles, object cannot be higher than X, indoor, natural light, etc.

Running instructions 
launch files 
parameters and arguments

Expected behavior
Progress rate: describe things that regularly change during execution, and try to give some bounds. For example, in navigation, the robot’s position should change at some rate
Resource consumption (typically battery)
Success rate: how likely is it to succeed in achieving its goal
Failure modes: what are typical ways it fails, and how likely are they.

Known issues

Troubleshooting: information on problems that are typically encountered and how they can be solved, when possible

Future work: ideas for improvements




