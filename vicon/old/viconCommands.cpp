/* 
	AscTec Hummingbird Quadrotor Control using Vicon Position Measurement
	Vicon Data Functions

	STUFF

	Functions Present:
	- getPositionData()
*/

#include "viconCommands.h"

using namespace std;

/**************************************************************************************
	Getting Position Measurement Data from Vicon 

	by: Wil Neeley
	June 2014

	Function Dependencies:
	-Class vicon_pos

	This function is used to obtain measurement information from the Vicon motion
	capture system. First, the "vicon->update()" function forces the program to 
	sleep until it receives the latest measurement information from Vicon. Next, 
	the desired subject (given by "quadName") will have its position and attitude
	information placed into a temporary array. As the Vicon functions require floats
	and the main program uses doubles for everything, a temporary array of floats
	(temp) was created for use with the functions. Afterwards, the values are moved
	from the array of floats into the desired array of doubles.
**************************************************************************************/

void waitForViconUpdate(vicon_pos *vicon) 
{
	// waits until it receives the next set of measurements from Vicon
	vicon->update();
}

void getPositionData(vicon_pos *vicon, double XquadVicon[], string quadName) 
{
	//waitForViconUpdate(vicon);
	
	// these functions require floats; creating a temporary float variable
	float temp[6];
			
	// gets the position in millimeters
	vicon->get_subject_coord(quadName,temp[0],temp[1],temp[2]);
	// gets the angles in radians
	vicon->get_subject_euler_xyz(quadName,temp[3],temp[4],temp[5]);


	// puts the "float" values into the desired "double" array
	for(int i = 0; i < 6; i++) {
		XquadVicon[i] = temp[i];
	}

	// converts position to meters
	for(int i = 0; i < 3; i++) {
		XquadVicon[i] = XquadVicon[i]/1000.0;
	}
}

void getTimeStampInformation(vicon_pos *vicon) 
{
	unsigned int time[3];

	int result = vicon->getTimeStampValues(time[0],time[1],time[2]);

	unsigned int hours = time[0], mins = time[1], secs = time[2];



	cout << "Time stamp info: ";
	if(result == 0)
		cout << "SUCCESS! ";
	if(result == -1)
		cout << "NOT CONNECTED ";
	if(result == 1)
		cout << "NO FRAME ";

	cout 	<< hours << " hours, " 
		<< mins  << " minutes, " 
		<< secs  << " seconds\n\r";
}

double getTotalViconLatency(vicon_pos *vicon)
{
	double latency = vicon->getLatencyTotal();
	cout << "Vicon latency: " << latency*1000.0 << " milliseconds\n\r";
	return latency;
}
