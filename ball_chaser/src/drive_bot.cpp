#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "geometry_msgs/Twist.h"

class class_drive_bot
{
public:
	class_drive_bot()
	{
		command_robot = n.advertiseService("/ball_chaser/command_robot",&class_drive_bot::command_robot_function,this);
		cmd_vel = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);
		ROS_INFO("Objected constructed");
	}

	bool command_robot_function(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
	{
		float requested_linear_x = req.linear_x;
		float requested_angular_z = req.angular_z;
		ROS_INFO("Request recieved");
		
		geometry_msgs::Twist motor_command;
        // Set wheel velocities, forward [0.5, 0.0]
        motor_command.linear.x = requested_linear_x;
        motor_command.angular.z = requested_angular_z;
        // Publish angles to drive the robot
        cmd_vel.publish(motor_command);
		
		ROS_INFO("Request processed");
		res.msg_feedback = "Wheel velocities: Linear x = " + std::to_string(requested_linear_x)+"   Angular z = "+ std::to_string(requested_angular_z);
		ROS_INFO_STREAM(res.msg_feedback);
		return true;
	}

private:
	ros::ServiceServer command_robot;
	ros::Publisher cmd_vel;
	ros::NodeHandle n;
};



int main(int argc, char** argv)
{
	ros::init(argc,argv,"drive_bot");
	class_drive_bot classobject;
	ros::spin();
	return 0;
}
