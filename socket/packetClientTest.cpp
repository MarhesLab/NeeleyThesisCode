#include <iostream>

#include "packet.h"
#include "QuadClient.h"
//#include "../defs.h"

using namespace std;

int main()
{
	struct quad2gcs outPacket, *outPacketPtr = &outPacket;
	//struct gcs2quad inPacket, *inPacketPtr = &inPacket;
	
	// Information to send:
	double Xq[STATES] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3};
	double XqRef[STATES] = { 0.11, 0.22, 0.33, 0.44, 0.55, 0.66, 0.77, 0.88, 0.99, 1.01, 1.11, 1.21, 1.31};
	double uq[INPUTS] = {-0.1, -0.2, -0.3, -0.4};
	double XqVic[VIC_STATES] = {0.1001, 0.2002, 0.3003, 0.7007, 0.8008, 0.9009, 1.001};
	double XqImu[IMU_STATES] = {0.4004, 0.5005, 0.6006, 1.0010, 1.1011, 1.2012};
	short apStatus = 0x0123;
	unsigned char motorChars[MOTORS] = {150, 125, 150, 125};
	double voltage = 4.321;
	double cpuLoad = 99.99;
	enum ConnStatus uartStatus = CONN_CONNECTING;
	enum ConnStatus viconStatus = CONN_CONNECTING;
	enum FlightMode currentMode = QUAD_IDLE;

	// Information to receive
	/*double XqRef[POS_REFS];
	unsigned char motorCharsNew[MOTORS] = {0, 0, 0, 0};
	enum FlightMode newMode = QUAD_OFF;

	for(int i = 0; i < POS_REFS; i++)
		XqRef[i] = 0;
	*/

	// Pre-packing the frame for the GCS
	packQuad2GcsFrame(outPacket, Xq, XqRef, uq, XqVic, XqImu, 
		motorChars, apStatus, voltage, cpuLoad, 
		uartStatus, viconStatus, currentMode);

	// Set up the Client object
	QuadClient client;

	client.init("127.0.0.1");
	client.connectNow();

	client.sendData((char *)outPacketPtr, sizeof(outPacket));

	return 0;
}
