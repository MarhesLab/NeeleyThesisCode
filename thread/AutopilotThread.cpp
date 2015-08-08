/*
	For more information on multithreading, see the PDF titled
	"POSIX Threading Programming Resource" in this folder.
*/

#include <iostream>	// for cout
#include <unistd.h> // for usleep

#include "AutopilotThread.h"
#include "../functions/autopilot.h"

using namespace std;

// Mutex Definitions
pthread_mutex_t imu_mutex;
pthread_mutex_t status_mutex;
pthread_mutex_t echo_mutex;
pthread_mutex_t version_mutex;


// Constructor
// Requires an already-initialized serial port
AutopilotThread::AutopilotThread()
{
	// receives the already-initialized serial port
	ap_read = NULL;

	// zeros out some of the values
	imu_timestamp = 0;
	status_timestamp = 0;
	battery_voltage = 0;
	cpu_load = 0;

	// initializes Boolean values
	imu_updated = false;
	status_updated = false;
	echo_updated = false;
	version_updated = false;
	ap_active = false;
	running = true;
	
	watchdogTimer.tic();
}


// Destructor
// terminates the thread and closes the serial port
AutopilotThread::~AutopilotThread()
{
	// flags the thread for shutdown (if it wasn't done beforehand)
	running = false;
	
	// forcibly shuts down the Autopilot thread; as the quadrotor battery may die mid-flight
	// and cause the thread to continuously block as it tries to read from the port.
	pthread_cancel(main_thread);
	pthread_cancel(watchdog_thread);
	
	// closes the serial port
	fclose(ap_read);
	fclose(ap_write);
}


// Starts the Ports and Populates the AutopiloStruct object
void AutopilotThread::initialize()
{
	// Serial port initialization
	if(!initSerialPort(&ap_read, false))
		exit(1);
	if(!initSerialPort(&ap_write, false))
		exit(1);

	// Main thread struct population
	astruct.struct_imu_timestamp = &imu_timestamp;
	astruct.struct_status_timestamp = &status_timestamp;
	astruct.struct_ap_read = ap_read;

	astruct.struct_data = imu_data;

	astruct.struct_ap_status = &ap_status;
	astruct.struct_voltage = &battery_voltage_buffer;
	astruct.struct_cpu_load = &cpu_load_buffer;
	astruct.struct_motor_rpm = motor_rpm_buffer;

	astruct.struct_echo_buffer = echo_buffer;

	astruct.struct_version_buffer = version_buffer;

	astruct.struct_imu_updated = &imu_updated;
	astruct.struct_status_updated = &status_updated;
	astruct.struct_echo_updated = &echo_updated;
	astruct.struct_version_updated = &version_updated;

	astruct.struct_ap_active = &ap_active;
	astruct.struct_running = &running;
	astruct.struct_timer = &watchdogTimer;

	// Watchdog thread struct population
	wdstruct.struct_active = &ap_active;
	wdstruct.struct_running = &running;
	wdstruct.struct_timer = &watchdogTimer;
}


// Actually starts the threaded process
bool AutopilotThread::startThreads()
{
	if(pthread_create(&main_thread, NULL, autopilotThreadFunction, (void *)&astruct))
	{
		std::cout << "Could not create Autopilot thread. \n\r";
		return false;
	}

	if(pthread_create(&watchdog_thread, NULL, autopilotWatchdogFunction, (void *)&wdstruct))
	{
		std::cout << "Could not create Autopilot watchdog thread. \n\r";
		return false;
	}
	
	return true;
}


// Flags the thread for peaceful termination, if it can;
// otherwise the destructor will forcefully cancel the thread.
void AutopilotThread::startShutdown()
{
	running = false;
}

void AutopilotThread::kill()
{
	pthread_cancel(main_thread);
	pthread_cancel(watchdog_thread);
}


// Checks to see if the serial communication with the Autopilot is active
bool AutopilotThread::connectionActive()
{
	return ap_active;
}


