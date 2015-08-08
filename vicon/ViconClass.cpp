/*
	Vicon Measurement Class
	by William Neeley
	September 2014
	
	for Vicon DataStream SDK v1.1.0
	heavily based on the "vicon_pos" class by "nlacock", made on Jan 20, 2011
	
	The motivation for creating this Vicon class is to improve understanding of the 
	Vicon SDK's functionality while improving upon the original. "nlacock"'s class proved
	to be instrumental to starting to understand how different aspects of the SDK
	worked, and I would not have been able to get this far without it.
	
	Note from "nlacock":
	DON'T directly output a string from a Vicon result, the << operator (for strings) that they wrote is broken.
	Note from Wil:
	it appears to work fine if you store the string first, then output it using the std::cout << operator.
*/

#include "ViconClass.h"

using namespace ViconDataStreamSDK::CPP;

// Client-polling based constructor
// Use the IP address of the computer hosting the Vicon system. The default port number is 801.
// Example input: "127.0.0.1:801"
ViconClass::ViconClass(const char *ViconIP)
{
	c = new Client;
	std::cout << "Connecting to Vicon at " << ViconIP << "...\n\r";
	while(!c->IsConnected().Connected) {
		c->Connect(ViconIP);
	    usleep(SLEEPTIME);
	}
	version = c->GetVersion();
	std::cout << "Connected to Vicon system (version " 
		<< version.Major << "." << version.Minor << "." << version.Point << ")\n\r";
	
	c->EnableSegmentData();
	c->EnableMarkerData();
	c->EnableUnlabeledMarkerData();
	c->SetAxisMapping(ViconDataStreamSDK::CPP::Direction::Forward,
			ViconDataStreamSDK::CPP::Direction::Left,
			ViconDataStreamSDK::CPP::Direction::Up );
			
	//c->SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPull); // default; safest for server buffer
	c->SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch); // a bit faster; adds a dedicated
		// thread for constantly fetching and holding onto frames. Can increase network traffic.
	//c->SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ServerPush); // fastest; the server pushes out a new frame
		// whenever it's ready. Can jam Client, then TCP/IP, then Server buffers if delay is a problem.
	
	c->GetFrame();		
	num_subjects = c->GetSubjectCount().SubjectCount;
	std::cout << "Found " << num_subjects << " subjects.\n\r";
	subjects = new std::string[num_subjects];
	valid = new bool[num_subjects];
	positions = new double*[num_subjects];
	euler_xyz = new double *[num_subjects];
	quats = new double *[num_subjects];
	
	std::string temp;
	for(unsigned int i = 0; i < num_subjects; i++) {
		temp = c->GetSubjectName(i).SubjectName;
		std::cout << "Adding Subject: " << temp << "\n\r";
		subjects[i] = temp;
		positions[i] = new double[3];
		euler_xyz[i] = new double[3];
		quats[i] = new double[4];
	}
	std::cout << "Wrapping up Vicon constructor...\n\r";
}


// Multicast-based constructor
// NOT NECESSARILY WORKING AT THE MOMENT; DON'T DEPEND ON IT
// Valid Multicast IP range: 224.0.0.0 to 239.255.255.255
// create a multicast host first before attempting to connect to the multicast. 
ViconClass::ViconClass(const char *ViconIP, const char *MulticastIP, bool multicastHost)
{
	isMulticastHost = multicastHost;
	
	c = new Client;
	
	if(multicastHost) 
	{
		std::cout << "Connecting to Vicon at " << ViconIP << "...\n\r";
		while(!c->IsConnected().Connected){
			c->Connect(ViconIP);
			usleep(SLEEPTIME);
		}
		version = c->GetVersion();
		std::cout << "Connected to Vicon system (version " 
			<< version.Major << "." << version.Minor << "." << version.Point << ")\n\r";
		
		std::cout << "Starting Multicast at " << MulticastIP << "...\n\r";
		c->StartTransmittingMulticast(ViconIP, MulticastIP);
		
		c->SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPull); // leave as ClientPull for efficiency
	}
	else
	{
		std::cout << "Connecting to Vicon at " << ViconIP << " via multicast...\n\r"
			<< "(Multicast address: " << MulticastIP << ")\n\r";
		while(!c->IsConnected().Connected){
			c->ConnectToMulticast(ViconIP,MulticastIP);
			usleep(SLEEPTIME);
		}
		version = c->GetVersion();
		std::cout << "Connected to Vicon system (version " 
			<< version.Major << "." << version.Minor << "." << version.Point << ")\n\r";
			
		c->SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch); // see other options in previous constructor
	}

	c->EnableSegmentData();
	c->EnableMarkerData();
	c->EnableUnlabeledMarkerData();
	c->SetAxisMapping(ViconDataStreamSDK::CPP::Direction::Forward,
			ViconDataStreamSDK::CPP::Direction::Left,
			ViconDataStreamSDK::CPP::Direction::Up );
	c->GetFrame();		
			
	num_subjects = c->GetSubjectCount().SubjectCount;
	std::cout << "Found " << num_subjects << " subjects.\n\r";
	subjects = new std::string[num_subjects];
	valid = new bool[num_subjects];
	positions = new double*[num_subjects];
	euler_xyz = new double *[num_subjects];
	quats = new double *[num_subjects];
	
	std::string temp;
	for(unsigned int i = 0; i < num_subjects; i++) {
		temp = c->GetSubjectName(i).SubjectName;
		std::cout << "Adding Subject: " << temp << "\n\r";
		subjects[i] = temp;
		positions[i] = new double[3];
		euler_xyz[i] = new double[3];
		quats[i] = new double[4];
	}
	std::cout << "Wrapping up Vicon multicast constructor...\n\r";
}


