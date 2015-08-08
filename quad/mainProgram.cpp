// Standard Libraries
#include <iostream>				// for outputting text
#include <sys/resource.h>		// for setting program priority

// Custom Classes/Functions/Definitions
#include "Quadrotor.h"
#include "../defs.h"
#include "../functions/Timer.h"
#include "../thread/LoggerThread.h"

// Using standard name space to avoid having to use "std::" everywhere
using namespace std;

int main()
{
	/* 
		Sets the program's scheduling priority to be slightly above average;
		from "http://linux.die.net/man/3/setpriority":

		"The nice value set with setpriority() shall be applied to the process. 
		If the process is multi-threaded, the nice value shall affect all system 
		scope threads in the process."
	*/ 
	int which = PRIO_PROCESS;
	id_t pid = getpid();
	int priority = -1; // sets the process's "niceness" (a Linux thing), 
						// which goes between -20 and 19; -20 = highest priority,
						// 19 = lowest priority, 0 = default priority

	int ret = setpriority(which, pid, priority);
	if(ret == -1) {
		cout << "\n\rERROR: Program priority was not set successfully." << endl
			 << "Try running the program as root. Exiting...\n" << endl;
		exit(-1);
	}

	ret = getpriority(which, pid);

	if(ret == priority)
		cout << "Program priority was set successfully!" << endl
			 << "Niceness: " << ret << endl;
	else {
		cout << "Program priority was not set successfully! :(" << endl
			<< "Desired niceness value: " << priority << endl
			<< "Set niceness value: " << ret << endl
			<< "Better luck next time! Exiting..." << endl;
		exit(-1);
	}
		


	// Quadrotor object declaration
	// Note that the quadrotor object has multiple subclasses that 
	// spawn their own threads
	Quadrotor quad(0);


	// Timer object declarations
	Timer loopTimer;
	Timer broadcastTimer;

	// Program Runtime Timers (for evaluation purposes)
	/*LoggerThread timingLog;
	timingLog.setName("timinglog.csv");
	timingLog.initialize();
	if(!timingLog.startThread())
		exit(-1);
	string titleLine = string("measurementMax,gcsSend,filtering,latencyComp,") +
		string("flightMode,reference,controlLaw,inputs,logging,nextloop,") + 
		string("timinglogging\n");
	timingLog.addTextLine(titleLine);
	Timer measTimer, gcsTimer, filterTimer, latencyTimer, modeTimer,
		  refTimer, controlTimer, inputTimer, loggingTimer, nextTimer, timingTimer;
	timingTimer.tic();
	timingTimer.toc();
	double measTimeMax = 0;*/


	// Initialization
	quad.initialize();
	quad.startLogging();

	
	// Starting the main loop
	loopTimer.tic();
	broadcastTimer.tic();
	while(true)
	{
		// Tries to get incoming Vicon, Autopilot, and GCS data
		// measTimer.tic();
		quad.updateFromThreads();
		// measTimer.toc();
		// if(measTimer.micros() > measTimeMax)
		// 	measTimeMax = measTimer.micros();
	
		// Periodically sends information back to the GCS
		broadcastTimer.toc();
		if(broadcastTimer.millis() >= 100.0)
		{
			broadcastTimer.tic();
			//gcsTimer.tic();
			quad.updateGCS();
			//gcsTimer.toc();
		}

		// Regularly executes the main control loop
		loopTimer.toc();
		if(loopTimer.secs() >= Ts) // Ts set in "../defs.cpp"
		{
			loopTimer.tic();
			
			//filterTimer.tic();
			quad.updateStates();		// filters state data with measurements
			//filterTimer.toc();

			//latencyTimer.tic();
			quad.latencyCompensation();	// compensates for system latency
			//latencyTimer.toc();

			//modeTimer.tic();
			quad.getFlightMode();		// determines current flight mode
			//modeTimer.toc();

			//refTimer.tic();
			quad.setReference();		// sets reference values for given flight mode
			//refTimer.toc();

			//controlTimer.tic();
			quad.controlLaw();			// executes control law for given flight mode
			//controlTimer.toc();

			//inputTimer.tic();
			quad.sendInputs();			// sends appropriate motor inputs
			//inputTimer.toc();

			//loggingTimer.tic();
			quad.logValues();			// logs various program variables
			//loggingTimer.toc();

			//nextTimer.tic();
			quad.prepareForNextLoop();	// gets ready for the next control loop
			//nextTimer.toc();

			/*timingTimer.tic();
			timingLog.logDouble(          measTimeMax,6);
			timingLog.logDouble(    gcsTimer.micros(),6);
			timingLog.logDouble( filterTimer.micros(),6);
			timingLog.logDouble(latencyTimer.micros(),6);
			timingLog.logDouble(   modeTimer.micros(),6);
			timingLog.logDouble(    refTimer.micros(),6);
			timingLog.logDouble(controlTimer.micros(),6);
			timingLog.logDouble(  inputTimer.micros(),6);
			timingLog.logDouble(loggingTimer.micros(),6);
			timingLog.logDouble(   nextTimer.micros(),6);
			timingLog.logDouble( timingTimer.micros(),6);
			timingLog.newLine();
			timingTimer.toc();

			measTimeMax = 0;*/
		}

		// Takes a small break; reduces unnecessary CPU load, and allows the other 
		// various threads (found in the Quadrotor object) to perform their operations
		usleep(10);
	}
	
	return 0;
}
