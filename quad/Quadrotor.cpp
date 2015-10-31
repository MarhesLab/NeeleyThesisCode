// Class Definition
#include "Quadrotor.h"

// Standard Libraries
#include <iostream> // for cout
#include <iomanip> // for setting precision
#include <stdio.h> // for exit()
#include <unistd.h> // for usleep

// Custom Classes/Functions/Definitions
#include "modes.h"
#include "parameters.h"
#include "../defs.h"
#include "../functions/transforms.h"

// Sets standard namespace to avoid writing "std::" everywhere
using namespace std;


// Constructor
Quadrotor::Quadrotor(const int quadNumber)
{
	// Value Initialization
	apActive = false;
	vicNew = false;
	vicActive = false;
	vicValid = false;
	vicOldValid = false;
	uartStatus = CONN_DISCONNECTED;
	viconStatus = CONN_DISCONNECTED;
	currentMode = QUAD_OFF;
	desiredMode = QUAD_OFF;
	previousMode = QUAD_OFF;
	
	cyclesSinceVicUpdate = 0;
	cyclesTilVicVel = 3;
	//rampUpCount = 0;

	// Array pointer initialization
	XqImu  		 	= XqImu_all[TIME_CUR];
	XqImu_lagged 	= XqImu_all[TIME_VIC];
	Xq     		 	= Xq_all[TIME_CUR];
	Xq_lagged		= Xq_all[TIME_VIC];
	Xq_lagged_old 	= Xq_all[TIME_VIC-1];
	Xq_new 		 	= Xq_all[TIME_NEW];
	Uq     		 	= Uq_all[TIME_NEW];

	loadParameters(quadNumber);
	initArrays();

	// Class initializations
	filter = new Filter3(Xq_lagged_old, Xq_lagged, Uq, XqVicon, XqVicon_old, XqImu_w, quadNumber); // "../functions/Filter3.cpp"
	ctrl = new Controller(Xq_new, XqTraj, XqRef, UqCmd, Uq, omega, motorDMCs, quadNumber); // "../functions/Controller.cpp"
	trajmkr = new TrajectoryMaker(XqTraj); // "../functions/TrajectoryMaker.cpp"
	
	// Thread initialization
	autopilot = new AutopilotThread();	// "../thread/AutopilotThread.cpp"
	vicon = new ViconThread(quadName);	// "../thread/ViconThread.cpp"
	quadclient = new QuadClientThread(Xq, XqRef, Uq, XqVicon, XqImu,
		motorDMCs, &apStatus, &apVoltage, &apCpuLoad,
		&uartStatus, &viconStatus, &currentMode);	// "../thread/QuadClientThread.cpp"
}


// Destructor
Quadrotor::~Quadrotor()
{
	// Starts the thread shutdown processes
	autopilot->startShutdown();		// "../thread/AutopilotThread.cpp"
	quadclient->startShutdown();	// "../thread/QuadClientThread.cpp"
	vicon->startShutdown();			// "../thread/ViconThread.cpp"

	// send signals to the threads and control loops that things are shutting down
	currentMode = QUAD_EXITING;
	autopilot->sendStopCommand();		// the outgoing connection to the Autopilot
										// won't shut down when the thread does, so
										// putting this here is fine.

	// sleep to let the change take effect
	usleep(10000);

	// Kills the threads
	autopilot->kill();
	quadclient->killThread();
	vicon->kill();

	// Destroys classes
	delete ctrl;
	delete filter;
	delete trajmkr;
	delete autopilot;
	delete quadclient;
	delete vicon;
}


// Quadrotor parameter initialization
// values (like mass, rotational inertia, etc.) are loaded from "parameters.cpp"
// based on the quadrotor index number given at Quadrotor class construction
// time. This value gets set in "mainProgram.cpp".
void Quadrotor::loadParameters(const int quadNumber)
{
	// loads Vicon name
	quadName = vicon_names[quadNumber];
	
	// loads inertia parameters
	Mq   = Mq_meas[quadNumber];
	Jqx  = Jqx_meas[quadNumber];
	Jqy  = Jqy_meas[quadNumber];
	Jqz  = Jqz_meas[quadNumber];
	
	// loads length paramters
	Lq   = Lq_meas[quadNumber];
	
	// loads propeller parameters
	Rp   = Rp_meas[quadNumber];
	Bp   = Bp_calc[quadNumber];
	Kp   = Kp_calc[quadNumber];

	return;
}


