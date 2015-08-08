// Function Definitions
#include "modes.h"

// Standard Libraries
#include <iostream>
#include <cmath>

// Custom Classes/Functions/Definitions
#include "../defs.h"
#include "../functions/autopilot.h"
#include "../functions/transforms.h"

// Sets standard namespace to avoid writing "std::" everywhere
using namespace std;


// Checks to see if any mode switches need to take place due to mode termination
// conditions being met or unsafe flight conditions.
void modeSwitchChecks(FlightMode &current, FlightMode &previous, 
	double Xq[], double XqRef[], double XqRefGCS[], double XqVicon[], 
	TrajectoryMaker* trajmkr, int &rampUpCount)
{

	// Makes sure the quadrotor is within the safe region of the 
	// Vicon area and is at a reasonable angle. If not, flag as
	// unsafe. If it was unsafe before and is now safe, revert 
	// to the previous mode.
	if(!safe(XqVicon)) {		// function found in this file
		if(current != QUAD_UNSAFE && current != QUAD_OFF) {
			previous = current;
			current = QUAD_UNSAFE;
		}
	}
	else if(current == QUAD_UNSAFE)
		current = previous;


	// If taking off, checks to see if the takeoff logic is done
	if(current == QUAD_TAKEOFF) {
		if( trajmkr->takeoffDone(Xq) )
			current = QUAD_HOVER;
	}

	// If landing, checks to see if the landing logic is done
	if(current == QUAD_LAND) {
		if( trajmkr->landingDone(Xq) )
			current = QUAD_IDLE;
	}


	// If flying to a waypoint, checks to see if the waypoint logic is done
	if(current == QUAD_WAYPOINT) {
		if( trajmkr->atWaypoint(Xq,XqRefGCS) )
			current = QUAD_HOVER;
	}


	// If flying home, checks to see if the home flight logic is done
	if(current == QUAD_HEAD_HOME) {
		if( trajmkr->atHome(Xq) )
			current = QUAD_HOVER;
	}


	// If ramping up, checks to see if it should transition to the 
	// takeoff control law. If not ramping up, reset the ramp up counter.
	//static int rampUpCount = 0;
	if(current == QUAD_RAMPUP) {
		if(rampUpCount >= ramp_up_max_count) // ramp_up_max_count defined in "defs.cpp"
			current = QUAD_TAKEOFF;
	}	
	else
		rampUpCount = 0;

	return;
}



