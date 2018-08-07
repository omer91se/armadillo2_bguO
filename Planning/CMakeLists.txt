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
  FILES Operation.action VocalCmd.action
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

add_executable(arm src/arm.cpp)
target_link_libraries(  arm  ${catkin_LIBRARIES})
add_dependencies(arm armadillo2_bgu_generate_messages_cpp)

add_executable(pour src/pour.cpp)
target_link_libraries(  pour  ${catkin_LIBRARIES})
add_dependencies(pour armadillo2_bgu_generate_messages_cpp)