// Initializes all arrays to default values. This pretty much means
// being set to zero, except for quaternion q0 values (which are set to 1).
//
// NOTICE: If you've gotten this far and don't understand quaternions,
// you should change that. Here's a good reference:
// http://math.ucr.edu/~huerta/introquaternions.pdf
void Quadrotor::initArrays()
{
	// Initializes Vicon arrays to 0
	for(int i = 0; i < VIC_STATES; i++)
		{ XqVicon_old[i] = 0; XqVicon[i] = 0; }

	// Initializes IMU arrays to 0
	for(int i = 0; i < TIME_IMU; i++) {
		for(int j = 0; j < IMU_STATES; j++)
			XqImu_all[i][j] = 0;
	}
	for(int i = 0; i < IMU_STATES; i++) {
		XqImu_w[i] = 0;
	}

	// Initializes input arrays to 0
	for(int i = 0; i < TIME_TOT; i++) {
		for(int j = 0; j < INPUTS; j++) {
			Uq_all[i][j] = 0;
		}
	}
	for(int i = 0; i < INPUTS; i++) { 
		UqCmd[i] = 0;
	}

	// Initializes motor arrays to 0
	for(int i = 0; i < MOTORS; i++) { 
		omega[i] 		= 0; 
		motorDMCs[i] 	= 0; 
		newDMCs[i] 		= 0; 
		motorRPMs[i] 	= 0;
	}

	// Initializes state arrays to 0, except for values associated with q0
	for(int i = 0; i < TIME_TOT; i++) {
		for(int j = 0; j < STATES; j++) {
			Xq_all[i][j] = 0;
		}
		Xq_all[i][Q0] = 1; // quaternion must have unit magnitude
	}
	for(int i = 0; i < STATES; i++) { 
		XqRef[i] 	= 0; 
		XqRefGCS[i] = 0;
	}
	// Sets q0 for all terms to 1; quaternion must have unit magnitude
	XqRef[Q0] = 1;
	XqRefGCS[Q0] = 1;
	XqRefGCS[ Z] = 0.75;

	// Initializes trajectory arrays to 0
	for(int i = 0; i < POS_REFS; i++)
		XqTraj[i] = 0;

	return;
}


// Commands all classes (except the one used for logging) to
// get started and ready to go.
void Quadrotor::initialize()
{
	// Setting up the Filter3 class
	filter->useQuickFilter();		// "../functions/Filter3.cpp"
	//filter->calcAd(Xq, Uq);

	// Starting Autopilot connection
	uartStatus = CONN_CONNECTING;
	autopilot->initialize();		// connects to port "AP_SERIAL" with baud rate
	autopilot->startThreads();		// "BAUD_RATE", both found in "../defs.cpp"
	uartStatus = CONN_CONNECTED;

	// Starting Client connection with GCS Server
	quadclient->initialize();		// connects to "GCS_IP" found in "../defs.cpp"
	quadclient->startThread();

	// Starting Vicon connection
	viconStatus = CONN_CONNECTING;
	vicon->initialize(VICON_IP); 	// connects to "VICON_IP" found in "../defs.cpp"
	vicon->startThreads();
	viconStatus = CONN_CONNECTED;

	return;
}


