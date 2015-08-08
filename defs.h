#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>

// For the actual values of these variables, see
// "defs.cpp"

// IP ADDRESS DEFINITIONS
extern char VICON_IP[];
extern const char GCS_IP[];
extern const char QUAD1_IP[];
extern const char QUAD2_IP[];
extern const char QUAD3_IP[];


// SERIAL PORT FOR AUTOPILOT
extern const std::string AP_SERIAL;
extern const std::string BAUD_RATE;


// LATENCY COMPENSATION ARRAY SIZING CONSTANTS
// I know, this section gets confusing. 
// variables declared in time steps (based on "Ts")
#define VIC_LAT 0 // Vicon measurement latency; sets the number of time steps in the
				  // past that the state vectors need to be tracked so they can be
				  // meshed appropriately with incoming, lagged Vicon measurements 
				  // for proper filtering
#define MOT_LAT 0 // motor response latency; sets the number of time steps in the
				  // future that need to be predicted for control law purposes, so,
				  // after latency, the input will be executed right when it's wanted.

#define TIME_VIC 1         // holds index matching with latest Vicon measurement
#define TIME_OLD 0+VIC_LAT // holds index of "last/old" state
#define TIME_CUR 1+VIC_LAT // holds index of "present/current" state
#define TIME_NEW 1+VIC_LAT+MOT_LAT // holds index of "future" state that's used for control laws

#define TIME_IMU 2+VIC_LAT // defines the number of IMU measurement vectors to be tracked for latency compensation purposes
#define TIME_TOT 2+VIC_LAT+MOT_LAT // defines the total number of state vectors that need to be kept for latency compensation/prediction purposes


// ARRAY SIZING CONSTANTS
// declared using #define to avoid cross-compilation issues
#define STATES 13 		// position (3), velocity (3), quaternion (4), and angular velocity (3)
#define INPUTS 4 		// thrust (1) and torques (3)
#define MOTORS 4 		// quadrotor has 4 motors
#define IMU_STATES 6 	// angular velocity (3) and linear acceleration (3)
#define VIC_STATES 7 	// position (3) and quaternion (4)
#define POS_REFS 18 	// five references each for X, Y, and Z, and 3 for heading/yaw 


// GENERAL CONSTANTS
extern const double Ts;
extern const double G;


// CONVERSIONS
extern const double pi;
extern const double rad2rpm;  // radians/second to rpm
extern const double rpm2rad; // rpm to radians/second
extern const double deg2rad;
extern const double rad2deg;


// STATE AND INPUT INDEX CONSTANTS
extern const int  X,   Y,   Z,  DX,  DY,  DZ;
extern const int Q0,  QI,  QJ,  QK;
extern const int WX,  WY,  WZ;
extern const int  X_t, DX_t, DDX_t, DDDX_t, DDDDX_t;
extern const int  Y_t, DY_t, DDY_t, DDDY_t, DDDDY_t;
extern const int  Z_t, DZ_t, DDZ_t, DDDZ_t, DDDDZ_t;
extern const int PSI_t, DPSI_t, DDPSI_t;
extern const int  Xv,  Yv,  Zv, Q0v, QIv, QJv, QKv;
extern const int WXi, WYi, WZi, AXi, AYi, AZi; 
extern const int  Th,  Tx,  Ty,  Tz;
extern const int Mfront, Mback, Mleft, Mright; 


// MOTOR/PROPELLER SPEED VALUES
extern const unsigned char maxDMCchar;
extern const double omega_min;
extern const double omega_max;
extern const double omega_sq_min;
extern const double omega_sq_max;

extern const double omega_idle;
extern const double omega_ramp;

extern const double ramp_up_max_count;


// QUADROTOR SAFETY VALUE LIMITATIONS
extern const double Xkill;
extern const double Ykill;
extern const double Zkill;
extern const double OXkill;
extern const double OYkill;


// ENUMERATIONS

// Flight Mode Declarations
// (values don't matter, as long as they're set to something specific)
// (DON'T CHANGE THE VALUES ONCE THEY'RE SET; otherwise, the log files
//  will no longer be consistent)
//
// See "Mode_Documentation.txt" for a detailed explanation of each of the modes.
enum FlightMode {
	// Flight/Control Modes
	QUAD_OFF = 0,
	QUAD_IDLE = 1, 
	QUAD_RAMPUP = 2, 
	QUAD_TAKEOFF = 3,
	QUAD_HOVER = 4, 
	QUAD_WAYPOINT = 5,  
	QUAD_VELOCITY = 6,
	QUAD_FIGURE_8 = 19,
	QUAD_SPECIAL  = 20,
	QUAD_FOLLOW_REF = 7,
	QUAD_HEAD_HOME = 8,  
	QUAD_LAND = 9, 
	QUAD_UNSAFE = 10, 
	QUAD_EXITING = 11, 
	
	// Testing Modes
	QUAD_CYCLE = 12, 
	QUAD_LATENCY_TEST = 13,  
	QUAD_DMC = 14, 
	QUAD_ATT_CTRL = 15,
	QUAD_HEIGHT_CTRL = 16,
	QUAD_POS_CTRL = 18,
	
	QUAD_UNDEFINED = 17
};

// Function for returning the flight mode as a string
std::string modeString(FlightMode mode);

// Connection Status Declarations
// (values doen't matter, as long as they're set to something specific)
enum ConnStatus {
	CONN_DISCONNECTED = 0,
	CONN_CONNECTING = 1,
	CONN_CONNECTED = 2
};


// Filter Type Declarations
// (values don't matter, as long as they're set to something specific)
enum FilterType {
	FILT_KALMAN = 0,		// Regular, pre-linearized Kalman filter
	FILT_EXT_KALMAN = 1,	// Extended Kalman filter; relinearizes at each iteration
	FILT_QUICK = 2			// Quick filter based on ETH's "Flying Machine Arena" paper
};

#endif
