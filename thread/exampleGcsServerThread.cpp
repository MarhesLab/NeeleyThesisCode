#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "GcsServerThread.h"
#include "../defs.h"
#include "../functions/Timer.h"

using namespace std;

// Class Pointer Declaration (by starting it as a pointer,
// it makes it easy to call the constructor once the variables
// have been declared)
GcsServerThread* gcsThread;

// Transmitted Values
double XqTrajDesired[POS_REFS];
double XqRefDesired[STATES];
unsigned char newDMCs[MOTORS];
FlightMode desiredMode;

// Received Values
double Xq[STATES];
double XqR[STATES];
double Uq[INPUTS];
double XqVicon[VIC_STATES];
double XqImu[IMU_STATES];
unsigned char motorDMCs[MOTORS];
short ap_status;
double batt_voltage;
double cpu_load;
ConnStatus uart_status;
ConnStatus vicon_status;
FlightMode currentMode;

// Timer object for keeping track of loop timing
Timer sendTimer;

int main()
{
	// Initialize values to be transmitted
	for(int i = 0; i < POS_REFS; i++)
		XqTrajDesired[i] = i;
	for(int i = 0; i < STATES; i++)
		XqRefDesired[i] = i;
	for(int i = 0; i < MOTORS; i++)
		newDMCs[i] = i + 1;
	desiredMode = QUAD_CYCLE;

	// Create the class object
	gcsThread = new GcsServerThread(XqTrajDesired, XqRefDesired, newDMCs, &desiredMode);

	// Initialize, Listen for a client, and Start the thread
	gcsThread->initialize();
	gcsThread->listenAndAccept();
	gcsThread->startThread();

	sendTimer.tic();
	while(true)
	{
	    if(gcsThread->gotFrame())
	    {
	        // Update values
	        gcsThread->getStates(Xq);
			gcsThread->getReferences(XqR);
	        gcsThread->getInputs(Uq);
	        gcsThread->getVicon(XqVicon);
	        gcsThread->getImu(XqImu);
	        gcsThread->getDMCs(motorDMCs);
			ap_status    = gcsThread->getAutopilotStatus();
	        batt_voltage = gcsThread->getVolts();
	        cpu_load     = gcsThread->getCPU();
			uart_status  = gcsThread->getSerialStatus();
			vicon_status = gcsThread->getViconStatus();
	        currentMode  = gcsThread->getMode();

			cout << setprecision(5) << "\n"
				<< "Battery Voltage: " << batt_voltage << "\n\r"
				<< "CPU Load: " << cpu_load << "\n\r";
	    }

		sendTimer.toc();
		if(sendTimer.secs() >= 1)
		{
			sendTimer.tic();
			gcsThread->sendFrame();

			for(int i = 0; i < POS_REFS; i++)
				XqTrajDesired[i] += 0.001;
			for(int i = 0; i < STATES; i++)
				XqRefDesired[i] += 0.001;
			for(int i = 0; i < MOTORS; i++)
				newDMCs[i]++;
		}

		usleep(1000);
	}

	gcsThread->startShutdown();
	gcsThread->killThread();

	return 0;
}