// Attempts to get information from the Autopilot, Vicon system, and 
// Ground Control Station (GCS). As many of the necessary operations (reading
// from the serial port, waiting for a Vicon frame, waiting for a GCS 
// frame) would normally hold up ("block") the main function, these tasks 
// were pushed into their own dedicated threads that Linux manages and multitasks.
// When information's ready, this function grabs the data from the threads.
// "AutopilotThread" and "ViconThread" have extra watchdog threads whose sole task
// is to make sure they're regularly getting updated information. For more info on
// how a particular thread works, see its appropriate files. For more info on using
// threads, see the "POSIX Threads Programming Resource" PDF in "../thread".
void Quadrotor::updateFromThreads()
{
	// Getting Autopilot values
	apActive = autopilot->connectionActive();	// see "../thread/AutopilotThread.cpp"
	if(autopilot->newMeasurement())				// and "../functions/autopilot.cpp"
		autopilot->getMeasurement(XqImu);		// for more information (if you want)
	if(autopilot->newStatus()) {
		apStatus = autopilot->getAPStatus();
		apVoltage = autopilot->getBatteryVoltage();
		apCpuLoad = autopilot->getCPULoadPercentage();
		autopilot->getMotorRPMs(motorRPMs);
	}
	if(apActive)
		apStatus = CONN_CONNECTED;
	else
		apStatus = CONN_CONNECTING;

	// Getting values from GCS Server
	if(quadclient->gotFrame()) {				// see "../thread/QuadClientThread.cpp"
		quadclient->getTrajectory(XqTrajGCS);	// and "../socket/QuadClient.cpp"
		//quadclient->getReference(XqRefGCS);		// for more information (if you want)
		quadclient->getSpeeds(newDMCs);
		desiredMode = quadclient->getMode();
	}
	// NOTE: the XqRefGCS update call is currently disabled until the appropriate
	// logic is implemented on the GCS. For now, it is left at a default altitude
	// of 0.75 m, q0 = 1, and all the rest at 0.

	
	// Getting Vicon values
	if(vicon->newMeasurement()) {				// see "../thread/ViconThread.cpp"
		vicValid = vicon->measurementValid();	// and "../vicon/ViconClass.cpp"
		vicon->getMeasurement(XqVicon);			// for more information (if you want)
		vicNew = true;
		cyclesSinceVicUpdate = 0;

		if(!trajmkr->homeIsSet())
			trajmkr->setHome(XqVicon);

		if(!trajmkr->groundIsSet())
			trajmkr->setGroundAlt(XqVicon[Zv]);
	}
	if(vicon->connActive()) {
		vicActive = true;
		viconStatus = CONN_CONNECTED;
	}
	else {
		vicActive = false;
		viconStatus = CONN_DISCONNECTED;
	}

	return;
}


// Sends a frame to the Ground Control Station (GCS) containing state, input,
// reference, trajectory, and basic status information so it can be
// displayed for the user/tester/debugger/pilot. 
void Quadrotor::updateGCS()
{
	quadclient->sendFrame();	// "../thread/QuadClientThread.cpp"
	return;
}


// Updates the (potentially) lagged states based on the (potentially) lagged 
// Vicon measurements and (potentially) artificially lagged IMU measurements.
// The logic behind whether or not the Vicon measurements are treated as "lagged"
// is determined by the "VIC_LAT" definition in "../defs.h". This gets complicated
// fairly quickly, so see the explanation found in "../defs.h" for more info.
// It may just redirect you to my (Wil Neeley's) thesis, or it may not. That can change.
void Quadrotor::updateStates()
{
	
	// Adapts the accelerometer IMU values from Body frame to World frame;
	// function found in "../functions/transforms.cpp"
	quatRot(&XqImu_lagged[AXi], &XqImu_w[AXi], &XqVicon[Q0]);

	// Removes gravity from IMU value; it shows up as a positive value
	// for some reason, so it has to be subtracted out
	XqImu_w[AZi] -= G;

	// Keeps angular velocities in Body frame
	XqImu_w[WXi] = XqImu_lagged[WXi];
	XqImu_w[WYi] = XqImu_lagged[WYi];
	XqImu_w[WZi] = XqImu_lagged[WZi];

	// For now, as long as it's not off or idling, the Kalman filter
	// will start filtering. Simply filters both values at the same time.
	/*if(currentMode != QUAD_OFF && currentMode != QUAD_IDLE) {
		filter->kalmanPredict();
		filter->kalmanUpdateBoth();
	}
	else {
		filter->resetPmatrix();		
	}*/

	
	
	// Based on Vicon activity, determines whether or not it should postpone
	// Vicon-based velocity estimation for a few control loop cycles.
	if(!vicActive)
		cyclesTilVicVel = 3;
	else if(cyclesSinceVicUpdate > 10)
		cyclesTilVicVel = 2;
	

	// Sets whether Vicon's values should be used for position calculation.
	bool useVicPos = ( vicValid && vicActive && vicNew );
	// Sets whether Vicon's values should be used for velocity calcuation.
	bool useVicVel = ( vicOldValid && (cyclesTilVicVel == 0) );
	
	// Performs a Quick prediction and Quick update. If the Vicon measurements
	// either aren't recent or aren't valid, the Quick update merely returns
	// the values generated by the Quick prediction; no measurement meshing
	// will take place. Otherwise, the Quick update takes place as normal.
	filter->quickPredict(currentMode);		// "../functions/Filter3.cpp"
	filter->quickUpdate(useVicPos, useVicVel);

	return;
}


