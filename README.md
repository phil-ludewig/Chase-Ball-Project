# Chase-Ball-Project
This project demonstrates the integration of a camera image with a ROS service node.
A camera node scans the raw camera stream for a white ball. If a ball is detected, the camera node calls a ROS service to steer the robot towards it.

#### Instructions to launch
- Create catkin_ws and clone /src folder contents into /src directory of workspace
- Build cakin_ws
- Source ROS environment
- Launch launch_project.sh from /shellScripts directory
- Move ball in front of robot
