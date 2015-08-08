/*
	For more information on multithreading, see the PDF titled
	"POSIX Threading Programming Resource" in this folder.
*/

#include <iostream>

#include "GcsServerThread.h"

using namespace std;

// Global Variable Mutex Definition
pthread_mutex_t gcs_server_mutex;

// Constructor
GcsServerThread::GcsServerThread(double* XqTp, double* XqRp, unsigned char* nDMCp, FlightMode* nMp)
{
	// Initializes Boolean values
	connected = false;
	updated = false;
	running = true;	

	// Initializes some of the values
	for(int i  = 0; i < STATES; i++) 
		 Xq[i] = 0;
	for(int i  = 0; i < STATES; i++)
		XqR[i] = 0;
	for(int i  = 0; i < VIC_STATES; i++)
		XqV[i] = 0;
	for(int i  = 0; i < IMU_STATES; i++) 
		XqI[i] = 0;
	for(int i  = 0; i < INPUTS; i++) 
		 Uq[i] = 0;
	for(int i  = 0; i < MOTORS; i++)
		DMCs[i] = 0;
	volts = 0;
	cpu = 0;
	mode = QUAD_OFF;

	// Initializes pointers
	frameToSendPtr = &frameToSend;	
	XqTraj  = XqTp;
	XqRef   = XqRp;
	newDMCs = nDMCp;
	newMode = nMp;
}


// Destructor
GcsServerThread::~GcsServerThread()
{
	// flags the thread for shutdown (if it wasn't beforehand)
	running = false;

	// forcibly shuts down the client thread in case it blocks while trying to read
	pthread_cancel(the_thread);	
}


// Initializes the Server and Populates the Struct
void GcsServerThread::initialize()
{
	// Initializes the Server object
	server.init();

	// Populates the struct for receiving data
	gstruct.struct_server    = &server;
	gstruct.struct_Xq        = XqTemp;
	gstruct.struct_XqR		 = XqRTemp;
	gstruct.struct_Uq        = UqTemp;
	gstruct.struct_XqV       = XqVTemp;
	gstruct.struct_XqI       = XqITemp;
	gstruct.struct_DMCs      = DMCsTemp;
	gstruct.struct_ap_status = &apStatusTemp;
	gstruct.struct_volts     = &voltsTemp;
	gstruct.struct_cpu       = &cpuTemp;
	gstruct.struct_uart_stat = &uartStatusTemp;
	gstruct.struct_vic_stat  = &viconStatusTemp;
	gstruct.struct_mode      = &modeTemp;
	gstruct.struct_connected = &connected;
	gstruct.struct_updated   = &updated;
	gstruct.struct_running   = &running;
}

void GcsServerThread::listenAndAccept()
{
	server.listenNow();
	server.acceptNow();
	connected = true;
}


// Actually starts the threaded process
bool GcsServerThread::startThread()
{
	running = true;

	if(pthread_create(&the_thread, NULL, gcsServerThreadFunction, (void *)&gstruct))
	{
		std::cout << "Could not create GCS Server thread. \n\r";
		return false;
	}

	return true;
}


// Grabs the data from the arrays/values pointed to by the pointers, then sends the data off in a frame to the quadrotor
void GcsServerThread::sendFrame()
{
	packGcs2QuadFrame(frameToSend, XqTraj, XqRef, newDMCs, (FlightMode&)*newMode);

	if(connected)
		server.sendData((char *)frameToSendPtr, sizeof(frameToSend));
	else
		cout << "Server: couldn't send frame. Connection Lost.\n\r";
}


// Checks to see if a new frame has arrived from the GCS
bool GcsServerThread::gotFrame()
{
	bool result = updated;

	if(updated)
	{
		updated = false;

		pthread_mutex_lock(&gcs_server_mutex);
		for(int i = 0; i < STATES; i++)
			Xq[i] = XqTemp[i];
		for(int i = 0; i < STATES; i++)
			XqR[i] = XqRTemp[i];
		for(int i = 0; i < VIC_STATES; i++) 
			XqV[i] = XqVTemp[i];
		for(int i = 0; i < IMU_STATES; i++)
			XqI[i] = XqITemp[i];
		for(int i = 0; i < INPUTS; i++)
			Uq[i] = UqTemp[i];
		for(int i = 0; i < MOTORS; i++)
			DMCs[i] = DMCsTemp[i];
		apStatus = apStatusTemp;
		volts = voltsTemp;
		cpu = cpuTemp;
		uartStatus = uartStatusTemp;
		viconStatus = viconStatusTemp;
		mode = modeTemp;
		pthread_mutex_unlock(&gcs_server_mutex);
	}

	return result;
}