// If the Vicon/motor latencies defined by "VIC_LAT" and "MOT_LAT" in "../defs.h"
// are greater than zero time steps, this function performs the necessary number
// of state predictions, one time step at a time, until it has a set of predicted
// states for the present (if "MOT_LAT" = 0) or the future (if "MOT_LAT" > 0)
// that the reference, trajectory, and control law algorithms can use. As mentioned
// above for "updateStates()", this gets complicated quickly, so see the expalantion
// found in "../defs.h" for more information.
void Quadrotor::latencyCompensation()
{
	// If Vicon Latency and Motor Latency are nonexistent, do nothing and exit
	if( (VIC_LAT == 0) && (MOT_LAT == 0) )
		return;

	// Otherwise, iteratively step through the states/inputs from the filtered (lagged)
	// time step all the way to the predicted (future) time step, and predict how the
	// quadrotor's state will evolve over time.
	double *Xq_current, *Xq_past, *Uq_past; 

	for(int i = TIME_VIC+1; i < TIME_TOT; i++) {
		Xq_current 	= Xq_all[i]; 
		Xq_past 	= Xq_all[i-1];
		Uq_past 	= Uq_all[i-1];

		// "../functions/Filter3.cpp"
		filter->predictState(currentMode, Xq_past, Xq_current, Uq_past, Ts); 
	}

	return;
}


// Performs any flight mode switching logic that may be necessary (such as
// ending the Takeoff procedure when appropriate or triggering the Unsafe
// flight mode when necessary) and coordinates the current flight mode with
// the desired flight mode given by the GCS. As the logic behind these mode
// switches gets convoluted fairly quickly, dedicated functions were made to
// handle these operations. See "modes.cpp" and its corresponding
// header file for more information on how each function works.
void Quadrotor::getFlightMode()
{
	// Periodically reports the current flight mode
	static int count = 0;
	const int countMax = 50;

	count++;
	if(count >= countMax)
		cout << "\n\rCurrent Mode: " << modeString(currentMode) << "\n\r";
		
	// Checks for any mode switches that may need to take place; as the
	// modes are passed by reference, they can be changed in the function.
	modeSwitchChecks(currentMode, previousMode, Xq, XqRef, XqRefGCS, XqVicon, 
		trajmkr, rampUpCount);

		
	// Makes sure it's safe to transition to the desired flight mode; if not, doesn't
	// do it. If so, makes the transition. For special cases, changes to a different
	// flight mode instead. See the "Mode Transition Limitation Rules" Excel document 
	// in "../functions" for a table showing the transitions.
	// Function definition found in "../functions/modes.cpp"
	currentMode = desiredModeSafetyCheck(currentMode, desiredMode, 
					trajmkr, Xq, XqRefGCS);

	return;
}


// Sets the reference and trajectory values for use in various potential 
// control law calculations. While most flight modes will use the nonlinear
// controller (which requires well defined trajectories), other controllers
// like the altitude/position/attitude controller combination merely use
// reference values. The TrajectoryMaker class uses the reference values in
// its trajectory generation, so this step cannot be skipped.
void Quadrotor::setReference()
{
	// see "../functions/modes.cpp" and "../functions/TrajectoryMaker.cpp"
	// for more information as to what's happening here. 
	modeReference( currentMode, Xq, XqRef, XqRefGCS, trajmkr);		
	modeTrajectory(currentMode, Xq, XqRef,           trajmkr);

	return;
}


