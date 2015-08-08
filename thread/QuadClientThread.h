#ifndef QUAD_CLIENT_THREAD_H
#define QUAD_CLIENT_THREAD_H

#include <pthread.h>

#include "../socket/packet.h"
#include "../socket/QuadClient.h"
#include "../defs.h"

// The function to be transferred to its own thread
void* quadClientThreadFunction(void *threadarg);

// Struct needed for passing data to the thread
struct QuadClientStruct 
{
	//pthread_mutex_t* struct_mutex;
	QuadClient* struct_client;
	double* struct_XqTraj;
	double* struct_XqRef;
	unsigned char* struct_DMCs;
	enum FlightMode* struct_mode;
	bool* struct_connected;
	bool* struct_updated;
	bool* struct_running;
};

// Quadrotor Client Thread Definition
class QuadClientThread 
{
  protected:
	// Threading values
	QuadClientStruct qstruct;
	pthread_t the_thread;
	//pthread_mutex_t the_mutex;
	bool connected;
	bool updated;
	bool running;
	
	// Client object
	QuadClient client;

	// Sent data values (mostly pointers to arrays containing the values to send)
	quad2gcs frameToSend, *frameToSendPtr;
	double *Xq;
	double *XqR;		// Quadrotor's current reference
	double *Uq;
	double *XqV;
	double *XqI;
	unsigned char *DMCs;
	short *apStatus;
	double *volts;
	double *cpu;
	enum ConnStatus *uartStatus;
	enum ConnStatus *viconStatus;
	enum FlightMode *mode;
	
	// Received data values
	double XqTraj[POS_REFS];
	double XqTrajTemp[POS_REFS];
	double XqRef[STATES];	// GCS's desired reference
	double XqRefTemp[STATES];
	unsigned char newDMCs[MOTORS];
	unsigned char newDMCsTemp[MOTORS];
	enum FlightMode newMode, newModeTemp;
	
  public:
	QuadClientThread(double *Xqp, double *XqRp, double *Uqp, double *XqVp, double *XqIp,
		unsigned char *DMCp, short *apsp, double *vp, double *cp, 
		ConnStatus *usp, ConnStatus *vsp, FlightMode *mp);
	~QuadClientThread();

	void initialize();
	bool startThread();

	void sendFrame();

	bool gotFrame();
	void getTrajectory(double Traj[]);
	void getReference(double Ref[]);
	void getSpeeds(unsigned char DMCs[]);
	FlightMode getMode();

	void startShutdown();
	void killThread();
};

#endif
