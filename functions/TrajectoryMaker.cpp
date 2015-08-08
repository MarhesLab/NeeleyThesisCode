#include <math.h>

#include "TrajectoryMaker.h"

#include "transforms.h"

// Takeoff/Land Values
const double takeoffAlt = 0.75;	// altitude for takeoff termination (meters)
const double takeoffTime = 2.5;	// time for takeoff process (seconds)
const double landTime = 3;		// time for landing process (seconds)


// Figure Eight Values
const double fig8alt    = 0.75;	// sets the average altitude for the figure 8
const double fig8width  = 1.50; // sets the width (in X)
const double fig8length = 2.00;	// sets the length (in Y)
const double fig8height = 0.50;	// sets the max variation in altitude

const double fig8period = 12;	// sets the period for the figure 8


// Special Pattern Values
const double specAlt = 0.75;		// sets the average altitude
const double specRadius = 0.75;		// sets the radius for the X/Y circle
const double specAltMag = 0.50;		// sets the max variation from average altitude

const double specCircPeriod = 5;	// sets the period for the X/Y circle
const double specAltPeriod = 20;	// sets the period for the Z oscillation
const double specRotPeriod = 20;	// sets the period for heading rotation


// Decision Boundary Buffer Values
const double groundBuffer = 0.05; // Creates a buffer from the actual ground altitude
							// for checking when the quadrotor's near the ground.
							// Buffer unit is in meters.
const double waypointZBuffer = 0.05; // Creates a square around a given waypoint; when
							// the quadrotor is detected in the square, it is
							// said to be at the waypoint. Units are in meters.
const double waypointXYBuffer = 0.05;



// Constructor
TrajectoryMaker::TrajectoryMaker(double *XqTrajp)
{
	// Initializes the waypoint time value
	waypointTime = 1.0;

	// Sets up the trajectory pointer appropriately
	XqTraj = XqTrajp;

	// Initializes values appropriately
	currentMode = QUAD_OFF;
	previousMode = QUAD_OFF;
	modeChanged = false;
	refChanged = false;
	groundSet = false;
	homeSet = false;
}

// Destructor
TrajectoryMaker::~TrajectoryMaker()
{

}



// Checks if the ground altitude has been set
bool TrajectoryMaker::groundIsSet()
{
	return groundSet;
}

// Sets the ground altitude
void TrajectoryMaker::setGroundAlt(double height)
{
	groundSet = true;
	groundAlt = height;
}

// Checks if the quadrotor is close to the ground
bool TrajectoryMaker::nearGround(double Xq[])
{
	// if it's above the ground and ground buffer, returns false
	if(Xq[Z] > (groundAlt + groundBuffer))
		return false;

	// Otherwise, returns true
	return true;
}



// Checks if the Home location was already set
bool TrajectoryMaker::homeIsSet()
{
	return homeSet;
}

// Sets the Home location
void TrajectoryMaker::setHome(double XqVicon[])
{
	homeSet = true;

	for(int i = 0; i < STATES; i++)
		XqHome[i] = 0;

	XqHome[ X] = XqVicon[ Xv];
	XqHome[ Y] = XqVicon[ Yv];
	//XqHome[ Z] = XqVicon[ Zv];
	XqHome[Q0] = XqVicon[Q0v];
	XqHome[QI] = XqVicon[QIv];
	XqHome[QJ] = XqVicon[QJv];
	XqHome[QK] = XqVicon[QKv];

	// Hard-codes the XqHome altitude to be the same as the takeoff altitude
	XqHome[ Z] = takeoffAlt;
}

// Gets an individual Home state value
double TrajectoryMaker::getHomeState(int index)
{
	return XqHome[index];
}



// Checks for any changes, and implements any appropriate logic
void TrajectoryMaker::checkForChanges(FlightMode newMode, double XqRef[])
{
	// Updates the flight mode information
	previousMode = currentMode;
	currentMode = newMode;

	// If the new mode is different from the previous one, flag that it was
	// changed and reset the flight path timer. This also resets any mode flags,
	// such as the "startSet" variable
	if(currentMode != previousMode)
	{
		modeChanged = true;
		startSet = false;
		pathTimer.tic();
	}

	// Compares all incoming reference values with current values;
	// if there's a change, a flag is set, and the current/previous values are updated
	refChanged = false;
	for(int i = 0; i < STATES; i++) {
		if(XqRef[i] != XqRefCurr[i]) {
			refChanged = true;
			break;
		}
	}
	if(refChanged) {
		for(int i = 0; i < STATES; i++) {
			XqRefPrev[i] = XqRefCurr[i];
			XqRefCurr[i] = XqRef[i];
		}
	}
}