// Performs the appropriate control law, if any, for a given flight mode.
// Most flight modes will use the nonlinear, feedback linearization-based
// controller, while some prototyping/testing modes will make use of PD-based
// altitude/position/attitude controllers used in tandem. 
void Quadrotor::controlLaw()
{
	// see "../functions/modes.cpp" and "../functions/Controller.cpp"
	// for more information as to what's happening here.
	modeControlLaw(currentMode, Uq, ctrl);

	return;
}


// Sends the appropriate inputs to the quadrotor. For flight modes, this means
// generating motor speeds from control inputs (net thrust/torques) and sending
// the appropriate Direct Motor Control (DMC) values for the desired motor speed.
// For other modes, special logic handles setting the appropriate DMC values. The
// AutopilotThread class handles sending the DMC values to the quadrotor's
// high level controller, which in turn sends it to the low level controller, which
// in turn actually tells the motor controllers to set the right speeds.
void Quadrotor::sendInputs()
{
	// For now, simply limits the yaw torque to make sure it doesn't get too large.
	// Otherwise, it copies the other commanded inputs to the inputs to execute.
	ctrl->limitInputs();

	// Find the appropriate Direct Motor Commands for the quadrotor
	// based on the current mode of operation and other parameters.
	//modeSetMotors(currentMode, Uq, omega, motorDMCs, newDMCs, rampUpCount,
	//		Lq, Bp, Kp, ctrl);
	modeSetMotors(currentMode, newDMCs, rampUpCount, ctrl);
	
	// Send the direct motor commands back to the quadrotor
	autopilot->sendDMCs(motorDMCs);
}


// Initializes the multithreaded logging class that's used to log states,
// inputs, references, trajectories, and status parameters to a CSV file.
// As writing to a file can cause large delays with the operating system,
// the LoggerThread class creates a dedicated thread for writing information
// to the file. Previous non-multithreaded implementations caused consistently
// periodic delays upwards of 350 ms, which is catastrophic for controls programs
// on sensitive systems like quadrotors. See "../thread/LoggerThread.cpp" for more
// information about how the process works, if you're curious. If you aren't curious,
// tisk tisk, but rest assured that it works and it helps a lot.
void Quadrotor::startLogging()
{
	// Start up the multithreaded logger
	logger.setName("logfile_v3.0.csv");
	logger.initialize();
	//if(!logger.startThread())
	//	exit(-1);

	// Set up log file header information
	logger.addLogTimeLine(); // logs the date and time in the first line
	logger.addTextLine("Quadrotor Log File Version: 3.0"); // lists the log file version on the second line
	
	// Logs the recorded quadrotor parameters on the third line
	stringstream quad_params;
	quad_params << "Name: " << quadName
		<< " M: " << setprecision(4) << Mq
		<< " Jxx: " << setprecision(5) << Jqx
		<< " Jyy: " << setprecision(5) << Jqy
		<< " Jzz: " << setprecision(5) << Jqz
		<< " L: " << setprecision(3) << Lq
		<< " Bp: " << setprecision(12) << Bp
		<< " Kp: " << setprecision(12) << Kp;
	logger.addTextLine(quad_params.str());	
	
	// starts naming columns on the fourth line
	logger.addTextBlock("seconds,");								// Time since logger started
	logger.addTextBlock("X,Y,Z,dX,dY,dZ,q0,qi,qj,qk,Wx,Wy,Wz,");	// Quadrotor states
	logger.addTextBlock("X_v,Y_v,Z_v,q0_v,qi_v,qj_v,qk_v,");		// Vicon states
	logger.addTextBlock("Wx_i,Wy_i,Wz_i,ddX_i,ddY_i,ddZ_i,");	// IMU states (body frame)
	logger.addTextBlock("Xr,Yr,Zr,dXr,dYr,dZr,");				// Reference states
	logger.addTextBlock("q0r,qir,qjr,qkr,Wxr,Wyr,Wzr,");
	logger.addTextBlock("Xt,dXt,ddXt,dddXt,ddddXt,");			// Trajectory states
	logger.addTextBlock("Xt,dXt,ddXt,dddXt,ddddXt,");
	logger.addTextBlock("Xt,dXt,ddXt,dddXt,ddddXt,");
	logger.addTextBlock("Ozt,dOzt,ddOzt,");
	logger.addTextBlock("Th_c,Tx_c,Ty_c,Tz_c,");					// Commanded input
	logger.addTextBlock("Th_a,Tx_a,Ty_a,Tz_a,");					// Actual input
	logger.addTextBlock("w1,w2,w3,w4,");							// Motor speeds
	logger.addTextBlock("M1,M2,M3,M4,");							// Motor DMCs
	logger.addTextBlock("RPM1,RPM2,RPM3,RPM4,");					// Motor RPMs (meas.)
	logger.addTextBlock("currentMode,desiredMode");				// Flight Modes
	logger.newLine();
	
	// starts logging timing
	logtimer.tic();
}