// Checks to see if it's safe to move the current FlightMode to the desired FlightMode.
// If so, it makes the change. If not, it preserves the current mode. For special cases,
// the flight mode may be changed to something else.
//
// Apologies in advance: While "switch" statements make implementing this logic
// computationally efficient and tend to help spell out which modes various logic
// applies to, it's hard to read without knowing what it's supposed to do.
// See the "Mode Transition Limitation Rules" Excel document for a diagram of
// what's going on here.
enum FlightMode desiredModeSafetyCheck(FlightMode current, FlightMode desired,
	TrajectoryMaker* trajmkr, double Xq[], double XqRefGCS[])
{
	// If either mode involves exiting, simply return an exiting mode.
	if((current == QUAD_EXITING) || (desired == QUAD_EXITING))
		return QUAD_EXITING;


	// Performs special checks related to Waypoint/Head Home commands
	// where the quadrotor may already be there and have just been
	// set to Hover
	if(current == QUAD_HOVER) {
		if(desired == QUAD_WAYPOINT  && trajmkr->atWaypoint(Xq, XqRefGCS) )
			return current;
		if(desired == QUAD_HEAD_HOME && trajmkr->atHome(Xq) )
			return current;
	}


	// Performs appropriate function calls based on the desired flight mode
	switch(desired)
	{
		case QUAD_OFF: 
			return desired; 		// turn off the motors at any time.
		case QUAD_IDLE:
			switch(current)
			{
				case QUAD_OFF:		// if testing, off, idle, or ramping up,
				case QUAD_IDLE:  	// it's okay to idle.
				case QUAD_RAMPUP:
				case QUAD_CYCLE: 
				case QUAD_DMC: 
				case QUAD_ATT_CTRL: 
				case QUAD_HEIGHT_CTRL:
				case QUAD_POS_CTRL:
					return desired;	
				default:			// otherwise, it's not, so don't switch.
					return current; 
			}
		case QUAD_RAMPUP:
			// motors must be idling to go into ramp up
			if(current == QUAD_IDLE)	return desired; 
			// otherwise, keep doing what you were doing (including possibly ramping up)
			else						return current;  
		case QUAD_TAKEOFF:
			// if ramping up or idling, go into ramp up.
			if(current == QUAD_IDLE || current == QUAD_RAMPUP) 	
				return QUAD_RAMPUP; 
			// otherwise, do what you were doing (including possibly taking off)
			else												
				return current; 
		// QUAD_LAND's logic purposefully spills over into that of QUAD_UNSAFE 
		// and the rest below, so DON'T INSERT BREAK/ELSE STATEMENTS.
		case QUAD_LAND:	
			// if ramping up and trying to land, go to Idle instead.
			if(current == QUAD_RAMPUP)	
				return QUAD_IDLE;
			// if taking off, go to Land 
			if(current == QUAD_TAKEOFF) 
				return desired; 
		// QUAD_UNSAFE also purposefully spills over.
		case QUAD_UNSAFE: 
			// if landing or unsafe, do what you want to do.
			if(current == QUAD_LAND || current == QUAD_UNSAFE)
				return desired; 
		// Handles a bunch of flight modes (and spilling over unsafe/landing
		// procedures) at once
		case QUAD_HOVER:
		case QUAD_WAYPOINT:
		case QUAD_VELOCITY:
		case QUAD_FIGURE_8:
		case QUAD_SPECIAL:
		case QUAD_HEAD_HOME:
		case QUAD_FOLLOW_REF:		
			switch(current)
			{
				case QUAD_HOVER:	// if flying and desired = fly/land/unsafe,
				case QUAD_WAYPOINT:	// go for desired value
				case QUAD_VELOCITY:
				case QUAD_FIGURE_8:
				case QUAD_SPECIAL:
				case QUAD_FOLLOW_REF:
				case QUAD_HEAD_HOME:
					return desired; 
				default:
					return current; // otherwise, keep doing what it was doing
			}
		case QUAD_LATENCY_TEST:
			// only jump to Latency Testing if everything's off or it's
			// already doing Latency Testing.
			if(current == QUAD_OFF || current == QUAD_LATENCY_TEST)	
				return desired; 
			else	
				return current;
		// Handles the logic for several testing modes at once
		case QUAD_CYCLE:
		case QUAD_DMC:
		case QUAD_ATT_CTRL:
		case QUAD_HEIGHT_CTRL:
		case QUAD_POS_CTRL:
			switch(current)
			{
				case QUAD_IDLE:	// if testing/idling and want to test, go for it
				case QUAD_CYCLE:
				case QUAD_DMC:
				case QUAD_ATT_CTRL:
				case QUAD_HEIGHT_CTRL:
				case QUAD_POS_CTRL:
					return desired; 
				default:		// otherwise, keep on keeping on
					return current; 
			}
		// Otherwise, do 
		case QUAD_UNDEFINED:
		case QUAD_EXITING:	// handled above, but included here to remove
		default:			// compiler warning flags
			return current;
	}
	
	// If the above logic fails (which it never should), the program will 
	// return an undefined state. This is mostly here for compiler friendliness,
	// but it's also a catch-all way to know if there's bogus logic above.
	return QUAD_UNDEFINED;
}



