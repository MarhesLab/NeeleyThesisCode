#ifndef VICON_THREAD_H
#define VICON_THREAD_H

#include <string>
#include <pthread.h>

#include "../defs.h"
#include "../functions/Timer.h"
#include "../vicon/ViconClass.h"

//#define VICON_VALS 6

// The functions to be transferred to their own threads
void* viconThreadFunction(void *threadarg);
void* viconWatchdogFunction(void *threadarg);

// Struct needed for passing data to the main thread
struct ViconStruct 
{
	//pthread_mutex_t* struct_mutex;
	ViconClass* struct_vicon;
	std::string struct_name;
	bool* struct_valid;
	double* struct_data;
	bool* struct_updated;
	bool* struct_active;
	bool* struct_running;
	Timer* struct_timer;
};


// Struct needed for passing data to the watchdog thread
struct watchdogStructVic
{
	bool* struct_active;
	bool* struct_running;
	Timer* struct_timer;
};


// Vicon Thread class object definition
class ViconThread 
{
  protected:
	pthread_t main_thread;		// Pthread object for main thread
	pthread_t watchdog_thread;
	
	ViconClass* vicon;			// ViconClass object 
	std::string name;			// Object's Vicon model name
	
	bool valid;					// Indicates if measurements are valid (object was seen)
	bool valid_buffer;
	double values[VIC_STATES];	// position and attitude measurement values; 
	double values_buffer[VIC_STATES]; // buffer values for passing between threads
	bool updated;		// Boolean value indicating measurements have been updated
	bool active;
	bool running;		// Boolean flag used to tell the thread to shut down on its own
	
	Timer watchdogTimer;

	struct ViconStruct vstruct;		// Struct necessary to pass data to the thread
	struct watchdogStructVic wdstruct;
	
  public:
	ViconThread(std::string vicon_name);
	~ViconThread();
	
	void initialize(const char vicon_ip[]);	// starts the viconClass object and populates the ViconStruct object
	bool startThreads(); // actually starts the Vicon thread; returns true if it worked
	
	bool connActive(); // checks if it's received a value recently

	bool newMeasurement();	// returns true if there's new measurements
	void getMeasurement(double states[]); // gives measurements to passed-in array
	bool measurementValid();	// returns true if current measurements are valid;
								// returns false if object was not seen by Vicon.
	
	void startShutdown();		// flags the thread for termination
	void kill();
};

#endif
