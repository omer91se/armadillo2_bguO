cmake_minimum_required(VERSION 2.8.3)
project(armadillo2_bgu)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  actionlib_msgs
  actionlib
  message_generation
  cv_bridge
  geometry_msgs
  moveit_core
  moveit_ros_planning
  moveit_ros_planning_interface 
  tf
  geometry_msgs
)
find_package( OpenCV REQUIRED )

add_message_files(
  FILES
  ActionDispatch.msg
  ActionFeedback.msg
)
add_service_files(
  FILES ReturnJointStates.srv
)
## Generate actions in the 'action' folder
add_action_files(
  DIRECTORY action
  	 FILES Operation.action 
	 VocalCmd.action
         SimplePick.action
         SimplePlace.action
         SimpleTarget.action
)


generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs

)

catkin_package(
  CATKIN_DEPENDS actionlib_msgs message_runtime
)


include_directories(

  ${catkin_INCLUDE_DIRS}
)

add_executable(arm_pick src/arm_pick.cpp)
target_link_libraries(  arm_pick  ${catkin_LIBRARIES})
add_dependencies(arm_pick armadillo2_bgu_generate_messages_cpp)

add_executable(arm_trajectory_give src/arm_trajectory_give.cpp)
target_link_libraries(arm_trajectory_give  ${catkin_LIBRARIES})
add_dependencies(arm_trajectory_give armadillo2_bgu_generate_messages_cpp)

add_executable(arm_trajectory_close_gripper src/arm_trajectory_close_gripper.cpp)
target_link_libraries(arm_trajectory_close_gripper  ${catkin_LIBRARIES})
add_dependencies(arm_trajectory_close_gripper armadillo2_bgu_generate_messages_cpp)

add_executable(arm_trajectory_open_gripper src/arm_trajectory_open_gripper.cpp)
target_link_libraries(arm_trajectory_open_gripper  ${catkin_LIBRARIES})
add_dependencies(arm_trajectory_open_gripper armadillo2_bgu_generate_messages_cpp)

add_executable(master src/master.cpp)
target_link_libraries(master ${catkin_LIBRARIES})
add_dependencies(master armadillo2_bgu_generate_messages_cpp)

add_executable(observe src/observe.cpp)
target_link_libraries(  observe  ${catkin_LIBRARIES})
add_dependencies(observe armadillo2_bgu_generate_messages_cpp)

add_executable(place src/place.cpp)
target_link_libraries(  place  ${catkin_LIBRARIES})
add_dependencies(place armadillo2_bgu_generate_messages_cpp)

add_executable(pick src/pick.cpp)
target_link_libraries(  pick  ${catkin_LIBRARIES})
add_dependencies(pick armadillo2_bgu_generate_messages_cpp)

add_executable(move src/move.cpp)
target_link_libraries(  move  ${catkin_LIBRARIES})
add_dependencies(move armadillo2_bgu_generate_messages_cpp)

add_executable(face_detect_omer src/face_detect_omer.cpp)
target_link_libraries(face_detect_omer ${catkin_LIBRARIES} ${OpenCV_LIBS})
add_dependencies(face_detect_omer armadillo2_bgu_generate_messages_cpp)

add_executable(object_detect src/object_detect.cpp)
target_link_libraries(  object_detect  ${catkin_LIBRARIES})
add_dependencies(object_detect armadillo2_bgu_generate_messages_cpp)

add_executable(omer_planner src/omer_planner.cpp)
target_link_libraries(  omer_planner  ${catkin_LIBRARIES})
add_dependencies(omer_planner armadillo2_bgu_generate_messages_cpp)

add_executable(pour src/pour.cpp)
target_link_libraries(  pour  ${catkin_LIBRARIES})
add_dependencies(pour armadillo2_bgu_generate_messages_cpp)

add_executable(detect_elv_door_open src/detect_elv_door_open.cpp)
target_link_libraries(detect_elv_door_open  ${catkin_LIBRARIES})
add_dependencies(detect_elv_door_open armadillo2_bgu_generate_messages_cpp)

add_executable(enter_elvator src/enter_elvator.cpp)
target_link_libraries(enter_elvator  ${catkin_LIBRARIES})
add_dependencies(enter_elvator armadillo2_bgu_generate_messages_cpp)

add_executable(exit-elevator src/exit-elevator.cpp)
target_link_libraries(exit-elevator  ${catkin_LIBRARIES})
add_dependencies(exit-elevator armadillo2_bgu_generate_messages_cpp)

add_executable(face_elevator src/face_elevator.cpp)
target_link_libraries(face_elevator  ${catkin_LIBRARIES})
add_dependencies(face_elevator armadillo2_bgu_generate_messages_cpp)

add_executable(move-away-elev src/move-away-elev.cpp)
target_link_libraries(move-away-elev   ${catkin_LIBRARIES})
add_dependencies(move-away-elev armadillo2_bgu_generate_messages_cpp)

add_executable(move-elv-2-floor src/move-elv-2-floor.cpp)
target_link_libraries(move-elv-2-floor   ${catkin_LIBRARIES})
add_dependencies(move-elv-2-floor armadillo2_bgu_generate_messages_cpp)

add_executable(move_to_elevator src/move_to_elevator.cpp)
target_link_libraries(move_to_elevator   ${catkin_LIBRARIES})
add_dependencies(move_to_elevator armadillo2_bgu_generate_messages_cpp)

add_executable(reset-mogo-db src/reset-mogo-db.cpp)
target_link_libraries(reset-mogo-db  ${catkin_LIBRARIES})
add_dependencies(reset-mogo-db armadillo2_bgu_generate_messages_cpp)

add_executable(give src/give.cpp)
target_link_libraries(give  ${catkin_LIBRARIES})
add_dependencies(give armadillo2_bgu_generate_messages_cpp)



add_executable(moveNear src/moveNear.cpp)
target_link_libraries(moveNear  ${catkin_LIBRARIES})
add_dependencies(moveNear armadillo2_bgu_generate_messages_cpp)

add_executable(master_old src/master_old.cpp)
target_link_libraries(master_old  ${catkin_LIBRARIES})
add_dependencies(master_old armadillo2_bgu_generate_messages_cpp)