// Executes the trajectory generation for taking off
void TrajectoryMaker::takeoff(double Xq[])
{
	// If the mode was just changed to takeoff, reset the flag and properly set 
	// the takeoff starting point. The takeoff ending point is set to directly
	// above the starting point at "takeoffAlt" height.
	if(modeChanged) {
		modeChanged = false;

		for(int i = 0; i < STATES; i++)
			XqStart[i] = Xq[i];

		startSet = true;
	}

	// Initially sets all references (X, Y, Z, yaw, and derivatives) to zero
	for(int i = 0; i < POS_REFS; i++)
		XqTraj[i] = 0;

	// Sets X and Y to stay above the takeoff point
	XqTraj[  X_t] = XqStart[X];
	XqTraj[  Y_t] = XqStart[Y];
	
	// Uses a cosine sweep to transition from starting altitude to ending altitude
	// (see the function call for a more in depth description of the altitude profile).
	// "takeoffTime" and "takeoffAlt" are set as globals at the top of this file.
	pathTimer.toc();
	cosineSweep( &XqTraj[  Z_t], XqStart[Z], takeoffAlt, 
		pathTimer.secs(), takeoffTime);

	// Sets the yaw angle to stay constant from the takeoff state
	//XqTraj[PSI_t] = quat2Yaw(&XqStart[Q0]);

	// Uses the same cosine sweep to set the reference yaw angle to go from the
	// initial yaw value to 0 radians.
	cosineSweepHeading( &XqTraj[PSI_t], quat2Yaw(&XqStart[Q0]), 0,
		pathTimer.secs(), takeoffTime);
}



// Executes the trajectory generation for hovering
void TrajectoryMaker::hover(double XqRef[])
{
	// If the mode was just changed to hovering, only reset the flag.
	if(modeChanged) {
		modeChanged = false;
	}

	// Initially sets all references (X, Y, Z, yaw, and derivatives) to zero
	for(int i = 0; i < POS_REFS; i++)
		XqTraj[i] = 0;

	// Sets X, Y, and Z to sit at the hover point
	XqTraj[  X_t] = XqRef[X];
	XqTraj[  Y_t] = XqRef[Y];
	XqTraj[  Z_t] = XqRef[Z];

	// Extracts the yaw angle from the reference vector
	XqTraj[PSI_t] = quat2Yaw(&XqRef[Q0]);
}



// Executes the trajectory generation for waypoints
void TrajectoryMaker::goToWaypoint(double Xq[], double XqRef[])
{
	// If the mode was just changed to going to the waypoint,
	// reset the flag, record starting point, and set the waypoint time
	if(modeChanged) {
		modeChanged = false;
		
		for(int i = 0; i < STATES; i++)
			XqStart[i] = Xq[i];

		startSet = true;

		// Scales waypoint time based on distance; always gives at least a second
		// regardless of distance, gives a greater-than-linear time increase for
		// short distances (<1), and gives a less-than-linear time increase for
		// longer distances (>1).
		double Xdiff = XqRef[X] - Xq[X];
		double Ydiff = XqRef[Y] - Xq[Y];
		double Zdiff = XqRef[Z] - Xq[Z];
		double dist = sqrt(Xdiff*Xdiff + Ydiff*Ydiff + Zdiff*Zdiff);
		waypointTime = sqrt(2*dist) + 1;
	}

	// Uses a cosine sweep to transition from starting point to the end point
	// (see the function call for a more in depth description of the function profile).
	pathTimer.toc();
	cosineSweep( &XqTraj[  X_t], XqStart[X], XqRef[X], 
		pathTimer.secs(), waypointTime);
	cosineSweep( &XqTraj[  Y_t], XqStart[Y], XqRef[Y], 
		pathTimer.secs(), waypointTime);
	cosineSweep( &XqTraj[  Z_t], XqStart[Z], XqRef[Z], 
		pathTimer.secs(), waypointTime);

	// Uses the same cosine sweep to set the reference yaw angle to go from the
	// initial yaw value to 0 radians.
	cosineSweepHeading( &XqTraj[PSI_t], quat2Yaw(&XqStart[Q0]), quat2Yaw(&XqRef[Q0]),
		pathTimer.secs(), waypointTime);
}



