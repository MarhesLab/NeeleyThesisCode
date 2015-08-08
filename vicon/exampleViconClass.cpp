#include <iostream>
#include <iomanip>

#include "ViconClass.h"

using namespace std;

char VICON_IP[] = "192.168.1.120:801";

int main()
{
	// Objects for interacting with Vicon
	string name = "quadrotor_1";
	bool data_valid = false; // indicates whether Vicon saw the object or not
	double pos[3];	// position, in meters
	double xyz[3];  // orientation representation using Euler XYZ angles (handled by Vicon library) in radians
	double zyx[3];	// orientation representation using Euler ZYX angles (calculated by the class) in radians
	double quat[4];	// orientation representation using quaternions
	unsigned int hours, mins, secs; // timestamp info for when data was received
	double latency; // latency of the measurement, in seconds

	// Makes the Vicon class object
	ViconClass vicon(VICON_IP);

	// Starts the loop of obtaining and displaying measurements
	while(true)
	{
		vicon.update();	// waits until it gets a new set of data from the Vicon server;
				// this process blocks, so the program cannot progress until it
				// gets the data. As such, if you're doing a big program, it would be
				// wise to create a thread dedicated to obtaining Vicon information.


		// Checks if the measurement information is valid
		data_valid = vicon.subject_info_valid(name);

		// Gets the position of the object in meters
		vicon.get_subject_coordinates(name, pos[0], pos[1], pos[2]);
		
		// Gets the orientation representation using Euler XYZ angles
		vicon.get_subject_euler_xyz(name, xyz[0], xyz[1], xyz[2]);

		// Gets the orientation representation using Euler ZYX angles
		vicon.get_subject_euler_zyx(name, zyx[0], zyx[1], zyx[2]);

		// Gets the orientation representation in quaternion form
		vicon.get_subject_quaternion(name, quat[0], quat[1], quat[2], quat[3]);

		// Gets the timestamp information present in the frame
		vicon.get_timestamp(hours, mins, secs);

		// Gets the latency of the measurement
		latency = vicon.get_total_latency();



		// Prints the values to the screen
		cout << "\n\nGot measurements!" << endl;

		if(data_valid)
			cout << "VIS  | YES" << endl;
		else
			cout << "VIS  | NO" << endl;

		cout<< "TIME | " << hours << "h " << mins << "m " << secs << "s" << endl;

		cout<< "LAT  | ";
		if(latency == 0)
			cout << "?????" << endl;
		else
			cout << setprecision(3) << latency << endl;

		cout << setprecision(3)
			<< "POS  |  X: " <<  pos[0] << "\t Y: " <<  pos[1] << "\t Z: " <<  pos[2] << endl
			<< "XYZ  | Ox: " <<  xyz[0] << "\tOy: " <<  xyz[1] << "\tOz: " <<  xyz[2] << endl 
			<< "ZYX  | Ox: " <<  zyx[0] << "\tOy: " <<  zyx[1] << "\tOz: " <<  zyx[2] << endl
			<< "QUAT | q0: " << quat[0] << "\tqi: " << quat[1] << "\tqj: " << quat[2] << "\tqk: " << quat[3] << endl;
	}	
	
	return 0;
}