// Chooses the reference states for each flight mode. Testing/Off modes
// get default references, while flight modes get more detailed references.
// The TrajectoryMaker has some utility functions for keeping track of the
// quadrotor's Home position, which is initially set by the first received
// Vicon measurement. These reference values potentially get used by the 
// TrajectoryMaker in "modeTrajectory", depending on the flight mode.
void modeReference(FlightMode currentMode, double Xq[], double XqRef[], 
	double XqRefGCS[], TrajectoryMaker* trajmkr)
{
	// NOTICE: If you've gotten this far and don't understand quaternions,
	// you should change that. Here's a good reference:
	// http://math.ucr.edu/~huerta/introquaternions.pdf

	// Keeps track of the hover point
	static bool hoverSet = false;
	static double XqHover[STATES] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	static enum FlightMode modeBeforeHover = QUAD_OFF;

	if(currentMode == QUAD_HOVER) {
		if(!hoverSet) {
			hoverSet = true;

			// If it was heading to a waypoint, sets the
			// hover point to the value stored in XqRef (which was
			// previously set to be the desired waypoint)
			if(modeBeforeHover == QUAD_WAYPOINT) {
				XqHover[ X] = XqRef[ X];
				XqHover[ Y] = XqRef[ Y];
				XqHover[ Z] = XqRef[ Z];
				XqHover[Q0] = XqRef[Q0];
				XqHover[QI] = XqRef[QI];
				XqHover[QJ] = XqRef[QJ];
				XqHover[QK] = XqRef[QK];
			}
			// If it was heading home, grab the states for the
			// Home location.
			else if(modeBeforeHover == QUAD_HEAD_HOME) {
				XqHover[ X] = trajmkr->getHomeState( X);
				XqHover[ Y] = trajmkr->getHomeState( Y);
				XqHover[ Z] = trajmkr->getHomeState( Z);
				XqHover[Q0] = trajmkr->getHomeState(Q0);
				XqHover[QI] = trajmkr->getHomeState(QI);
				XqHover[QJ] = trajmkr->getHomeState(QJ);
				XqHover[QK] = trajmkr->getHomeState(QK);
			}
			// Otherwise, hover precisely where it was when it was told to hover
			else {
				XqHover[ X] = Xq[ X];
				XqHover[ Y] = Xq[ Y];
				XqHover[ Z] = Xq[ Z];
				XqHover[Q0] = Xq[Q0];
				XqHover[QI] = Xq[QI];
				XqHover[QJ] = Xq[QJ];
				XqHover[QK] = Xq[QK];
			}
		}
	}
	else {
		hoverSet = false;
		modeBeforeHover = currentMode;
	}
	


	// Selects the appropriate reference for the current mode
	switch(currentMode)
	{
		// For Testing/Non-flying test modes, set everything to zero
		// (except the quaternion's q0 value)
		case QUAD_OFF:
		case QUAD_IDLE:
		case QUAD_RAMPUP:
		case QUAD_EXITING:
		case QUAD_CYCLE:
		case QUAD_LATENCY_TEST:
		case QUAD_DMC:
		case QUAD_UNDEFINED:
		case QUAD_ATT_CTRL:
			for(int i = 0; i < STATES; i++)
				XqRef[i] = 0;
			XqRef[Q0] = 1;
			break;
		// For the Position Control test, set it to try to maintain
		// the Home X/Y position; all other values -> 0 (except quaternion q0)
		// Same for landing
		case QUAD_POS_CTRL:
		case QUAD_LAND:
			for(int i = 0; i < STATES; i++)
				XqRef[i] = 0;
			XqRef[Q0] = 1;
			XqRef[X] = trajmkr->getHomeState(X);
			XqRef[Y] = trajmkr->getHomeState(Y);
			break;
		// For Taking off/Heading Home/Height Control, set it to
		// try to maintain the Home X/Y position and a set altitude;
		// all other values -> 0 (except quaternion q0)
		case QUAD_TAKEOFF:
		case QUAD_HEAD_HOME:
		case QUAD_HEIGHT_CTRL:
			for(int i = 0; i < STATES; i++)
				XqRef[i] = 0;
			XqRef[Q0] = 1;
			XqRef[X] = trajmkr->getHomeState(X);
			XqRef[Y] = trajmkr->getHomeState(Y);
			XqRef[Z] = 0.75;
			break; 
		case QUAD_HOVER:
			for(int i = 0; i < STATES; i++)
				XqRef[i] = XqHover[i];
			break;
		// For special flight paths, just set to zero
		case QUAD_FIGURE_8:	
		case QUAD_SPECIAL:
			for(int i = 0; i < STATES; i++)
				XqRef[i] = 0;
			XqRef[Q0] = 1;
			break;
		// TODO: implement appropriate logic on the GCS, then have it
		// update this array accordingly; see "Quadrotor::updateFromThreads()"
		case QUAD_WAYPOINT: 
			for(int i = 0; i < STATES; i++)
				XqRef[i] = XqRefGCS[i]; 
			break;
		// TODO: implement their logic later
		case QUAD_VELOCITY:
		case QUAD_FOLLOW_REF:
		case QUAD_UNSAFE:
			break;
	}

	return;
}



