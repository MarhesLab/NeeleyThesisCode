#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "QuadClientThread.h"
#include "../defs.h"
#include "../functions/Timer.h"

using namespace std;

// Class Pointer Declaration (by starting it as a pointer,
// it makes it easy to call the constructor once the variables
// have been declared)
QuadClientThread* quadThread;

// Received Values
double XqTrajDesired[POS_REFS];
double XqRefDesired[STATES];
unsigned char newDMCs[MOTORS];
FlightMode desiredMode;

// Transmitted Values
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
	for(int i = 0; i < STATES; i++)
		Xq[i] = i;
	for(int i = 0; i < STATES; i++)
		XqR[i] = i;
	for(int i = 0; i < VIC_STATES; i++) 
		XqVicon[i] = i;
	for(int i = 0; i < IMU_STATES; i++)
		XqImu[i] = i;
	for(int i = 0; i < INPUTS; i++)
		Uq[i] = i;
	for(int i = 0; i < MOTORS; i++)
		motorDMCs[i] = i + 1;

	cout << "Starting IMU Values: \n\r";
	for(int i = 0; i < IMU_STATES; i++) {
	cout << XqImu[i] << "\t";
		if((i == 2) || (i == 5) || (i == 9))
			cout << "\n\r";
	}
	cout << "\n\r";


	ap_status = 0x0000;
	batt_voltage = 12.4;
	cpu_load = 4.2;

	uart_status = CONN_CONNECTING;
	vicon_status = CONN_CONNECTING;
	currentMode = QUAD_IDLE;

	// Create the class object
	quadThread = new QuadClientThread(Xq, XqR, Uq, XqVicon, XqImu,
		motorDMCs, &ap_status, &batt_voltage, &cpu_load, 
		&uart_status, &vicon_status, &currentMode);

	// Initialize, Listen for a client, and Start the thread
	quadThread->initialize();
	quadThread->startThread();

	sendTimer.tic();
	while(true)
	{
	    if(quadThread->gotFrame())
	    {
	        // Update values
			quadThread->getTrajectory(XqTrajDesired);
	        quadThread->getReference(XqRefDesired);
	        quadThread->getSpeeds(newDMCs);
	        desiredMode = quadThread->getMode();

			/*cout << "\nMotor Speeds: "; 
			for(int i = 0; i < MOTORS; i++)
				cout << "\t" << (unsigned int)newDMCs[i];

			cout << "\n\rDesired Mode: " 
				 << modeString(desiredMode) << "\n\n\r";*/
	    }

		sendTimer.toc();
		if(sendTimer.secs() >= 1)
		{
			sendTimer.tic();
			quadThread->sendFrame();


			cout << "Sent IMU Values: \n\r";
			for(int i = 0; i < IMU_STATES; i++) {
				cout << XqImu[i] << "\t";
				if((i == 2) || (i == 5) || (i == 9))
					cout << "\n\r";
			}
			cout << "\n\r";


			for(int i = 0; i < STATES; i++)
				Xq[i] -= 0.001;

			for(int i = 0; i < STATES; i++)
				XqR[i] += 0.001;

			for(int i = 0; i < VIC_STATES; i++) 
				XqVicon[i] += 0.001;

			for(int i = 0; i < IMU_STATES; i++)
				XqImu[i] -= 0.001;

			for(int i = 0; i < INPUTS; i++)
				Uq[i] += 0.001;

			for(int i = 0; i < MOTORS; i++)
				motorDMCs[i] = motorDMCs[i] + 1;

			ap_status = ap_status ^ 0xFFFF;
			batt_voltage -= 0.01;
			cpu_load += 0.1;
		}

		usleep(1000);
	}

	quadThread->startShutdown();
	quadThread->killThread();

	return 0;
}