// Destructor
ViconClass::~ViconClass() 
{
	if(isMulticastHost)
		c->StopTransmittingMulticast();
	c->Disconnect();
		
	delete c;
	delete [] subjects;
	delete [] positions;
	delete [] euler_xyz;
	delete [] quats; 
	
	//delete [] segment;
	//delete [] pos_name;
}


// Finds the index of a subject by its given name
int ViconClass::find_subject(std::string name)
{
	for(unsigned int i = 0; i < num_subjects; i++) {
		if(subjects[i] == name)
			return i;
	}
	
	std::cout << "Subject " << name << " not found\n\r";
	exit(1);
	return -1;
}


// Update function to get a new frame from Vicon; call this whenever you want a new measurement.
void ViconClass::update()
{
	while(c->GetFrame().Result != Result::Success)
	{
		usleep(SLEEPTIME);
	}
	
	for(unsigned int i = 0; i < num_subjects; i++)
	{
		temp_segment = c->GetSegmentGlobalTranslation(       subjects[i],subjects[i]); // replace the second subjects[] call with segments for segment info
		temp_euler_xyz     = c->GetSegmentGlobalRotationEulerXYZ(  subjects[i],subjects[i]);
		temp_quat    = c->GetSegmentGlobalRotationQuaternion(subjects[i],subjects[i]);

		if(temp_segment.Occluded)
			valid[i] = false;
		else
			valid[i] = true;

    	positions[i][0] = temp_segment.Translation[0]/1000.0; // converts the measurements from millimeters to meters
	    positions[i][1] = temp_segment.Translation[1]/1000.0;
	    positions[i][2] = temp_segment.Translation[2]/1000.0;

		euler_xyz[i][0] = temp_euler_xyz.Rotation[0];
		euler_xyz[i][1] = temp_euler_xyz.Rotation[1];
		euler_xyz[i][2] = temp_euler_xyz.Rotation[2];
		
		// For the record: Vicon is kind of a jerk. All the literature I've seen has put the quaternion as
		// [ang iv jv kv]; but noooooooo, they had to be different and put it as [iv jv kv ang]. 
		// This stores the values as [ang iv jv kv], or [q0 q1 q2 q3].
		quats[i][0] = temp_quat.Rotation[3]; 
		quats[i][1] = temp_quat.Rotation[0]; 
		quats[i][2] = temp_quat.Rotation[1]; 
		quats[i][3] = temp_quat.Rotation[2]; 
	}
}


// Gets an object's position coordinates (in meters)
void ViconClass::get_subject_coordinates(int index,double& x,double& y,double& z)
{
	x = positions[index][0];
	y = positions[index][1];
	z = positions[index][2];
}
void ViconClass::get_subject_coordinates(std::string name,double& x ,double& y,double& z)
{
	get_subject_coordinates(find_subject(name),x,y,z);
}


