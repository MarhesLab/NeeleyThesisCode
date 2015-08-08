#ifndef MODES_H
#define MODES_H

// Custom Classes/Functions/Definitions
#include "../defs.h"
#include "../functions/Controller.h"
#include "../functions/TrajectoryMaker.h"

// Function Definitions
// See "modes.cpp" for in-depth explanations of what each function does.

void modeSwitchChecks(FlightMode &current, FlightMode &previous, 
	double Xq[], double XqRef[], double XqRefGCS[], double XqVicon[], 
	TrajectoryMaker* trajmkr, int &rampUpCount);

enum FlightMode desiredModeSafetyCheck(FlightMode current, FlightMode desired,
	TrajectoryMaker* trajmkr, double Xq[], double XqRefGCS[]);

void modeReference(FlightMode currentMode, double Xq[], double XqRef[], 
	double XqRefGCS[], TrajectoryMaker* trajmkr);

void modeTrajectory(FlightMode currentMode, double Xq[], double XqRef[],
	TrajectoryMaker* trajmkr);

void modeControlLaw(FlightMode currentMode, double inputs[], Controller* ctrl);

void modeSetMotors(FlightMode currentMode, unsigned char newDMCs[], 
		int &rampCount, Controller* ctrl);

bool safe(double viconStates[]);

void rampMotors(double Omegas[], int &rampCount);

void cycleMotors(double Omegas[], bool reset);

#endif
