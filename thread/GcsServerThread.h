#ifndef GCS_SERVER_THREAD_H
#define GCS_SERVER_THREAD_H

#include <pthread.h>

#include "../socket/packet.h"
#include "../socket/QuadServer.h"
#include "../defs.h"

// The function to be transferred to its own thread
void* gcsServerThreadFunction(void *threadarg);

// Struct needed for passing data to the thread
struct GcsServerStruct 
{
	//pthread_mutex_t* struct_mutex;
	QuadServer* struct_server;
	double* struct_Xq;
	double* struct_XqR;
	double* struct_Uq;
	double* struct_XqV;
	double* struct_XqI;
	unsigned char* struct_DMCs;
	short* struct_ap_status;
	double* struct_volts;
	double* struct_cpu;
	ConnStatus* struct_uart_stat;
	ConnStatus* struct_vic_stat;
	enum FlightMode* struct_mode;
	bool* struct_connected;
	bool* struct_updated;
	bool* struct_running;
};

// Quadrotor Client Thread Definition
class GcsServerThread 
{
  protected:
	// Threading values
	GcsServerStruct gstruct;
	pthread_t the_thread;
	bool running;
	
	// Client object
	QuadServer server;

	// Sent data values (mostly pointers to arrays containing the values to send)
	struct gcs2quad frameToSend, *frameToSendPtr;
	double* XqTraj;									// GCS desired trajectory
	double* XqRef;									// GCS desired reference
	unsigned char* newDMCs;							// GCS desired DMCs
	FlightMode* newMode;							// GCS desired flight mode

	// Received data values
	double Xq[STATES], XqTemp[STATES];				// quadrotor's filtered state
	double XqR[STATES], XqRTemp[STATES];			// quadrotor's current reference
	double Uq[INPUTS], UqTemp[INPUTS];				// quadrotor's current input
	double XqV[VIC_STATES], XqVTemp[VIC_STATES];	// raw Vicon measurements
	double XqI[IMU_STATES], XqITemp[IMU_STATES];	// raw IMU measurements
	unsigned char DMCs[MOTORS], DMCsTemp[MOTORS];	// quadrotor's commanded DMCs
	short apStatus, apStatusTemp;					// 
	double volts, voltsTemp;
	double cpu, cpuTemp;
	enum ConnStatus uartStatus, uartStatusTemp;
	enum ConnStatus viconStatus, viconStatusTemp;
	enum FlightMode mode, modeTemp;

	
  public:
	bool connected;
	bool updated;	

	GcsServerThread(double* XqTp, double* XqRp, unsigned char* nDMCp, FlightMode* nMp);
	~GcsServerThread();

	void initialize();
	void listenAndAccept();
	bool startThread();

	void sendFrame();

	bool gotFrame();
	void getStates(double states[]);
	void getReferences(double refs[]);
	void getInputs(double inputs[]);
	void getVicon(double vicon[]);
	void getImu(double imu[]);
	void getDMCs(unsigned char dmcs[]);
	short getAutopilotStatus();
	double getVolts();
	double getCPU();
	ConnStatus getSerialStatus();
	ConnStatus getViconStatus();
	FlightMode getMode();

	void startShutdown();
	void killThread();
};

#endif
