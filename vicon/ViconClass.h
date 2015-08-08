/*
	Vicon Measurement Class Header File
	by William Neeley
	September 2014
	
	based on the "vicon_pos" class by "nlacock", made on Jan 20, 2011
	
	See "ViconClass.cpp" for more information.
*/

#ifndef VICON_CLASS_H
#define VICON_CLASS_H

#include "Client.h"
#include <unistd.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iostream>

#define SLEEPTIME 10

class ViconClass {
	// the Client object used to interact with the Vicon system
	ViconDataStreamSDK::CPP::Client* c;
	bool isMulticastHost;
	
	
	// Subject Information
	unsigned int num_subjects;
	std::string * subjects; 	// names of detected subjects
	//std::string *segment, *pos_name	// names of segments of detected subjects; only really useful
			// for multi-segment objects
	
	bool   *  valid;				// indicates if a set of measurements is valid		
	double ** positions;			// positions of detected subject (in METERS)
	double ** euler_xyz;			// Euler angles of detected subjects (Euler XYZ configuration, RADIANS) 
	double ** quats;				// Quaternion information of detected subjects
	
	
	// Temporary, Vicon SDK-specific objects
	ViconDataStreamSDK::CPP::Output_GetVersion version;
	ViconDataStreamSDK::CPP::Output_GetSegmentGlobalTranslation temp_segment;
	ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationEulerXYZ temp_euler_xyz;
	ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationQuaternion temp_quat;
	ViconDataStreamSDK::CPP::Output_GetTimecode temp_timecode;
	ViconDataStreamSDK::CPP::Output_GetLatencyTotal temp_totallatency;
	
	
	
public:
	// Constructors and Destructors
	ViconClass(const char *ViconIP);
	ViconClass(const char *ViconIP, const char *MulticastIP, bool multicastHost); // NOT WORKING RIGHT NOW; kinda started looking into multicasting, but there isn't a big demand for it, so I didn't see it through. -Wil
	~ViconClass();
	
	
	// Update function for getting a new set of measurements
	void update();
	
	
	// Function calls to get measurement information
	int find_subject(std::string subjectname);
	void get_subject_coordinates(int index,double& x,double& y,double& z);
	void get_subject_coordinates(std::string name,double& x ,double& y,double& z);
	void get_subject_euler_xyz(unsigned int index,double& theta_x,double& theta_y,double& theta_z);
	void get_subject_euler_xyz(std::string name,double& theta_x,double& theta_y,double& theta_z);
	void get_subject_euler_zyx(unsigned int index,double& theta_x,double& theta_y,double& theta_z);
	void get_subject_euler_zyx(std::string name,double& theta_x,double& theta_y,double& theta_z);
	void get_subject_quaternion(unsigned int index,double& ang,double& iv,double& jv,double& kv);
	void get_subject_quaternion(std::string name,double& ang,double& iv,double& jv,double& kv);
	
	void get_subject_position_and_attitude(std::string name, double states[]);
	void get_subject_position_and_quaternion(std::string name, double states[]);
	
	bool subject_info_valid(int index);
	bool subject_info_valid(std::string name);	


	// Function calls for timing information
	void get_timestamp(unsigned int& hours, unsigned int& mins, unsigned int& secs);
	double get_total_latency();
	
};
	
#endif
