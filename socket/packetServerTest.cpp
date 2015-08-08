#include <iostream>
#include <iomanip>

#include "packet.h"
#include "QuadServer.h"
#include "../defs.h"

using namespace std;

int main()
{
	struct quad2gcs inPacket, *inPacketPtr = &inPacket;
	struct gcs2quad outPacket;//, *outPacketPtr = &outPacket;
	
	// Information to receive
	double Xq[STATES] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	double XqRef[STATES] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	double uq[INPUTS] = {0, 0, 0, 0};
	double XqVic[VIC_STATES] = {0, 0, 0, 1, 0, 0, 0};
	double XqImu[IMU_STATES] = {0, 0, 0, 0, 0, 0};
	short apStatus = 0x0000;
	unsigned char motorChars[MOTORS] = {0, 0, 0, 0};
	double voltage = 0;
	double cpuLoad = 0;

	enum ConnStatus uartStatus = CONN_DISCONNECTED;
	enum ConnStatus viconStatus = CONN_DISCONNECTED;
	enum FlightMode currentMode = QUAD_OFF;

	// Information to send
	int chars_received = sizeof(inPacket);
	double XqTraj[POS_REFS];
	double XqRefOut[STATES];
	unsigned char motorCharsNew[MOTORS] = {100, 75, 100, 75};
	enum FlightMode newMode = QUAD_CYCLE;

	// Set up the Server object
	QuadServer server;

	server.init();
	server.listenNow();
	server.acceptNow();

	if(server.receiveData((char *)inPacketPtr, chars_received))
		cout << "Received " << chars_received << " bytes from client" << endl;
	else
		cout << "Nothing/Partial Frame received from the client." << endl;

	// Unpacking the info from the quadrotor
	unpackQuad2GcsFrame(inPacket, Xq, XqRef, uq, XqVic, XqImu, 
		motorChars, apStatus, voltage, cpuLoad, 
		uartStatus, viconStatus, currentMode);

	cout << "The received data: " << setprecision(5) << endl << endl
		<< " Xq: " << endl;
		for(int i = 0; i < STATES; i++) {
			cout << Xq[i] << "\t";
			if((i == QK) || (i == WZ))
				cout << endl;
		}

	cout << "\n Uq: " << endl;
		for(int i = 0; i < INPUTS; i++)
			cout << uq[i] << "\t";
	cout << "\n\n\r XqVic: " << endl;
		for(int i = 0; i < VIC_STATES; i++)
			cout << XqVic[i] << "\t";
	cout << "\n\n\r XqImu: " << endl;
		for(int i = 0; i < IMU_STATES; i++)
			cout << XqImu[i] << "\t";
	cout << "\n\n\r Motor Speeds: " << endl;
		for(int i = 0; i < MOTORS; i++)
			cout << (unsigned int)motorChars[i] << "\t";
	cout << "\n\n\r Voltage level: " << voltage << endl
		<< "CPU Load: " << cpuLoad << endl;
	cout << "Current Flight Mode: ";

	cout << modeString(currentMode) << endl;
	/*
	switch(currentMode)
	{
		case QUAD_OFF:
			cout << "Off" << endl;
			break;
		case QUAD_IDLE:
			cout << "Idle" << endl;
			break;
		case QUAD_RAMPUP:
			cout << "Ramping up" << endl;
			break;
		case QUAD_TAKEOFF:
			cout << "Taking off" << endl;
			break;
		case QUAD_HOVER:
			cout << "Hovering" << endl;
			break;
		case QUAD_FOLLOW_REF:
			cout << "Following Reference" << endl;
			break;
		case QUAD_UNSAFE:
			cout << "!!! UNSAFE !!!" << endl;
			break;
		case QUAD_EXITING:
			cout << "Exiting" << endl;
			break;
		case QUAD_CYCLE:
			cout << "Cycling Motor Speeds" << endl;
			break;
		case QUAD_LATENCY_TEST:
			cout << "Performing Latency Test" << endl;
			break;
		case QUAD_DMC:
			cout << "Direct Motor Control" << endl;
			break;
		default:
			cout << "STUFF IS BROKEN, YO" << endl;
	}*/

	// Adjusting values and packing the frame
	for(int i = 0; i < STATES; i++)
		XqRefOut[i] = Xq[i] + 0.5;

	packGcs2QuadFrame(outPacket, XqTraj, XqRefOut, motorCharsNew, newMode);
	//server.sendData((unsigned char *)outPacket, sizeof(outPacket));	
	
	return 0;
}