// Calculates the trajectory information for the figure eight
void TrajectoryMaker::doFigureEight()
{
	// If the mode was just changed to hovering, only reset the flag.
	if(modeChanged) {
		modeChanged = false;
	}

	// precalculates some repeated terms
	double mag_X = 0.5*fig8width;
	double mag_Y = 0.5*fig8length;
	double mag_Z = 0.5*fig8height;
	double val_X = 4*pi/fig8period; // sets the X sinusoid to travel twice as fast
	double val_Y = 2*pi/fig8period;
	double val_Z = 4*pi/fig8period; // same for Z sinusoid
	
	pathTimer.toc();
	double t = pathTimer.secs();

	// Oscillatory X trajectory and its derivatives
	XqTraj[    X_t] = mag_X                *  sin(t * val_X);
	XqTraj[   DX_t] = mag_X *     val_X    *  cos(t * val_X);
	XqTraj[  DDX_t] = mag_X * pow(val_X,2) * -sin(t * val_X);
	XqTraj[ DDDX_t] = mag_X * pow(val_X,3) * -cos(t * val_X);
	XqTraj[DDDDX_t] = mag_X * pow(val_X,4) *  sin(t * val_X);

	// Oscillatory Y trajectory and its derivatives
	XqTraj[    Y_t] = mag_Y                *  sin(t * val_Y);
	XqTraj[   DY_t] = mag_Y *     val_Y    *  cos(t * val_Y);
	XqTraj[  DDY_t] = mag_Y * pow(val_Y,2) * -sin(t * val_Y);
	XqTraj[ DDDY_t] = mag_Y * pow(val_Y,3) * -cos(t * val_Y);
	XqTraj[DDDDY_t] = mag_Y * pow(val_Y,4) *  sin(t * val_Y);

	// Oscillatory Z trajectory and its derivatives
	XqTraj[    Z_t] = fig8alt   +    mag_Z *  sin(t * val_Z);
	XqTraj[   DZ_t] = mag_Z *     val_Z    *  cos(t * val_Z);
	XqTraj[  DDZ_t] = mag_Z * pow(val_Z,2) * -sin(t * val_Z);
	XqTraj[ DDDZ_t] = mag_Z * pow(val_Z,3) * -cos(t * val_Z);
	XqTraj[DDDDZ_t] = mag_Z * pow(val_Z,4) *  sin(t * val_Z);

	// Fixes yaw and its derivatives to 0
	for(int i = PSI_t; i < DDPSI_t; i++)
		XqTraj[i] = 0;
}