// Calculates the appropriate trajectory for each flight mode. This is only
// applicable for flight modes that make use of the nonlinear controller, as
// other control schemes do not use the same type of trajectory. See
// "../functions/TrajectoryMaker.cpp" for the details of how trajectories
// are generated for each flight mode.
//
// Format for the trajectory vector (Psi = yaw angle):
// [ X, dX, ddX, dddX, ddddX, Y, dY, ddY, dddY, ddddY, ...
//   Z, dZ, ddZ, dddZ, ddddZ, Psi, dPsi, ddPsi ]
void modeTrajectory(FlightMode currentMode, double Xq[], double XqRef[], TrajectoryMaker* trajmkr)
{
	// Updates the TrajectoryMaker with the latest flight mode and reference
	trajmkr->checkForChanges(currentMode, XqRef);

	// Selects the appropriate trajectory generation methods
	switch(currentMode)
	{
		// Non-Flying/(most) Test modes: do nothing
		case QUAD_OFF:
		case QUAD_IDLE:
		case QUAD_RAMPUP:
		case QUAD_EXITING:
		case QUAD_CYCLE:
		case QUAD_LATENCY_TEST:
		case QUAD_DMC:
		case QUAD_ATT_CTRL:
		case QUAD_POS_CTRL:
		case QUAD_UNDEFINED:
			break;
		// Takeoff: sweep the commanded height to the desired altitude
		// in a sinusoidal shape while maintaining X/Y/Psi
		case QUAD_TAKEOFF:
			trajmkr->takeoff(Xq);
			break;
		// Height Control/Hover: maintain constant X/Y/Z/Psi
		case QUAD_HOVER:
		case QUAD_HEIGHT_CTRL:
			trajmkr->hover(XqRef);
			break;
		// Waypoint: Sinusoidally sweeps the trajectory over to
		// the desired waypoint
		case QUAD_WAYPOINT: 
			trajmkr->goToWaypoint(Xq, XqRef);
			break;
		// Figure Eight: maintain zero Psi while sinusoidally
		// varying X, Y, and Z such that it goes in a figure 8.
		case QUAD_FIGURE_8:
			trajmkr->doFigureEight();
			break;
		// Special: fly in a circular-type pattern
		case QUAD_SPECIAL:
			trajmkr->doSpecial();
			break;
		// Head Home: like Waypoint, except specifically to Home
		case QUAD_HEAD_HOME:
			trajmkr->goHome(Xq);
			break;
		// Landing: Like takeoff, but in reverse
		case QUAD_LAND:
			trajmkr->land(Xq);
			break;
		// TODO: implement these later
		case QUAD_VELOCITY:
		case QUAD_FOLLOW_REF: 
		case QUAD_UNSAFE:
			break;
	}
}



// Calculates control laws associated with each flight mode. Most proper flight modes
// will make use of the nonlinear feedback linearization controller, but some test
// modes will use PD controllers used during prototyping. 
void modeControlLaw(FlightMode currentMode, double inputs[], Controller* ctrl)
{
	// Selects the appropriate control law for each mode
	switch(currentMode)
	{	
		// Non-Flying Modes: do nothing
		case QUAD_OFF:	
		case QUAD_IDLE:
		case QUAD_RAMPUP:
		case QUAD_EXITING:
		case QUAD_CYCLE:
		case QUAD_LATENCY_TEST:
		case QUAD_DMC:
		case QUAD_UNDEFINED:
			break;
		// Test PD control law for maintaining a desired attitude
		case QUAD_ATT_CTRL:		
			ctrl->defaultThrust();	
			ctrl->calcRefError();					
			ctrl->attitudeControlPD();
			ctrl->compCGoffset();
			break;
		// Test PD control law for maintaining a desired XY position
		case QUAD_POS_CTRL:		
			ctrl->defaultThrust();
			ctrl->calcRefErrorNoIntegral();
			// generates reference Ox/Oy Euler angles for the attitude controller
			ctrl->positionControlPID();
			// recalculates errors based on updated reference angles
			ctrl->calcRefError(); 
			ctrl->attitudeControlPD();
			ctrl->compCGoffset();
			break;
		// Test control law for maintaining a desired height;
		// uses prototyped PD controllers for altitude, position, and attitude.
		case QUAD_HEIGHT_CTRL:	
			ctrl->calcRefErrorNoIntegral();
			ctrl->altitudeControlPID();
			ctrl->positionControlPID();
			ctrl->calcRefError();
			ctrl->attitudeControlPD();
			ctrl->compCGoffset();
			//ctrl->feedbackLinController();
			//ctrl->compCGoffset();
			break;
		case QUAD_TAKEOFF:
		case QUAD_HOVER:
		case QUAD_WAYPOINT: 
		case QUAD_FIGURE_8:
		case QUAD_SPECIAL:
		case QUAD_HEAD_HOME:
		case QUAD_LAND:
			ctrl->feedbackLinController();
			//ctrl->compCGoffset();
			break;
		// TODO: implement these later		
		case QUAD_VELOCITY:
		case QUAD_FOLLOW_REF:
		case QUAD_UNSAFE: 		// eventually the same as Waypoint, possibly
			for(int i = 0; i < 4; i++)
				inputs[i] = 0;
			break;
		default:
			break;
	}

	return;
}



