#include <iostream>
#include <bitset> // for outputting binary

#include "AutopilotThread.h"
#include "../functions/autopilot.h"

using namespace std;

int main()
{
	double imu_values[IMU_STATES];
	short ap_status;
	double voltage;
	double cpu_load;
	double motorRPMs[MOTORS];
	
	// Thread object creation
	AutopilotThread apthread;
	
	// Thread object initialization and start
	apthread.initialize();
	apthread.startThreads();
	
	while(true)
	{
		// checks if a new set of measurements has been received
		if(apthread.newMeasurement())
		{
			// if so, gets the new measurement values and returns them through the function call
			apthread.getMeasurement(imu_values);
			
			cout << "Received IMU values: \n\r";
			
			for(int i = 0; i < IMU_STATES; i++)
				cout << imu_values[i] << "\n\r";
				
			cout << "\n\r";
		}
		// checks if a new set of quadrotor status values has been received; if so, updates them automatically
		else if(apthread.newStatus())
		{
			// directly grabs the new values without having to load them first
			ap_status = apthread.getAPStatus();
			voltage = apthread.getBatteryVoltage();
			cpu_load = apthread.getCPULoadPercentage();
			apthread.getMotorRPMs(motorRPMs);
			
			cout << "AP Status: " << (bitset<16>)ap_status << "\n\r"; 
			cout << "Battery voltage: " << voltage << "\n\r"
				<< "CPU Load: " << cpu_load << " (%) \n\r" 
				<< "Motor RPMs: " << motorRPMs[0] << "\t" << motorRPMs[1] << "\t" 
								  << motorRPMs[2] << "\t" << motorRPMs[3] << "\n\r\n\r";
		}
	}
	
	// call this as soon as the shutdown process starts to give the thread ample time to shut down on its own
	apthread.startShutdown();
	
	return 0;
}
