#ifndef FEEDBACK_LIN_H
#define FEEDBACK_LIN_H

#include "../defs.h"
#include "../quad/parameters.h"


// This set of calculations is quite dense. For an idea of what's even going on here,
// read these two papers first:
//	- O. Fritsch, P. De Monte, M. Buhl, and B. Lohmann, 
//		"Quasi-static feedback linearization for the translational dynamics 
//		 of a quadrotor helicopter"
// 	- M. Buhl and B. Lohmann, 
//		"Control with exponentially decaying Lyapunov functions and its use 
//		 for systems with input saturation"



// A definition variable for adding in integral control.
// Comment it out to disable the integral term. 
#define DO_INTEGRAL

// A definition variable for putting out debug text.
// Comment it out to disable debug messages.
//#define DEBUG

// Pre-declares the gains for the feedback linearization "inputs"
#ifdef DO_INTEGRAL
						 //    int     v    dv   ddv  dddv
	const int gains_xy[]  = { 1024, 1280,  640,  160,   20};
	const int gains_z[]   = {   27,   27,    9};
	const int gains_psi[] = {   27,   27,    9};
#else
						 //            v    dv   ddv  dddv
	const int gains_xy[]  = {    0,  256,  256,   96,   16};
	const int gains_z[]   = {    0,    9,    6};
	const int gains_psi[] = {    0,    9,    6};
#endif


// Struct containing all the relevant variables that need calculating
struct FeedbackLinStruct {
	// General values
	double Ts;
	double g;
	double Mq;
	double Jqx;
	double Jqy;
	double Jqz;
	
	
	// Split Quaternion values
	double   qw,   qz;				// Z quaternion values
	double   qp,   qx,   qy;		// XY quaternion values
	double  dqp,  dqx,  dqy; 		// XY quaternion velocities
	double       ddqx, ddqy; 		// XY quaternion accelerations
	
	// Auxiliary angular velocity values
	double  wxa,  wya,  wza;		// auxiliary angular velocities
	double dwxa, dwya, dwza;		// auxiliary angular accelerations
	double dwx,  dwy,  dwz;         // body frame angular accelerations
	
	// Thrust-related values
	double gamma; 					// Z component of thrust
	double gamma_inv;				// inverse of gamma (to save a division)
	double T, dT, ddT;				// thrust and its derivatives
	
	// State derivative values
	double XqPos[POS_REFS]; 		// holds calculations for X,Y,Z and their derivatives
									// all the way to their fourth derivatives, as well as
									// heading angle down to second derivative
	double XqPosError[POS_REFS];    // holds error calculations
	double XqPosErrorInt[POS_REFS]; // holds error integral calculations
};

void initializeStruct(struct FeedbackLinStruct &f, int quadNum);

void splitQuaternion(double Xq[], struct FeedbackLinStruct &f);

void getAuxAngVels(double Xq[], struct FeedbackLinStruct &f);

void getSplitQuatDerivs(struct FeedbackLinStruct &f);

void getSecondDerivs(double Xq[], double XqTraj[],  struct FeedbackLinStruct &f);

void getThirdDerivs(double Xq[], double XqTraj[], struct FeedbackLinStruct &f);

void getQuatAccels(double Xq[], double XqTraj[],  struct FeedbackLinStruct &f);
		
void getWauxDots(struct FeedbackLinStruct &f);

void getWxyDots(double Xq[], struct FeedbackLinStruct &f);

void headingControl(double XqTraj[], struct FeedbackLinStruct &f);

void getTrueInputs(double Xq[], double Uq[], struct FeedbackLinStruct f);
		
#endif
