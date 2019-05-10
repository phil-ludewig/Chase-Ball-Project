#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <math.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if(!client.call(srv))
      ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int rgb_threshold = 240;
    int section_size = ceil(img.step / 9); // determines which section the white ball is: 3 values for rgb each, then 3 sections (3x3 = 9)
    bool white_detected = false;
    int pixel_column = 0;

    // find first white pixel in image (should be top of the ball)
    for(int i = 0; i < img.height * img.step - 4; i+=3) // i+3 to skip to start of next rgb row, -4 to prevent i surpassing matrix size
    {
      int red_channel = img.data[i];
      int green_channel = img.data[i+1];
      int blue_channel = img.data[i+2];

      if(red_channel > rgb_threshold && green_channel > rgb_threshold && blue_channel > rgb_threshold) // white ball detected
      {
        //ROS_INFO("White Pixel detected.");
        pixel_column = (i % img.step) / 3;
        white_detected = true;
        break;
      }

    }

    if(white_detected)
    {
      if(pixel_column < section_size) // white detected in left section
      {
        //ROS_INFO("Left");
        drive_robot(0.0, 0.5);
      }

      else if(pixel_column < section_size * 2) // white detected in middle section
      {
        //ROS_INFO("Straight");
        drive_robot(0.5, 0.0);
      }

      else // white detected in right section
      {
        //ROS_INFO("Right");
        drive_robot(0.0, -0.5);
      }
    }

    else
      drive_robot(0.0, 0.0); // stop robot
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);
    ROS_INFO("Process image node initialized.");
    ROS_INFO("Move ball in front of robot now.");
    // Handle ROS communication events
    ros::spin();

    return 0;
}