// Gets the state variables
void GcsServerThread::getStates(double states[])
{
	for(int i = 0; i < STATES; i++)
		states[i] = Xq[i];
}


// Gets the state references
void GcsServerThread::getReferences(double refs[])
{
	for(int i = 0; i < STATES; i++)
		refs[i] = XqR[i];
}


// Gets the input variables
void GcsServerThread::getInputs(double inputs[])
{
	for(int i = 0; i <  INPUTS; i++)
		inputs[i] = Uq[i];
}


// Gets the Vicon measurements
void GcsServerThread::getVicon(double vicon[])
{
	for(int i = 0; i < VIC_STATES; i++)
		vicon[i] = XqV[i];
}


// Gets the IMU measurements
void GcsServerThread::getImu(double imu[])
{
	for(int i = 0; i < IMU_STATES; i++)
		imu[i] = XqI[i];
}


// Gets the DMCs of the quadrotor
void GcsServerThread::getDMCs(unsigned char dmcs[])
{
	for(int i = 0; i < MOTORS; i++)
		dmcs[i] = DMCs[i];
}


// Gets the short responsible for representing the Autopilot's states.
short GcsServerThread::getAutopilotStatus()
{
	return apStatus;
}

// Gets the voltage of the quadrotor battery
double GcsServerThread::getVolts()
{
	return volts;
}


// Gets the CPU load percentage
double GcsServerThread::getCPU()
{
	return cpu;
}


// Gets the state of the serial link
ConnStatus GcsServerThread::getSerialStatus()
{
	return uartStatus;
}


// Gets the state of the Vicon link
ConnStatus GcsServerThread::getViconStatus()
{
	return viconStatus;
}

// Returns the quadrotor mode
FlightMode GcsServerThread::getMode()
{
	return mode;
}


// Starts the thread shutdown process
void GcsServerThread::startShutdown()
{
	running = false;
}


// Kills the thread
void GcsServerThread::killThread()
{
	pthread_cancel(the_thread);
}


// The actual function that gets sent to its own thread. This function starts a loop that
// continually scans for quadrotor frames, then processes them appropriately.
void* gcsServerThreadFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct GcsServerStruct *gstruct;
	gstruct = (struct GcsServerStruct *) threadarg;	

	// declaring the appropriate variables
	QuadServer* server  = gstruct->struct_server;
	double* Xq          = gstruct->struct_Xq;
	double* XqR			= gstruct->struct_XqR;
	double* Uq          = gstruct->struct_Uq;
	double* XqV         = gstruct->struct_XqV;
	double* XqI         = gstruct->struct_XqI;
	unsigned char* DMCs = gstruct->struct_DMCs;
	short* apStatus		= gstruct->struct_ap_status;
	double* volts       = gstruct->struct_volts;
	double* cpu         = gstruct->struct_cpu;
	ConnStatus* uartStatus	= gstruct->struct_uart_stat;
	ConnStatus* viconStatus   = gstruct->struct_vic_stat;
	FlightMode* mode    = gstruct->struct_mode;
	bool* connected     = gstruct->struct_connected;
	bool* updated       = gstruct->struct_updated;
	bool* running       = gstruct->struct_running;

	// other declarations
	quad2gcs frameReceived, *frameReceivedPtr = &frameReceived;

	// Keep getting values until flagged to stop
	while(*running)
	{
		do
		{
			// grabs data; blocks if connected, returns "false" if the connection is lost
			// or if there was an error. Otherwise, only returns once it gets the
			// desired number of bytes. 
			if(!server->demandData((char *)frameReceivedPtr, sizeof(frameReceived)))
			{
				*connected = false;			
				cout << "Lost connection to client. Looking again..." << endl;
				server->listenNow();
				server->acceptNow();
				*connected = true;
			}
		}while(!*connected);

		pthread_mutex_lock(&gcs_server_mutex);
		unpackQuad2GcsFrame(frameReceived, Xq, XqR, Uq, 
			XqV, XqI, DMCs, 
			*apStatus, *volts, *cpu, 
			*uartStatus, *viconStatus, *mode);
		pthread_mutex_unlock(&gcs_server_mutex);

		*updated = true;
	}

	// Kill the thread
	pthread_exit(NULL);
}