// Checks to see if a new measurement has been received
bool AutopilotThread::newMeasurement()
{
	return imu_updated;
}


// Passes the measurement data to states[]; best called when "updated" is true
void AutopilotThread::getMeasurement(double states[])
{
	// resets the "updated" indicator
	imu_updated = false;
	
	// locks the mutex, then copies the data to "states"
	pthread_mutex_lock(&imu_mutex);
	for(int i = 0; i < IMU_STATES; i++)
		states[i] = imu_data[i];
	pthread_mutex_unlock(&imu_mutex);
}


// Returns the timestamp value from the Autopilot
unsigned char AutopilotThread::getmeasurementTimestamp()
{
	return imu_timestamp;
}


// Checks to see if new status values have been received
bool AutopilotThread::newStatus()
{
	// preserves the result before potentially changing it
	bool result = status_updated;

	// if it's been updated, get those updates
	if(status_updated)
	{
		// resets the update flag
		status_updated = false;

		// locks the data before reading it, then unlocks it
		pthread_mutex_lock(&status_mutex);
		ap_status = ap_status_buffer;
		battery_voltage = battery_voltage_buffer;
		cpu_load = cpu_load_buffer;
		for(int i = 0; i < MOTORS; i++)
			motor_rpm[i] = motor_rpm_buffer[i];
		pthread_mutex_unlock(&status_mutex);
	}

	// returns whether or not the status was updated
	return result;
}


short AutopilotThread::getAPStatus()
{
	return ap_status;
}


// Gets the battery voltage 
double AutopilotThread::getBatteryVoltage()
{
	return battery_voltage;
}


// Gets the CPU load percentage
double AutopilotThread::getCPULoadPercentage()
{
	return cpu_load;
}


// Gets the measured motor RPMs
void AutopilotThread::getMotorRPMs(double motorRPMs[])
{
	for(int i = 0; i < MOTORS; i++)
		motorRPMs[i] = motor_rpm[i];
}


// Checks for a new echo frame, then gets the updated values if there was
bool AutopilotThread::newEcho()
{
	bool result = echo_updated;

	if(echo_updated)
	{
		echo_updated = false;
		
		pthread_mutex_lock(&echo_mutex);
		for(int i = 0; i < 5; i++)
			echo_chars[i] = echo_buffer[i];

		/*cout << "Frame received (update): ";
		for(int i = 0; i < 4; i++)
			cout << (unsigned int)echo_buffer[i] << ",\t";
		cout << (unsigned int)echo_buffer[4] << "\n\r";*/
		pthread_mutex_unlock(&echo_mutex);		
	}

	return result;
}

void AutopilotThread::getEcho(char fiveChars[])
{
	for(int i = 0; i < 5; i++)
		fiveChars[i] = echo_chars[i];
}


// Checks for a new version frame, then gets the updated values if there was
bool AutopilotThread::newVersion()
{
	bool result = version_updated;

	if(version_updated)
	{
		version_updated = false; 

		pthread_mutex_lock(&version_mutex);
		for(int i = 0; i < 12; i++)
			version[i] = version_buffer[i];
		pthread_mutex_unlock(&version_mutex);
	}

	return result;
}

void AutopilotThread::getVersion(char twelveChars[])
{
	for(int i = 0; i < 12; i++)
		twelveChars[i] = version[i];
}


// Sends desired motor speeds (in radians per second) to the quadrotor
void AutopilotThread::sendSpeeds(double omega[])
{	setMotorSpeeds(ap_write, omega, false); }

void AutopilotThread::sendDMCs(unsigned char DMCs[])
{	setMotorChars(ap_write, DMCs, false); }

void AutopilotThread::sendStopCommand()
{	stopMotors(ap_write); }

void AutopilotThread::sendGPIOState(int on)
{	setGPIO(ap_write, on); }

void AutopilotThread::sendGPIOToggle()
{	toggleGPIO(ap_write); }

void AutopilotThread::sendHLVersionRequest()
{	getHLVersion(ap_write); }

