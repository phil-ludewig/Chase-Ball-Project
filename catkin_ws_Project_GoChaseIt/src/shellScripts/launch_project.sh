#!/bin/sh
xterm -e "roslaunch ~/catkin_ws_Project_GoChaseIt/src/my_robot/launch/world.launch" &
sleep 5
xterm -e "rosrun image_view image_view image:=/camera/rgb/image_raw" &
sleep 5
xterm -e "roslaunch ~/catkin_ws_Project_GoChaseIt/src/ball_chaser/launch/ball_chaser.launch"
