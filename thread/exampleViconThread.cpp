#include <iostream>
#include <string>

#include "ViconThread.h"
#include "../defs.h"
#include "../functions/Timer.h"

//#define VICON_IP "192.168.0.120:801"

using namespace std;

int main()
{
	// Timer for checking connection status
	Timer statusTimer;

	// Vicon measurements	
	double vicon_values[VIC_STATES];
	
	// name of Vicon model to track
	string name = "quadrotor_1b";

	// Thread object creation
	ViconThread vthread(name);
	
	// Thread initialization and start
	vthread.initialize(VICON_IP);
	vthread.startThreads();
	
	// Starting the timer by default
	statusTimer.tic();

	while(1)
	{
		// checks if a new set of Vicon measurements has been received; returns true if so
		if(vthread.newMeasurement())
		{
			// checks if incoming measurements are good/valid
			if( vthread.measurementValid() )
				cout << "Object has been seen!\n\r";
			else
				cout << "OBJECT NOT SEEN\n\r";

			// gets the new measurement values and returns them via the function call
			vthread.getMeasurement(vicon_values);
			
			cout << "Received Vicon values: \n\r";
			
			for(int i = 0; i < VIC_STATES; i++)
				cout << vicon_values[i] << "\n\r";
				
			cout << "\n\r";
		}

		// Checks the status of the connection every second or so
		statusTimer.toc();
		if(statusTimer.secs() > 1 ) {
			statusTimer.tic();

			if( vthread.connActive() )
				cout << "Connection active!" << endl;
			else
				cout << "Connection not active!" << endl;
		}
	}

	// call this as soon as the shutdown process starts to give the thread ample time to shut down on its own
	vthread.startShutdown();

	// call this to completely kill the thread
	vthread.kill();	

	return 0;
}
