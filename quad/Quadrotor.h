#ifndef QUAD_H
#define QUAD_H

// Standard Libraries
#include <string>

// Custom Classes/Functions/Definitions
#include "../defs.h"
#include "../functions/Controller.h"
#include "../functions/Filter3.h"
#include "../functions/Timer.h"
#include "../functions/TrajectoryMaker.h"
#include "../thread/AutopilotThread.h"
#include "../thread/LoggerThread.h"
#include "../thread/QuadClientThread.h"
#include "../thread/ViconThread.h"

using namespace std;

// Quadrotor Class Definition
class Quadrotor {
protected:
	// Pointers to Multithreaded Class Objects
	AutopilotThread *autopilot;	
	QuadClientThread *quadclient;
	ViconThread *vicon;

	// Connection Health Variables
	// enumeration type defined in "../defs.h"
	enum ConnStatus uartStatus;
	enum ConnStatus viconStatus;

	// Vicon-related variables
	bool vicNew;					// indicates a Vicon measurement is new and has not1
									// been processed before
	bool vicActive;					// indicates new measurements arrived recently
	bool vicValid, vicOldValid;		// indicates received values aren't phony
	double XqVicon[VIC_STATES];		// Latest Vicon measurements (could be laggy; 
									// set "VIC_LAT" in "../defs.h" to the average 
									// # of time steps (relative to "Ts") by which
									// the received Vicon system lags reality.
	double XqVicon_old[VIC_STATES];	// The previous Vicon measurements
	string quadName;				// The name of the quadrotor in the Vicon system

	// Autopilot-related variables
	bool apActive;
	double XqImu_all[TIME_IMU][IMU_STATES]; // keeps track of raw IMU values over time
	double *XqImu;	            // points to the current raw IMU values measured in the 
								// quadrotor's Body frame
	double *XqImu_lagged;		// points to the IMU values associated with the 
								// latest Vicon measurements (Body frame)
	double XqImu_w[IMU_STATES]; // IMU values adapted to align with the World frame
	short apStatus;				// raw status value from Low Level Processor (LLP)
	double apVoltage;			// raw quadrotor battery voltage
	double apCpuLoad;			// raw High Level Processor (HLP) CPU load (percentage)
	double motorRPMs[MOTORS];	// measured motor RPM values
	
	// Ground Control Station (GCS)-related values
	double XqTrajGCS[POS_REFS]; // trajectory sent from the GCS
	double XqRefGCS[STATES];    // reference sent from the GCS
	unsigned char newDMCs[MOTORS]; // DMC values sent from the GCS (only for 
	FlightMode desiredMode;
	

	// Quadrotor Parameters	
	double Mq, Jqx, Jqy, Jqz; // masses and inertias
	double Lq;                // boom arm length
	double Rp, Bp, Kp;        // propeller radius, thrust factor, and drag factor
	
	
	// State and Filtering Parameters
	int cyclesTilVicVel;			// allows Vicon-based velocity calculation to be postponed
	int cyclesSinceVicUpdate;		// keeps track of the number of loops since Vicon updated
	Filter3* filter;
	double Xq_all[TIME_TOT][STATES]; // holds all the state vectors over time 
									 // (past, present, and future; these are
									 // used for filtering/latency compensation)
	double *Xq; 			// points at the "current" state vector
	double *Xq_lagged;		// points to the state values associated with the latest
							// Vicon measurements
	double *Xq_lagged_old;	// points at the state values associated with the previous
							// Vicon measurements (used for filtering)
	double *Xq_new;			// points at the "future" state vector (used in control laws)
	

	// Input and Control Parameters
	Controller* ctrl;
	TrajectoryMaker* trajmkr;
	double Uq_all[TIME_TOT][INPUTS]; // holds all the input vectors over time 
									 // (past, present, and future; these are
									 // used for filtering/latency compensation)
	double *Uq;				// points at the "future" input (what gets sent to the
							// quadrotor after latency compensation)
	double UqCmd[INPUTS];   // holds what the control law generated
	double omega[MOTORS];   // gives the commanded motor speed
	unsigned char motorDMCs[MOTORS];	// raw motor values sent to the quadrotor's HLP
	double XqTraj[POS_REFS];	// Trajectory values used in nonlinear, feedback 
								// linearization-based controller
	double XqRef[STATES];
	int rampUpCount;
	

	// Status Parameters
	// enumeration type defined in "../defs.h"
	enum FlightMode currentMode;
	enum FlightMode previousMode;
	
	// Timing and Logging parameters
	LoggerThread logthread;
	Timer logtimer;
	
	// Private function declarations; 
	// used internally for initialization
	void loadParameters(const int quadNumber);
	void initArrays();


public:
	// Constructor and Destructor
	Quadrotor(const int quadNumber);
	~Quadrotor();
	
	// Initialization Function
	void initialize();

	// Obtaining Measurements and Status Values
	void updateFromThreads();
	void updateGCS();
	void updateStates();
	void latencyCompensation();
	
	// Flight Mode and Reference Determination
	void getFlightMode();
	void setReference();
	void controlLaw();
	void sendInputs();
	
	// Logging and End of Loop Functions
	void startLogging();
	void logValues();
	void prepareForNextLoop();
};

#endif