// Calculates the trajectory information for the special flight pattern.
void TrajectoryMaker::doSpecial()
{
	// If the mode was just changed to hovering, only reset the flag.
	if(modeChanged) {
		modeChanged = false;
	}

	// precalculates some repeated terms
	double val_X = 2*pi/specCircPeriod;
	double val_Y = val_X;
	double val_Z = 2*pi/specAltPeriod;
	double val_PSI = 2*pi/specRotPeriod;
	
	pathTimer.toc();
	double t = pathTimer.secs();

	// Oscillatory X trajectory and its derivatives
	XqTraj[    X_t] = specRadius                *  cos(t * val_X);
	XqTraj[   DX_t] = specRadius *     val_X    * -sin(t * val_X);
	XqTraj[  DDX_t] = specRadius * pow(val_X,2) * -cos(t * val_X);
	XqTraj[ DDDX_t] = specRadius * pow(val_X,3) *  sin(t * val_X);
	XqTraj[DDDDX_t] = specRadius * pow(val_X,4) *  cos(t * val_X);

	// Oscillatory Y trajectory and its derivatives
	XqTraj[    Y_t] = specRadius                *  sin(t * val_Y);
	XqTraj[   DY_t] = specRadius *     val_Y    *  cos(t * val_Y);
	XqTraj[  DDY_t] = specRadius * pow(val_Y,2) * -sin(t * val_Y);
	XqTraj[ DDDY_t] = specRadius * pow(val_Y,3) * -cos(t * val_Y);
	XqTraj[DDDDY_t] = specRadius * pow(val_Y,4) *  sin(t * val_Y);

	// Oscillatory Z trajectory and its derivatives
	XqTraj[    Z_t] = specAlt +      specAltMag *  sin(t * val_Z);
	XqTraj[   DZ_t] = specAltMag *     val_Z    *  cos(t * val_Z);
	XqTraj[  DDZ_t] = specAltMag * pow(val_Z,2) * -sin(t * val_Z);
	XqTraj[ DDDZ_t] = specAltMag * pow(val_Z,3) * -cos(t * val_Z);
	XqTraj[DDDDZ_t] = specAltMag * pow(val_Z,4) *  sin(t * val_Z);

	// Sets yaw such that it spins in a circle
	XqTraj[  PSI_t] = -minAngle(val_PSI*t);
	XqTraj[ DPSI_t] = -val_PSI;
	XqTraj[DDPSI_t] = 0;
}



// Performs waypoint trajectory generation using XqHome as a reference
void TrajectoryMaker::goHome(double Xq[])
{
	goToWaypoint(Xq, XqHome);
}



// Executes the trajectory generation for landing
void TrajectoryMaker::land(double Xq[])
{
	// If the mode was just changed to land, reset the flag and properly set 
	// the landing starting point. The landing ending point is set to directly
	// below the starting point at "groundAlt" height.
	if(modeChanged) {
		modeChanged = false;

		for(int i = 0; i < STATES; i++)
			XqStart[i] = Xq[i];

		startSet = true;
	}

	// Initially sets all references (X, Y, Z, yaw, and derivatives) to zero
	for(int i = 0; i < POS_REFS; i++)
		XqTraj[i] = 0;

	// Sets X and Y to stay above the landing point
	XqTraj[  X_t] = XqStart[X];
	XqTraj[  Y_t] = XqStart[Y];
	
	// Uses a cosine sweep to transition from starting altitude to ending altitude
	// (see the function call for a more in depth description of the altitude profile).
	// "landTime" is set as globals at the top of this file.
	pathTimer.toc();
	cosineSweep( &XqTraj[  Z_t], XqStart[Z], groundAlt, 
		pathTimer.secs(), landTime);

	// Sets the yaw angle to stay constant from the landing state
	//XqTraj[PSI_t] = quat2Yaw(&XqStart[Q0]);

	// Uses the same cosine sweep to set the reference yaw angle to go from the
	// initial yaw value to 0 radians.
	cosineSweepHeading( &XqTraj[PSI_t], quat2Yaw(&XqStart[Q0]), 0,
		pathTimer.secs(), landTime);
}








// Checks if the takeoff process is done
bool TrajectoryMaker::takeoffDone(double Xq[])
{
	// If the quadrotor is above the takeoff altitude (with buffer), 
	// it is deemed to be done with taking off.
	if(Xq[Z] >= takeoffAlt - waypointZBuffer)
		return true;

	// Also, if the current path time exceeds the desired takeoff time,
	// it is deemed to be done taking off.
	pathTimer.toc();
	if(pathTimer.secs() >= takeoffTime)
		return true;

	// If not, it's not done taking off
	return false;
}


// Checks if the quadrotor's within the buffer box for the waypoint by
// checking if the quadrotor's outside of it; if it fails all the tests
// for it being outside, then it must be inside.
bool TrajectoryMaker::atWaypoint(double Xq[], double XqRef[])
{
	// Checks if the path timer's ended
	pathTimer.toc();
	if(pathTimer.secs() >= waypointTime)
		return true;

	bool inBox = true;	

	// Checks if it's outside the box on the X axis
	if ( Xq[X] > XqRef[X] + waypointXYBuffer)
		inBox = false;
	if ( Xq[X] < XqRef[X] - waypointXYBuffer)
		inBox = false;

	// Checks if it's outside the box on the Y axis
	if ( Xq[Y] > XqRef[Y] + waypointXYBuffer)
		inBox = false;
	if ( Xq[Y] < XqRef[Y] - waypointXYBuffer)
		inBox = false;

	// Checks if it's outside the box on the Z axis
	if ( Xq[Z] > XqRef[Z] + waypointZBuffer)
		inBox = false;
	if ( Xq[Z] < XqRef[Z] - waypointZBuffer)
		inBox = false;

	// If it hasn't been caught by the previous tests, it must be inside!
	return inBox;
}


