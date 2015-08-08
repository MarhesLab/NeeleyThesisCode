#ifndef TRAJECTORY_MAKER_H
#define TRAJECTORY_MAKER_H

#include "../defs.h"
#include "Timer.h"

class TrajectoryMaker {
  protected:
	// Flight mode variables
	bool modeChanged;
	FlightMode currentMode;
	FlightMode previousMode;

	// Reference variables
	bool groundSet;
	double groundAlt;
	bool homeSet;
	double XqHome[STATES];
	bool startSet;
	double XqStart[STATES];
	bool refChanged;
	double XqRefCurr[STATES];
	double XqRefPrev[STATES];

	// Trajectory variables
	double *XqTraj;

	// Timing values
	Timer pathTimer;
	double waypointTime;

  public:
	// Constructor/Destructor
	TrajectoryMaker(double *XqTrajp);
	~TrajectoryMaker(); 

	// Functions related to setting ground altitude
	bool groundIsSet();
	void setGroundAlt(double height);
	bool nearGround(double Xq[]);

	// Functions related to getting/setting Home
	bool homeIsSet();
	void setHome(double XqVicon[]);
	double getHomeState(int index);

	// Updates the current flight mode/reference, and implements any 
	// appropriate logic for various changes
	void checkForChanges(FlightMode newMode, double XqRef[]);

	// Flight Commands
	void takeoff(double Xq[]);
	void hover(double XqRef[]);	
	void goToWaypoint(double Xq[], double XqRef[]);
	void doFigureEight();
	void doSpecial();
	void goHome(double Xq[]);
	void land(double Xq[]);

	// Checks for flight pattern start/stop conditions
	bool takeoffDone(double Xq[]);
	bool atWaypoint(double Xq[], double XqRef[]);
	bool atHome(double Xq[]);
	bool readyForFigureEight();			// TODO: implement this
	bool landingDone(double Xq[]);
	
	// Functions for generating smooth trajectories between two points
	void cosineSweep(double valAndDerivs[], double valStart, double valEnd, 
		double time, double endtime);
	void cosineSweepHeading(double valAndDerivs[], double valStart, double valEnd, 
		double t, double t_end);
};

#endif
