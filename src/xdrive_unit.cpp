
#include <vector>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <ros/ros.h>
#include <signal.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include "xdrive_unit/WheelSpeed.h"
#include "xdrive_unit/RazorImu.h"
// #include "xdrive_unit/TickLR.h"

#include <map>
#include "xdrive_unit/xdriver.h"
 
ros::Time current_time, last_time;

#define M_PIpi 3.1415926
int i_gear_ratio;
double d_drive_freq;
double d_wheel_diameter;
double d_diam;

ros::NodeHandle *private_n;
FILE *fpSerial = NULL; // serial port file pointer

using namespace std;
std::map<int,termios> serial_oldtermios;
int right_wheel_speed;
int left_wheel_speed;
unsigned int numA;
unsigned int numB;

int brate;

typedef struct {
	float Vlin;
	float Vang;
} twist_struct;
twist_struct twist_vec;

void wheel_update_Speed_callback(const xdrive_unit::WheelSpeed& msg)
{
    d_diam = 0.14;
    left_wheel_speed = msg.lwheelmotor/d_diam*M_PIpi*27*60;
    right_wheel_speed = msg.rwheelmotor/d_diam*M_PIpi*27*60;

}


int main(int argc, char** argv)
{  
  char strBuf[100];

	// struct twist_struct twist_vec;
	twist_vec.Vlin =0.2;
	twist_vec.Vang =0;

  ros::init(argc, argv, "vel_basecomm" );
  ROS_INFO("vel_basecomm starting");
  ros::NodeHandle nh;

    // ros::NodeHandle private_n.
  private_n= new ros::NodeHandle("~");
  
    double d_exec_rate;
    if(!private_n->getParam("exec_rate", d_exec_rate))
    {
        ROS_WARN("No exec_rate provided - default: 20 Hz");
        d_exec_rate = 20;
    }

  ROS_INFO("serial connection _ not-use");
  // 
  // ros::Subscriber sub = nh.subscribe("/wheel_speed", 20, update_Speed_callback);
  //
  //  publish ROS messages
  // ros::Publisher ticksLR_pub = nh.advertise<xdrive_unit::TicksLR>("/ticks", 20);
  ros::Publisher ticksMLR_pub = nh.advertise<xdrive_unit::RazorImu>("/ticksMLR", 20);
  
  ros::Duration dur_time;  
  // xdrive_unit::Encoder ticksMsg;
  xdrive_unit::RazorImu ticksMLRmsg;

  ros::Rate loop_rate(d_exec_rate);
  ros::Time read_time = ros::Time::now();

    while(ros::ok())
    {

	xdriver_struct_get("Velo",&twist_vec);
	xdriver_struct_set("Velo",&twist_vec);
	sleep(3);

	xdriver_struct_get("Vlin",&twist_vec);
	xdriver_struct_set("Vlin",&twist_vec);
	
	sleep(2);

	twist_vec.Vlin =0;	
	xdriver_struct_get("Vang",&twist_vec);
	xdriver_struct_set("Vang",&twist_vec);

	// xdrive_motor();
	// ticksMsg.rticks = 99; // numA; // 'L'+numA==rwheel
	// ticksLR_pub.publish(ticksMsg);
    ticksMLRmsg.yaw = 99;
    ticksMLRmsg.pitch = 99;
    ticksMLR_pub.publish(ticksMLRmsg);

        ros::spinOnce();
        loop_rate.sleep();
    }// end.mainloop
}// end.main()