// Sets the motor speeds appropriately based on the given flight mode.
// Some modes require the motors to always be off (Off, Exiting, Latency Testing,
// and Undefined), others require a set speed (Idle, DMC Test), others have
// their own logic (Ramp Up, Cycle) and the rest do their best to execute
// the inputs set by the control laws (Thrust/Torques -> Motor rad/s -> DMC values).
void modeSetMotors(FlightMode currentMode, unsigned char newDMCs[], 
		int &rampCount, Controller* ctrl)
{
	// Selects the appropriate means of setting motor speeds
	switch(currentMode)
	{
		// Off Modes: turn motors off
		case QUAD_OFF:
		case QUAD_EXITING:
		case QUAD_LATENCY_TEST:
		case QUAD_UNDEFINED:
			for(int i = 0; i < 4; i++)
				ctrl->motorDMCs[i] = 0;
			break;
		// Idle: set motors to slowest speed
		case QUAD_IDLE:
			for(int i = 0; i < 4; i++) {
				ctrl->omegas[i] = omega_idle;
			}
			ctrl->generateDMCs();
			break;
		// Ramp Up: use Ramp Up logic to set the speeds
		case QUAD_RAMPUP:
			rampMotors(ctrl->omegas, rampCount);	// found in this file
			ctrl->generateDMCs();			
			break;
		// Cycle: use Cycle Motors logic to set the speeds
		case QUAD_CYCLE:
			cycleMotors(ctrl->omegas, false);		// found in this file
			ctrl->generateDMCs();
			break;
		// DMC: set DMCs from values given by GCS
		case QUAD_DMC:
			for(int i = 0; i < 4; i++)
				ctrl->motorDMCs[i] = newDMCs[i];
			break;
		// Flight/Flight-Testing modes: set speeds/DMCs from inputs
		case QUAD_TAKEOFF:
		case QUAD_HOVER: 
		case QUAD_WAYPOINT: 
		case QUAD_VELOCITY: 
		case QUAD_FIGURE_8:
		case QUAD_SPECIAL:
		case QUAD_FOLLOW_REF:
		case QUAD_HEAD_HOME:
		case QUAD_LAND:	
		case QUAD_UNSAFE: 
		case QUAD_ATT_CTRL:	
		case QUAD_HEIGHT_CTRL:
		case QUAD_POS_CTRL:	
			ctrl->generateMotorSpeeds();
			ctrl->generateDMCs();
			break;
	}

	return;
}



