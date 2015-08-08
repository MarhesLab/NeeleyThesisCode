#ifndef PACKET_DEFS_H
#define PACKET_DEFS_H

#include <cstdint>	 // used for set size definitions
#include "../defs.h" // used for flightMode enumerations and array sizing

#define STAT_DISCONNECTED 0
#define STAT_CONNECTING 1
#define STAT_CONNECTED 2

struct quad2gcs {
	int32_t states[STATES]; 	// holds state values * 10^6
	int32_t refs[STATES];		// holds reference state values * 10^6
	int32_t inputs[INPUTS];  	// holds input values * 10^6
	int32_t vicon[VIC_STATES];  // holds Vicon measurements * 10^6
	int32_t imu[IMU_STATES];	// holds IMU measurements
	uint8_t motors[MOTORS];  	// holds motor speed characters
	uint16_t ap_status;		// holds binary representation of the Autopilot's status
	uint32_t batt_voltage; 	// holds battery voltage * 1000
	uint32_t cpu_load;		// holds CPU load percentage * 1000
	uint8_t uart_status;	// holds representation of serial connection status
	uint8_t vicon_status;	// holds representation of vicon connection status
	uint8_t flightmode;		// holds representation of flight mode	
};

struct gcs2quad {
	int32_t state_traj[POS_REFS];
	int32_t state_ref[STATES];
	uint8_t motors[MOTORS];
	uint8_t flightmode;
};


void packQuad2GcsFrame(quad2gcs &frame, double states[], double refs[], double inputs[], 
	double vicon[], double imu[], unsigned char motorDMCs[], 
	short auto_status, double voltage, double cpu_load, 
	ConnStatus serial_status, ConnStatus vicon_status, enum FlightMode currentFlightMode);

void unpackQuad2GcsFrame(quad2gcs &frame, double states[], double refs[], double inputs[], 
	double vicon[], double imu[], unsigned char motorDMCs[], 
	short &auto_status, double &voltage, double &cpu_load, 
	ConnStatus &serial_status, ConnStatus &vicon_status, enum FlightMode &currentFlightMode);

void packGcs2QuadFrame(gcs2quad &frame, double trajectory[], 
	double reference[], unsigned char motorDMCs[],
	enum FlightMode &currentFlightMode);

void unpackGcs2QuadFrame(gcs2quad &frame, double trajectory[],
	double reference[], unsigned char motorDMCs[],
	enum FlightMode &currentFlightMode);


#endif
