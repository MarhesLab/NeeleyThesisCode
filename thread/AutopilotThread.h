#ifndef AUTOPILOT_THREAD_H
#define AUTOPILOT_THREAD_H

#include <pthread.h>	// pthread multithreading
#include <stdio.h> 		// FILE
#include "../defs.h"	// IMU_STATES
#include "../functions/Timer.h"

// The functions to be transferred to their own threads
void* autopilotThreadFunction(void *threadarg);
void* autopilotWatchdogFunction(void *threadarg);

// Struct needed for passing data to the main thread
struct AutopilotStruct 
{
	//pthread_mutex_t* struct_imu_mutex;
	//pthread_mutex_t* struct_status_mutex;
	//pthread_mutex_t* struct_echo_mutex;
	unsigned char* struct_imu_timestamp;
	unsigned char* struct_status_timestamp;
	FILE* struct_ap_read;
	double* struct_data;
	short* struct_ap_status;
	double* struct_voltage;
	double* struct_cpu_load;
	double* struct_motor_rpm;
	char* struct_echo_buffer;
	char* struct_version_buffer;
	bool* struct_imu_updated;
	bool* struct_status_updated;
	bool* struct_echo_updated;
	bool* struct_version_updated;
	bool* struct_ap_active;
	bool* struct_running;
	Timer* struct_timer;
};


// Struct needed for passing data to the watchdog thread
struct watchdogStructAP
{
	bool* struct_active;
	bool* struct_running;
	Timer* struct_timer;
};


// Autopilot Thread class object definition
class AutopilotThread 
{
  protected:
	pthread_t main_thread;		// Pthread object for main function thread
	pthread_t watchdog_thread;
	
	unsigned char imu_timestamp;	// frame timestamp values
	unsigned char status_timestamp;	
	
	FILE* ap_read;							// FILE pointer used to read from the port connected to the Autopilot
	FILE* ap_write;
	
	double imu_data[IMU_STATES];		// IMU output values
	
	short ap_status;
	double battery_voltage;					// Autopilot status-related values
	double cpu_load;
	double motor_rpm[MOTORS];
	short ap_status_buffer;
	double battery_voltage_buffer; // status buffer values
	double cpu_load_buffer;
	double motor_rpm_buffer[MOTORS];

	char echo_chars[5];
	char echo_buffer[5];
	char version[12];
	char version_buffer[12];
	
	bool imu_updated;				// Boolean flag values for getting updates from the thread
	bool status_updated;
	bool echo_updated;
	bool version_updated;
	bool ap_active;
	bool running;

	Timer watchdogTimer;
	
	struct AutopilotStruct astruct;			// struct for passing data to the thread
	struct watchdogStructAP wdstruct;
	
  public:
	// Constructors and Destructors
    AutopilotThread();
	~AutopilotThread();
	
	// Initialization calls
	void initialize();
	bool startThreads();

	// Termination calls
	void startShutdown();
	void kill();

	// Connection status
	bool connectionActive();
	
	// IMU values
	bool newMeasurement();
	void getMeasurement(double states[]);
	unsigned char getmeasurementTimestamp();
	
	// Autopilot/Quadrotor status values
	bool newStatus();
	short getAPStatus();
	double getBatteryVoltage();
	double getCPULoadPercentage();
	void getMotorRPMs(double motorRPMs[]);
	
	// Serial connection echo testing
	bool newEcho();
	void getEcho(char fiveChars[]);

	// HL controller program version
	bool newVersion();
	void getVersion(char twelveChars[]);

	// Computer -> Autopilot command functions
	void sendSpeeds(double omega[]);
	void sendDMCs(unsigned char DMCs[]);
	void sendStopCommand();
	void sendGPIOState(int on);
	void sendGPIOToggle();
	void sendHLVersionRequest();
	void sendEcho(char fiveChars[]);
};



#endif
