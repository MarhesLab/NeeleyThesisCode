/*
	For more information on multithreading, see the PDF titled
	"POSIX Threading Programming Resource" in this folder.
*/

#include <iostream>

#include "ViconThread.h"

using namespace std;

// Mutex Declarations
pthread_mutex_t vicon_mutex;

// Constructor
ViconThread::ViconThread(string vicon_name)
{
	
	// copies over the object's Vicon model name	
	name = vicon_name;
	
	// initializes Boolean values
	valid = false;
	updated = false;
	active = false;
	running = true;

	watchdogTimer.tic();
}


// Destructor
ViconThread::~ViconThread()
{
	// flags the thread for shutdown (if it wasn't done beforehand)
	running = false;
	
	// Shuts down the watchdog thread
	pthread_cancel(watchdog_thread);	

	// joins the main thread and waits for it to shut down
	void *status;
	pthread_join(main_thread, &status);
	
	// deletes the ViconClass object
	delete vicon;
}


// Starts the ViconClass object and populates the ViconStruct object
void ViconThread::initialize(const char vicon_ip[])
{
	// creates the ViconClass object
	vicon = new ViconClass(vicon_ip);
	
	// starts stuffing the ViconStruct
	//vstruct.struct_mutex = &the_mutex;
	vstruct.struct_vicon = vicon;
	vstruct.struct_name  = name;
	vstruct.struct_valid = &valid_buffer;
	vstruct.struct_data  = values_buffer;
	vstruct.struct_updated = &updated;
	vstruct.struct_active = &active;
	vstruct.struct_running = &running;
	vstruct.struct_timer = &watchdogTimer;

	// Populates the watchdog struct
	wdstruct.struct_active = &active;
	wdstruct.struct_running = &running;
	wdstruct.struct_timer = &watchdogTimer;
}


// Actually starts the threaded process
bool ViconThread::startThreads()
{
	if(pthread_create(&main_thread, NULL, viconThreadFunction, (void *)&vstruct))
	{
		cout << "Could not create main Vicon thread. \n\r";
		return false;
	}
	
	if(pthread_create(&watchdog_thread, NULL, viconWatchdogFunction, (void *)&wdstruct))
	{
		cout << "Could not create Vicon watchdog thread. \n\r";
		return false;
	}

	return true;
}


// Returns the activity status of the Vicon thread
bool ViconThread::connActive()
{
	return active;
}


// Checks to see if a new measurement has been received
bool ViconThread::newMeasurement()
{
	bool result = updated;

	if(updated)
	{
		// resets the "updated" indicator
		updated = false;
		
		// locks the mutex, then copies the data to "states"
		pthread_mutex_lock(&vicon_mutex);
		valid = valid_buffer;
		for(int i = 0; i < VIC_STATES; i++)
			values[i] = values_buffer[i];
		pthread_mutex_unlock(&vicon_mutex);
	}

	return result;
}


// Passes the measurement data to states[]; best called when "updated" is true
void ViconThread::getMeasurement(double states[])
{
	for(int i = 0; i < VIC_STATES; i++)
		states[i] = values[i];
}


// Returns whether or not the measurements are valid
bool ViconThread::measurementValid()
{
	return valid;
}


// Flags the thread for termination; the earlier it's done, the sooner the thread will terminate
void ViconThread::startShutdown()
{
	running = false;
}


// Outright kills the thread
void ViconThread::kill()
{
	pthread_cancel(main_thread);
}


// The actual function that gets sent to its own thread. This function starts a loop that
// continually checks for Vicon updates, then gets the latest values. 
void* viconThreadFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct ViconStruct *vstruct;
	vstruct = (struct ViconStruct *) threadarg;
	
	// declaring appropriate variables
	//pthread_mutex_t* vicon_mutex = vstruct->struct_mutex;
	ViconClass* vicon = vstruct->struct_vicon;
	std::string name = vstruct->struct_name;
	bool* validValues = vstruct->struct_valid;
	double* viconValues = vstruct->struct_data;
	bool* updated = vstruct->struct_updated;
	bool* active = vstruct->struct_active;
	bool* running = vstruct->struct_running;
	Timer* watchdogTimer = vstruct->struct_timer;
	
	// Keep getting measurements until flagged to stop
	while(*running)
	{
		// Blocks the thread until a new Vicon measurement is obtained
		vicon->update();
		
		// Verifies the thread is still active
		watchdogTimer->tic();
		*active = true;		

		// Locks the mutex, updates the measurements, then unlocks the mutex
		pthread_mutex_lock(&vicon_mutex);
		*validValues = vicon->subject_info_valid(name);
		vicon->get_subject_position_and_quaternion(name,viconValues);
		pthread_mutex_unlock(&vicon_mutex);

		// Indicates the measurements were updated
		*updated = true;
	}
	
	// Kill the thread
	pthread_exit(NULL);
}


// This watchdog thread is responsible for making sure the program is
// regularly getting Vicon frames. If it isn't, it'll change the status
// of the connection.
void* viconWatchdogFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct watchdogStructVic* wdstruct;
	wdstruct = (struct watchdogStructVic *) threadarg;

	// declaring appropriate variables
	bool* active = wdstruct->struct_active;
	bool* running = wdstruct->struct_running;
	Timer* watchdogTimer = wdstruct->struct_timer;

	while(*running)
	{
		// if it's been more than 0.1 seconds since the last update...
		watchdogTimer->toc();
		if(watchdogTimer->millis() >= 100.0)
			// flag that the conneciton is lost.
			*active = false;
	
		// sleep for 50 ms, then check again.
		usleep(50000);
	}

	// if running -> false, kill the thread
	pthread_exit(NULL);
}