// Checks the raw position/attitude received from Vicon to verify the quadrotor
// is 1) within the safe operating area of the Vicon capture area and 2)
// at a reasonable attitude. As quaternions aren't very intuitive for knowing
// what's safe, the quaternion gets converted to Euler angles before being compared
// with pitch/roll angles past which the quadrotor is deemed unsafe. 
//
// The various kill parameters (Xkill, Ykill, etc.) are all set in "../defs.cpp".
bool safe(double viconStates[])
{
	// Converts the quaternion to Euler angles
	double euler[3];
	quat2EulerZYX(&viconStates[Q0v], euler);

	// Position Check
	if(abs(viconStates[Xv]) > Xkill) 
	{
		cout << "\n\r SAFETY CHECK FAILSAFE!!! X AXIS\n\r";
		return false;
	}
	else if(abs(viconStates[Yv]) > Ykill)
	{
		cout << "\n\r SAFETY CHECK FAILSAFE!!! Y AXIS\n\r";
		return false;		
	}
	else if(abs(viconStates[Zv]) > Zkill)
	{
		cout << "\n\r SAFETY CHECK FAILSAFE!!! Z AXIS\n\r";
		return false;		
	}
	// Attitude Check
	else if(abs(euler[0]) > OXkill)
	{
		cout << "\n\r SAFETY CHECK FAILSAFE!!! Ox ANGLE\n\r";
		return false;		
	}
	else if(abs(euler[1]) > OYkill)
	{
		cout << "\n\r SAFETY CHECK FAILSAFE!!! Oy ANGLE\n\r";
		return false;		
	}

	// If the quadrotor passed all the previous tests, it's safe!
	return true;
}



// Progressively, linearly increases the speeds for all motors from the idle speed
// to a pre-determined ramp termination speed before Takeoff. While the quadrotor
// could probably handle jumping straight to Takeoff, this 1) gives the motors a chance
// to speed up and 2) gives people a warning that it's about to fly.
// The idle speed (omega_idle), ramp termination speed (omega_ramp), 
// and the number of iterations for the ramp-up process (ramp_up_max_count) 
// are all set in "../defs.cpp".
void rampMotors(double Omegas[], int &rampCount)
{
	rampCount++;
	
	// the "1.0*rampCount" ensures a proper floation point division is performed
	double speed = omega_idle + (omega_ramp - omega_idle)*((1.0*rampCount)/ramp_up_max_count);
	
	Omegas[0] = speed;
	Omegas[1] = speed;
	Omegas[2] = speed;
	Omegas[3] = speed;

	return;
}



// Progressively steps through ramping up each motor in series from a hard-coded
// minimum speed to a hard-coded maximum speed, then cycles down the motors in the
// same order. This test verifies 1) the quadrotor's motors respond to speed commands,
// 2) the motors are being set in order, and 3) things aren't locking up/stuttering.
// Increases and decreases should be smooth. 
//
// For the quadrotor's specified motor labeling convention, the increasing/decreasing
// pattern should go in a counter-clockwise pattern.
void cycleMotors(double Omegas[], bool reset)
{
	// Speed limits for cycle test
	const int maxSpeed = 450; 	// rad/s
	const int minSpeed = 125; 	// rad/s
	
	// Static variables for tracking the cycling process
	static bool ascending = true;
	static int currentMotor = 0;
	static double cycledSpeed[4] = {minSpeed, minSpeed, minSpeed, minSpeed};
	
	// Resets the values back to zero if desired
	if(reset)
	{
		ascending = true;
		currentMotor = 0;
		for(int i = 0; i < 4; i++)
			cycledSpeed[i] = minSpeed;
	}
	
	// If in the ascending phase, speed up the motors
	if(ascending)
	{
		// Ramps up motors at 100 rad/s^2
		cycledSpeed[currentMotor] += (100*Ts);	
		
		// If the max speed has been met,
		if(cycledSpeed[currentMotor] >= maxSpeed)
		{
			// move to the next motor;
			cycledSpeed[currentMotor] = maxSpeed;
			currentMotor++;
			
			// If it's gone past the last motor, start at the first
			// motor and switch to the descending phase.
			if(currentMotor > 3)
			{
				currentMotor = 0;
				ascending = false;
			}
		}
	}
	// Otherwise, it's in the descending phase, so slow down the motors
	else
	{
		// Ramps down motors at -100 rad/s^2
		cycledSpeed[currentMotor] -= (100*Ts);
		
		// If the minimum speed has been met,
		if(cycledSpeed[currentMotor] <= minSpeed)
		{
			// move to the next motor
			cycledSpeed[currentMotor] = minSpeed;
			currentMotor++;
			
			// If it's gone past the last motor, start at the first
			// motor and switch to the ascending phase.
			if(currentMotor > 3)
			{
				currentMotor = 0;
				ascending = true;
			}
		}	
	}
	
	// Set all the motor speeds to the cycling speeds
	for(int i = 0; i < 4; i++)
		Omegas[i] = cycledSpeed[i];

	return;
}
