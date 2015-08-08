/*
	For more information on multithreading, see the PDF titled
	"POSIX Threading Programming Resource" in this folder.
*/

#include <iostream>

#include "QuadClientThread.h"
#include "../socket/packet.h"
#include "../defs.h"

using namespace std;

// Mutex Declaration
pthread_mutex_t quad_client_mutex;

// Constructor
QuadClientThread::QuadClientThread(double *Xqp, double *XqRp, double *Uqp, double *XqVp, double *XqIp,
		unsigned char *DMCp, short *apsp, double *vp, double *cp, 
		ConnStatus *usp, ConnStatus *vsp, FlightMode *mp)
{
	// Initializes the appropriate Boolean values
	connected = false;
	updated = false;
	running = true;
	
	// Initializes some of the arrays for data coming from the GCS
	for(int i = 0; i < POS_REFS; i++)
		XqTraj[i] = 0;
	for(int i = 0; i < STATES; i++)
		XqRef[i] = 0;
	for(int i = 0; i <  MOTORS; i++)
		newDMCs[i] = 0;

	// Initializes the quadrotor to be off
	newMode = QUAD_OFF;

	// Sets the pointers to point at the right spots for gathering data
	frameToSendPtr = &frameToSend;
	Xq = Xqp;
	XqR = XqRp;
	Uq = Uqp;
	XqV = XqVp;
	XqI = XqIp;
	DMCs = DMCp;

	apStatus = apsp;
	volts = vp;
	cpu = cp;

	uartStatus = usp;
	viconStatus = vsp;
	mode = mp;
}


// Destructor
QuadClientThread::~QuadClientThread()
{
	// flags the thread for shutdown (if it wasn't beforehand)
	running = false;

	// forcibly shuts down the client thread in case it blocks while trying to read
	pthread_cancel(the_thread);
}


// Initializes the Client and Populates the Struct
void QuadClientThread::initialize()
{
	// Initializes the Client object
	client.init(GCS_IP);
	client.connectNow();
	connected = true;

	// Populates the struct for receiving data
	//qstruct.struct_mutex = &the_mutex;
	qstruct.struct_client    = &client;
	qstruct.struct_XqTraj    = XqTrajTemp;
	qstruct.struct_XqRef     = XqRefTemp;
	qstruct.struct_DMCs      = newDMCsTemp;
	qstruct.struct_mode      = &newModeTemp;
	qstruct.struct_connected = &connected;
	qstruct.struct_updated   = &updated;
	qstruct.struct_running   = &running;
}


// Actually starts the threaded process
bool QuadClientThread::startThread()
{
	running = true;

	if(pthread_create(&the_thread, NULL, quadClientThreadFunction, (void *)&qstruct))
	{
		std::cout << "Could not create Quadrotor Client thread. \n\r";
		return false;
	}

	return true;
}


// Grabs the data from the arrays/values pointed to by the pointers, then sends the data off in a frame to the GCS
void QuadClientThread::sendFrame()
{
	packQuad2GcsFrame(frameToSend, Xq, XqR, Uq, 
		XqV, XqI, DMCs, 
		*apStatus, *volts, *cpu, 
		*uartStatus, *viconStatus, *mode);

	if(connected)
		client.sendData((char *)frameToSendPtr, sizeof(frameToSend));
	else
		cout << "Client: couldn't send frame. Connection lost.\n\r";
}


// Checks to see if a new frame has arrived from the GCS
bool QuadClientThread::gotFrame()
{
	bool result = updated;
	
	if(updated)
	{
		updated = false;

		pthread_mutex_lock(&quad_client_mutex);
		for(int i = 0; i < POS_REFS; i++)
			XqTraj[i] = XqTrajTemp[i];
		for(int i = 0; i < STATES; i++)
			XqRef[i] = XqRefTemp[i];
		for(int i = 0; i <  MOTORS; i++)
			newDMCs[i] = newDMCsTemp[i];
		newMode = newModeTemp;
		pthread_mutex_unlock(&quad_client_mutex);
	}

	return result;
}


// Returns the desired position/heading trajectory
void QuadClientThread::getTrajectory(double Traj[])
{
	for(int i = 0; i < POS_REFS; i++)
		Traj[i] = XqTraj[i];
}


// Returns the reference states
void QuadClientThread::getReference(double Ref[])
{
	for(int i = 0; i < STATES; i++)
		Ref[i] = XqRef[i];
}


// Returns the motor speeds
void QuadClientThread::getSpeeds(unsigned char DMCs[])
{
	for(int i = 0; i < MOTORS; i++)
		DMCs[i] = newDMCs[i];
}


// Returns the new, commanded flight mode
FlightMode QuadClientThread::getMode()
{
	return newMode;
}


// Starts the thread shutdown process
void QuadClientThread::startShutdown()
{
	running = false;
}

void QuadClientThread::killThread()
{
	pthread_cancel(the_thread);
}

// The actual function that gets sent to its own thread. This function starts a loop that
// continually scans for GCS frames, then processes them appropriately.
void* quadClientThreadFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct QuadClientStruct *qstruct;
	qstruct = (struct QuadClientStruct *) threadarg;

	// declaring appropriate variables
	QuadClient* client  = qstruct->struct_client;
	double* XqTraj      = qstruct->struct_XqTraj;
	double* XqRef       = qstruct->struct_XqRef;
	unsigned char* DMCs = qstruct->struct_DMCs;
	FlightMode* mode    = qstruct->struct_mode;
	bool* connected		= qstruct->struct_connected;
	bool* updated       = qstruct->struct_updated;
	bool* running       = qstruct->struct_running;

	// Additional values
	gcs2quad frameReceived, *frameReceivedPtr = &frameReceived;

	// Keep getting values until flagged to stop
	while(*running)
	{
		do
		{
			// grabs data; blocks if connected, returns "false" if the connection is lost
			// or if there was an error. Otherwise, only returns once it gets the
			// desired number of bytes.
			if(!client->demandData((char *)frameReceivedPtr, sizeof(frameReceived)))
			{
				*connected = false;
				cout << "Lost connection to server. Looking again..." << endl;
				client->connectNow();
				*connected = true;
			}
		}while(!*connected);

		pthread_mutex_lock(&quad_client_mutex);
		unpackGcs2QuadFrame(frameReceived, XqTraj, XqRef, DMCs, *mode);
		pthread_mutex_unlock(&quad_client_mutex);
		
		*updated = true;
	}

	// Kill the thread
	pthread_exit(NULL);
}