void AutopilotThread::sendEcho(char fiveChars[])
{	sendEchoFrame(ap_write, fiveChars, false);}


// The actual function that gets sent to its own thread. This function starts a loop that
// continually scans for Autopilot frames, then processes them appropriately.
void* autopilotThreadFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct AutopilotStruct* astruct;
	astruct = (struct AutopilotStruct *) threadarg;
	
	// declaring appropriate variables
	unsigned char* imu_timestamp 		= astruct->struct_imu_timestamp;
	unsigned char* status_timestamp 	= astruct->struct_status_timestamp;
	FILE* ap_read 						= astruct->struct_ap_read;
	double* imu_data 					= astruct->struct_data;
	short* ap_status					= astruct->struct_ap_status;
	double* batt_voltage 				= astruct->struct_voltage;
	double* cpu_load 					= astruct->struct_cpu_load;
	double* motor_rpm					= astruct->struct_motor_rpm;
	char* echo_buffer					= astruct->struct_echo_buffer;
	char* version_buffer				= astruct->struct_version_buffer;
	bool* imu_updated 					= astruct->struct_imu_updated;
	bool* status_updated 				= astruct->struct_status_updated;
	bool* echo_updated					= astruct->struct_echo_updated;
	bool* version_updated				= astruct->struct_version_updated;
	bool* ap_active						= astruct->struct_ap_active;
	bool* running 						= astruct->struct_running;
	Timer* watchdogTimer				= astruct->struct_timer;
	
	// creating temporary values for frame checking
	unsigned char frame[12], frametype, frametimestamp;
	
	// Keep getting measurements/status updates until flagged to stop
	while(*running)
	{		
		getAutopilotFrame(ap_read, frame, frametype, frametimestamp);

		*ap_active = true;
		watchdogTimer->tic();

		if(frametype == IMUFRAME)
		{
			pthread_mutex_lock(&imu_mutex);
			*imu_timestamp = frametimestamp;
			processImuFrame(frame, imu_data);
			pthread_mutex_unlock(&imu_mutex);
			
			*imu_updated = true;
		}
		else if(frametype == STATUSFRAME)
		{
			pthread_mutex_lock(&status_mutex);
			*status_timestamp = frametimestamp;
			processStatusFrame(frame, *ap_status, *batt_voltage, *cpu_load, motor_rpm);
			pthread_mutex_unlock(&status_mutex);
			
			*status_updated = true;
		}
		else if(frametype == ECHOFRAME)
		{
			pthread_mutex_lock(&echo_mutex);
			processEchoFrame(frame, echo_buffer);
			/*cout << "Frame received (buffer): ";
			for(int i = 0; i < 4; i++)
				cout << (unsigned int)echo_buffer[i] << ",\t";
			cout << (unsigned int)echo_buffer[4] << "\n\r";*/
			pthread_mutex_unlock(&echo_mutex);

			*echo_updated = true;
		}
		else if(frametype == VERSIONFRAME)
		{
			pthread_mutex_lock(&version_mutex);
			processVersionFrame(frame, version_buffer);
			pthread_mutex_unlock(&version_mutex);

			*version_updated = true;
		}
	}
	
	// Kill the thread
	pthread_exit(NULL);
}


// This watchdog thread is responsible for making sure the Autopilot is
// regularly sending frames. If it isn't, it'll change the status of the
// connection.
void* autopilotWatchdogFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct watchdogStructAP* wdstruct;
	wdstruct = (struct watchdogStructAP *) threadarg;

	// declaring appropriate variables
	bool* active 			= wdstruct->struct_active;
	bool* running			= wdstruct->struct_running;
	Timer* watchdogTimer	= wdstruct->struct_timer;

	while(*running)
	{
		// if it's been more than 0.25 seconds since last update...
		watchdogTimer->toc();
		if(watchdogTimer->millis() >= 250.0)
			// flag that the connection is lost.
			*active = false;

		// sleep for 100ms, then check again.
		usleep(100000);
	}

	// Kill the thread
	pthread_exit(NULL);
}
