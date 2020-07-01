#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "sensor_msgs/Image.h"

class process_image_class
{
public:
	process_image_class()
	{
		client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
		rawimage = n.subscribe("/camera/rgb/image_raw", 10 ,&process_image_class::processor_callback, this);
		ROS_INFO_STREAM("Constructor process_image initialized");
	}

	void send_command(float lin_x, float ang_z)
	{
		ball_chaser::DriveToTarget srv;
		srv.request.linear_x = lin_x;
		srv.request.angular_z = ang_z;

		if(!client.call(srv))
		{
			ROS_ERROR("Cannot call the service");
		}
	}


	void processor_callback(sensor_msgs::Image img)
	{
		int step = 0;
		int r,g,b;


		for (int h = 0; h<img.height; h++)
		{
			for (int s = 0; s<img.step; s+=3)
			{
				r = img.data[h*img.step+(s+0)];
				g = img.data[h*img.step+(s+1)];
				b = img.data[h*img.step+(s+2)];

				if (r==255 && g==255 && b==255)
				{
					step = s;
					break;
				}
			}
		}


		float lin_x, ang_z;

		if (step ==0)
		{
			lin_x=0;
			ang_z=0;
		}


		if (0< step && step <= img.step/3.0)
		{
			lin_x = 0;
			ang_z = 0.5;
		}

		if (img.step/3.0 < step && step< img.step*(2.0/3.0))
		{
			lin_x = 0.5;
			ang_z = 0;
		}

		if (step>=img.step*(2.0/3.0))
		{
			lin_x = 0;
			ang_z = -0.5;
		}


		send_command(lin_x,ang_z);
		
	}




private:
	ros::NodeHandle n;
	ros::ServiceClient client;
	ros::Subscriber rawimage;
	
};



int main(int argc, char** argv)
{
	ros::init(argc,argv,"process_image");
	process_image_class object;
	ros::spin();
}