// Checks if the quadrotor's within the buffer box for the Home waypoint
bool TrajectoryMaker::atHome(double Xq[])
{
	return atWaypoint(Xq, XqHome);
}



// Checks if the landing process is done
bool TrajectoryMaker::landingDone(double Xq[])
{
	// If the quadrotor is at/below the ground altitude (with buffer), 
	// it is deemed to be done with landing off.
	if(Xq[Z] <= groundAlt + waypointZBuffer)
		return true;

	// Also, if the current path time exceeds the desired landing time,
	// it is deemed to be done landing.
	pathTimer.toc();
	if(pathTimer.secs() >= landTime)
		return true;

	// If not, it's not done landing.
	return false;
}






// For a given position state (X, Y, or Z) and a set of start and end points,
// a cosine-based path is created to slowly start moving, pick up speed,
// then slow down before reaching the end point.
void TrajectoryMaker::cosineSweep(double valAndDerivs[], double valStart, double valEnd, 
		double t, double t_end)
{
	// pre-calculates values used throughout the function
	double pi_t_end = pi/t_end;
	double valDiff = valEnd - valStart;

	// Position calculation: starts with a half period of a cosine function 
	// (from 0 to t_end) that sweeps from 1 to -1, subtracts it from 1 to
	// make it ramp from 0 to 2, divides it by 2 to go from 0 to 1, multiplies it to
	// have a magnitude equal to the distance between the start and stop points, 
	// and finally adds in the location of the starting point. 
	valAndDerivs[0] = ( 1 - cos(t * pi_t_end) )*0.5*valDiff + valStart;

	// Velocity calculation: the derivative of above
	valAndDerivs[1] = (     pi_t_end    *  sin(t * pi_t_end) )*0.5*valDiff;

	// Acceleration calculation: the derivative of above
	valAndDerivs[2] = ( pow(pi_t_end,2) *  cos(t * pi_t_end) )*0.5*valDiff;

	// Jerk calculation: the derivative of above
	valAndDerivs[3] = ( pow(pi_t_end,3) * -sin(t * pi_t_end) )*0.5*valDiff;

	// Snap calculation: the derivative of above
	valAndDerivs[4] = ( pow(pi_t_end,4) * -cos(t * pi_t_end) )*0.5*valDiff;
}


// For the heading and a set of start and end angles,
// a cosine-based path is created to slowly start moving, pick up speed,
// then slow down before reaching the end angle.
void TrajectoryMaker::cosineSweepHeading(double angAndDerivs[], double angStart, double angEnd, 
		double t, double t_end)
{
	// pre-calculates values used throughout the function
	double pi_t_end = pi/t_end;
	double angDiff = minAngle(angEnd - angStart);

	// Angle calculation: starts with a half period of a cosine function 
	// (from 0 to t_end) that sweeps from 1 to -1, subtracts it from 1 to
	// make it ramp from 0 to 2, divides it by 2 to go from 0 to 1, multiplies it to
	// have a magnitude equal to the distance between the start and stop points, 
	// and finally adds in the location of the starting point. 
	angAndDerivs[0] = ( 1 - cos(t * pi_t_end) )*0.5*angDiff + angStart;
	angAndDerivs[0] = minAngle(angAndDerivs[0]);

	// Velocity calculation: the derivative of above
	angAndDerivs[1] = (     pi_t_end    *  sin(t * pi_t_end) )*0.5*angDiff;

	// Acceleration calculation: the derivative of above
	angAndDerivs[2] = ( pow(pi_t_end,2) *  cos(t * pi_t_end) )*0.5*angDiff;
}
