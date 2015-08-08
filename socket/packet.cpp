#include <netinet/in.h>

#include "packet.h"

#define SCALE 1000000.0
#define VSCALE 1000.0

// Packs the data into the given frame 
void packQuad2GcsFrame(quad2gcs &frame, double states[], double refs[], double inputs[], 
	double vicon[], double imu[], unsigned char motorDMCs[], 
	short auto_status, double voltage, double cpu_load, 
	ConnStatus serial_status, ConnStatus vicon_status, enum FlightMode currentFlightMode)
{
	// htonl() converts the data from Host byte format to Network 
	// byte format (look at the guide for more info)	
	for(int i = 0; i < STATES; i++)
		frame.states[i] = htonl((int)(states[i]*SCALE));

	for(int i = 0; i < STATES; i++)
		frame.refs[i] = htonl((int)(refs[i]*SCALE));

	for(int i = 0; i < INPUTS; i++)
		frame.inputs[i] = htonl((int)(inputs[i]*SCALE));

	for(int i = 0; i < VIC_STATES; i++)
		frame.vicon[i]  = htonl((int)(vicon[i]*SCALE));

	for(int i = 0; i < IMU_STATES; i++)
		frame.imu[i]    = htonl((int)(imu[i]*SCALE));

	for(int i = 0; i < MOTORS; i++)
		frame.motors[i] = motorDMCs[i];

	frame.ap_status     = htons(auto_status);
	frame.batt_voltage  = htonl((int)(voltage*VSCALE));
	frame.cpu_load      = htonl((int)(cpu_load*VSCALE));

	frame.uart_status   = (uint8_t)serial_status;
	frame.vicon_status  = (uint8_t)vicon_status;
	frame.flightmode    = currentFlightMode;
}


void unpackQuad2GcsFrame(quad2gcs &frame, double states[], double refs[], double inputs[], 
	double vicon[], double imu[], unsigned char motorDMCs[], 
	short &auto_status, double &voltage, double &cpu_load, 
	ConnStatus &serial_status, ConnStatus &vicon_status, enum FlightMode &currentFlightMode)
{
	// ntohl() converts the data from Network byte format to Host
	// byte format (look at the guide for more info)	
	for(int i = 0; i < STATES; i++)
		states[i] = (1.0*(int)ntohl(frame.states[i]))/SCALE;

	for(int i = 0; i < STATES; i++)
		refs[i] = (1.0*(int)ntohl(frame.refs[i]))/SCALE;

	for(int i = 0; i < INPUTS; i++)
		inputs[i] = (1.0*(int)ntohl(frame.inputs[i]))/SCALE;

	for(int i = 0; i < VIC_STATES; i++)
		vicon[i]  = (1.0*(int)ntohl(frame.vicon[i]))/SCALE;

	for(int i = 0; i < IMU_STATES; i++)
		imu[i]    = (1.0*(int)ntohl(frame.imu[i]))/SCALE;

	for(int i = 0; i < MOTORS; i++)
		motorDMCs[i] = frame.motors[i];

	auto_status		= ntohs(frame.ap_status);
	voltage       	= (1.0*(int)ntohl(frame.batt_voltage))/VSCALE;
	cpu_load      	= (1.0*(int)ntohl(frame.cpu_load))/VSCALE;

	serial_status 	= (ConnStatus)frame.uart_status;
	vicon_status	= (ConnStatus)frame.vicon_status;
	currentFlightMode = (enum FlightMode)frame.flightmode;
}


void packGcs2QuadFrame(gcs2quad &frame, double trajectory[], 
	double reference[], unsigned char motorDMCs[],
	enum FlightMode &currentFlightMode)
{
	for(int i = 0; i < POS_REFS; i++)
		frame.state_traj[i] = htonl((int)(trajectory[i]*SCALE));

	for(int i = 0; i < STATES; i++)
		frame.state_ref[i] = htonl((int)(reference[i]*SCALE));
	
	for(int i = 0; i < MOTORS; i++)
		frame.motors[i] = motorDMCs[i];
	
	frame.flightmode = currentFlightMode;
}


void unpackGcs2QuadFrame(gcs2quad &frame, double trajectory[],
	double reference[], unsigned char motorDMCs[],
	enum FlightMode &currentFlightMode)
{
	for(int i = 0; i < POS_REFS; i++)
		trajectory[i] = (1.0*(int)ntohl(frame.state_traj[i]))/SCALE;

	for(int i = 0; i < STATES; i++)
		reference[i] = (1.0*(int)ntohl(frame.state_ref[i]))/SCALE;

	for(int i = 0; i < MOTORS; i++)
		motorDMCs[i] = frame.motors[i];
		
	currentFlightMode = (enum FlightMode)frame.flightmode;
}