// Writes all the appropriate state/input/reference/trajectory/status information
// to the log file. Again, if you're curious how the multithreaded logger works,
// feel free to look at "../thread/LoggerThread.cpp" for more information.
void Quadrotor::logValues()
{
	// Gets the log timer
	logtimer.toc();
	
	// Starts logging values; this should match the order spelled out by the
	// log file initializer in "startLogging()" above.
	logger.logDouble(logtimer.secs(), 6);
	logger.logDoubleArray(Xq, STATES, 5);
	logger.logDoubleArray(XqVicon, VIC_STATES, 5);
	logger.logDoubleArray(XqImu, IMU_STATES, 5);
	logger.logDoubleArray(XqRef, STATES, 5);
	logger.logDoubleArray(XqTraj, POS_REFS, 4);
	logger.logDoubleArray(UqCmd, INPUTS, 6);
	logger.logDoubleArray(Uq, INPUTS, 6);
	logger.logDoubleArray(omega, MOTORS, 5);
	logger.logUCharArray(motorDMCs, MOTORS);
	logger.logDoubleArray(motorRPMs, MOTORS, 6);
	logger.logInt(currentMode);
	logger.logInt(desiredMode);
	logger.newLine();
}


// Updates values for the next loop. As the two-dimensional arrays are set up in
// a static manner (as opposed to using <vector> or using pointers to arrays to
// create new/shift/destroy arrays of state values at a particular time), this involves
// iteratively shifting values via equality statements. 
//
// NOTE: The pointers-to-arrays method was examined for speed; while it was faster
// for larger arrays with more time steps present, the update process was found to 
// take longer for less time steps due to having to free up/reallocate memory in 
// the OS. Periodic delay spikes would occur as the OS tried managing the memory.
// The ponters-to-arrays method also would have required rewriting classes 
// that were written to only point at one chunk of memory for the whole program,
// so the idea was scrapped. 
void Quadrotor::prepareForNextLoop()
{
	// Keeps track of Vicon activity for filtering purposes
	cyclesSinceVicUpdate++;
	if(cyclesSinceVicUpdate > 2000)	// prevents overflow
		cyclesSinceVicUpdate = 1000;
	
	if(cyclesTilVicVel > 0)
		cyclesTilVicVel--;

	// Shifts all the recorded IMU values
	for(int i = 0; i < TIME_IMU-1; i++) {
		for(int j = 0; j < IMU_STATES; j++)
			XqImu_all[i][j] = XqImu_all[i+1][j];
	}

	// Shifts all the recorded state values
	for(int i = 0; i < TIME_TOT-1; i++) {
		for(int j = 0; j < STATES; j++)
			Xq_all[i][j] = Xq_all[i+1][j];
	}

	// Shifts all the recorded IMU values
	for(int i = 0; i < TIME_TOT-1; i++) {
		for(int j = 0; j < INPUTS; j++)
			Uq_all[i][j] = Uq_all[i+1][j];
	}

	// Copies over Vicon values
	vicNew = false;
	for(int i = 0; i < VIC_STATES; i++)
		XqVicon_old[i] = XqVicon[i];

	// Flags that the last set of Vicon values isn't valid; if Vicon
	// becomes active again, this will prevent a massive derivative from being generated
	if(vicActive)
		vicOldValid = vicValid;
	else
		vicOldValid = false;
}