// Gets an object's attitude using Euler rotation angles (XYZ, radians)
void ViconClass::get_subject_euler_xyz(unsigned int index,double& theta_x,double& theta_y,double& theta_z)
{
	theta_x = euler_xyz[index][0];
	theta_y = euler_xyz[index][1];
	theta_z = euler_xyz[index][2];
}
void ViconClass::get_subject_euler_xyz(std::string name,double& theta_x,double& theta_y,double& theta_z)
{
	get_subject_euler_xyz(find_subject(name),theta_x,theta_y,theta_z);
}


// Gets an object's attitude using Euler rotation angles (ZYX, radians)
// NOTE: This function starts giving peculiar results when the object is rotated about the Y axis such that 
// (theta_y < -pi/2) || (theta_y > pi/2). Be careful with this!
void ViconClass::get_subject_euler_zyx(unsigned int index, double& theta_x, double& theta_y, double& theta_z)
{
	double q0, q1, q2, q3, temp;
	q0 = quats[index][0];
	q1 = quats[index][1];
	q2 = quats[index][2];
	q3 = quats[index][3];
	temp = 2*(q1*q3 - q0*q2);
	
	theta_x = atan2(2*(q2*q3 + q0*q1), 1 - 2*(q1*q1 + q2*q2));
	theta_y = atan2(-temp, sqrt(1 - temp*temp));
	theta_z = atan2(2*(q1*q2 + q0*q3), 1 - 2*(q2*q2 + q3*q3));
}
void ViconClass::get_subject_euler_zyx(std::string name,double& theta_x,double& theta_y,double& theta_z)
{
	get_subject_euler_zyx(find_subject(name),theta_x,theta_y,theta_z);
}

// Gets an object's quaternion 
void ViconClass::get_subject_quaternion(unsigned int index,double& ang,double& iv,double& jv,double& kv)
{
	ang = quats[index][0]; // q0
	iv  = quats[index][1]; // q1
	jv  = quats[index][2]; // q2
	kv  = quats[index][3]; // q3

	// ensures q0 is always greater than 0;
	// a quaternion (q) and its negative (-q) are the same.
	if(quats[index][0] < 0) {
		ang = -ang;
		iv = -iv;
		jv = -jv;
		kv = -kv;
	}
}
void ViconClass::get_subject_quaternion(std::string name,double& ang,double& iv,double& jv,double& kv)
{
	get_subject_quaternion(find_subject(name),ang,iv,jv,kv);
}


// Gets an object's position and attitude and puts it into a 6-dimensional vector.
void ViconClass::get_subject_position_and_attitude(std::string name, double states[])
{
	int index = find_subject(name);
	get_subject_coordinates(index, states[0], states[1], states[2]);
	get_subject_euler_zyx(  index, states[3], states[4], states[5]);
}


// Gets an object's position and quaternion-based attitude, then puts it into a 7-dimensional vector.
void ViconClass::get_subject_position_and_quaternion(std::string name, double states[])
{
	int index = find_subject(name);
	get_subject_coordinates(index, states[0], states[1], states[2]);
	get_subject_quaternion( index, states[3], states[4], states[5], states[6]);
}


// Returns whether or not an object's measurement data is valid (nonzero)
bool ViconClass::subject_info_valid(int index)
{
	return valid[index];
}
bool ViconClass::subject_info_valid(std::string name)
{
	return subject_info_valid( find_subject(name) );
}


// Gets the timestamp values of the last frame received
// DOESN'T APPEAR TO WORK WITH VERSION 1.1.0
void ViconClass::get_timestamp(unsigned int& hours, unsigned int& mins, unsigned int& secs)
{
	temp_timecode = c->GetTimecode();

	hours = temp_timecode.Hours;
	mins = temp_timecode.Minutes;
	secs = temp_timecode.Seconds;
	
	// troubleshooting/testing information
	/*std::cout << "Timestamp information: " 
		<< temp_timecode.Hours    << " hours, "
		<< temp_timecode.Minutes  << " minutes, "
		<< temp_timecode.Seconds  << " seconds; \n\rTime Standard: "
		<< temp_timecode.Standard << "\n\rFrame #" 
		<< temp_timecode.Frames   << ", subframe #"
		<< temp_timecode.SubFrame << ", subframes per frame: "
		<< temp_timecode.SubFramesPerFrame << "\n\r";*/
}


// Gets the path latency of the frame in seconds
double ViconClass::get_total_latency()
{
	temp_totallatency = c->GetLatencyTotal();
	return temp_totallatency.Total;
}
