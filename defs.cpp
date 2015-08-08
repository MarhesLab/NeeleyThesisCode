#include "defs.h"

#include <string>

// IP ADDRESS DEFINITIONS
char VICON_IP[] = "192.168.1.120:801";
//const char GCS_IP[]   = "127.0.0.1";
const char GCS_IP[]   = "192.168.1.2";
const char QUAD1_IP[] = "192.168.1.21";
const char QUAD2_IP[] = "192.168.1.22";
const char QUAD3_IP[] = "192.168.1.23";


// SERIAL PORT FOR AUTOPILOT
//const std::string AP_SERIAL = "/dev/ttyUSB0";	// for testing on desktops/laptops
const std::string AP_SERIAL = "/dev/ttyMFD1";	// for implementation on the Edison
const std::string BAUD_RATE = "921600"; //"57600"; 
// AutoPilot supports integer multiples of this value, up to the Edison max.
// Intel Edison supports up to 3,686,400 in hardware;
// good luck finding out how to set it that high

// GENERAL CONSTANTS
const double Ts = 0.005;	// control loop period (s)
const double G = 9.81;		// gravitational acceleration (m/s^2)

// CONVERSIONS
const double pi = 3.141592653589793;
const double rad2rpm = 60/(2*pi);  // radians/second to rpm
const double rpm2rad = (2*pi)/60; // rpm to radians/second
const double deg2rad = pi/180.0;
const double rad2deg = 180.0/pi;

// STATE AND INPUT CONSTANTS
// state index values
const int  X =  0,  Y =  1,   Z =  2; // positions
const int DX = 3,  DY =  4,  DZ =  5; // velocities
const int Q0 =  6, QI =  7,  QJ =  8,  QK = 9;                     // quaternion values
const int WX = 10, WY = 11,  WZ = 12;                              // angular velocities
// Trajectory index values
const int   X_t =  0,   DX_t =  1,   DDX_t =  2, DDDX_t =  3, DDDDX_t =  4;
const int   Y_t =  5,   DY_t =  6,   DDY_t =  7, DDDY_t =  8, DDDDY_t =  9;
const int   Z_t = 10,   DZ_t = 11,   DDZ_t = 12, DDDZ_t = 13, DDDDZ_t = 14;
const int PSI_t = 15, DPSI_t = 16, DDPSI_t = 17;
// Vicon measurement index values
const int Xv = 0, Yv = 1, Zv = 2, Q0v = 3, QIv = 4, QJv = 5, QKv = 6;
// IMU measurement index values
const int  WXi = 0,  WYi = 1,  WZi = 2, // Body frame angular velocities and accelerations
           AXi = 3,  AYi = 4,  AZi = 5; 
		   
// input index values
const int Th = 0, Tx = 1, Ty = 2, Tz = 3;
// motor index values
const int Mfront = 0, Mback = 2, Mleft = 1, Mright = 3;
// Motor 1 (index 0) starts in front, and subsequent motors are counter-clockwise from there.

// MOTOR/PROPELLER VALUES
const unsigned char maxDMCchar = 200;

const double omega_min = 1200*rpm2rad;
const double omega_max = 8600*rpm2rad;
const double omega_sq_min = omega_min*omega_min;
const double omega_sq_max = omega_max*omega_max;

const double omega_idle = 1200*rpm2rad;
const double omega_ramp = 4300*rpm2rad;

const double ramp_up_max_count = 2.5/Ts;	// sets ramp-up time to be 2.5 seconds

// QUADROTOR SAFETY VALUE LIMITATIONS
const double Xkill = 1.50;
const double Ykill = 2.00;
const double Zkill = 2.50;
const double OXkill = 1.5;
const double OYkill = 1.5;


// Returns the name of the mode as a string
std::string modeString(FlightMode mode)
{
	switch(mode)
	{
		// Flight/Control Modes
		case QUAD_OFF:			return "Off";
		case QUAD_IDLE: 		return "Idle";		
		case QUAD_RAMPUP:  		return "Ramping Up";
		case QUAD_TAKEOFF: 		return "Takeoff";
		case QUAD_HOVER:  		return "Hover";
		case QUAD_WAYPOINT: 	return "Waypoint";
		case QUAD_VELOCITY:   	return "Velocity";
		case QUAD_FIGURE_8:     return "Figure 8";
		case QUAD_FOLLOW_REF: 	return "Follow Ref";
		case QUAD_HEAD_HOME:   	return "Head Home";
		case QUAD_LAND: 		return "Land";
		case QUAD_UNSAFE:  		return "Unsafe";
		case QUAD_EXITING:  	return "Exiting";

		// Testing Modes
		case QUAD_CYCLE:  		return "Cycle";
		case QUAD_LATENCY_TEST: return "Latency Test";
		case QUAD_DMC:  		return "Direct Motor Ctrl.";
		case QUAD_ATT_CTRL: 	return "Attitude Ctrl.";
		case QUAD_HEIGHT_CTRL:	return "Height Ctrl.";
		case QUAD_POS_CTRL:		return "Position Ctrl.";

		case QUAD_UNDEFINED: 	
		default: 				return "Undefined";
	}
	return "modeString() FAILED"; // in case the switch statement failed
}
