#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../defs.h"
#include "feedbackLin.h"
#include "transforms.h"
#include "../opt/YawOptimizer.h"
#include "../quad/parameters.h"

class Controller {
  protected:
	// Model-related parameters
	double Mq;
	double dCGx, dCGy;
	double Lq, Bp, Kp;

	// State-related parameters
	double* Xq;
	double* XqTraj;
	double* XqRef;
	double XqTrajErr[POS_REFS];
	double XqTrajErrInt[POS_REFS];
	double XqRefErr[STATES];
	double XqRefErrInt[STATES];

	// Input-related parameters
	double* Uq_cmd;
	double* Uq_act;

	// Safety/optimization-related parameters
	YawOptimizer yawOpt;

	// Feedback linearization-related parameters
	struct FeedbackLinStruct fstruct;
	
  public:
	// Making the motor-related values public so various modes can alter them
	double *omegas;
	unsigned char* motorDMCs;
  
	// Constructor and Destructor
	Controller(double *Xq_in, double *XqTraj_in, double *XqRef_in, 
		double *Uq_cmd_in, double *Uq_act_in, double* omegas_in, 
		unsigned char* motorDMCs_in, int quad_index);
	~Controller();
	void calcRefError();
	void calcRefErrorNoIntegral();

	// Input result checking
	bool inputIsValid();
	void limitInputs();
	void limitMotors();
	
	// Output generation
	void compCGoffset();
	void generateMotorSpeeds();
	void generateDMCs();

	// Default output generations
	void defaultThrust();
	void defaultAngleRef();

	// PID control laws
	void attitudeControlPD();
	void altitudeControlPID();
	void positionControlPID();

	// Feedback Linearization Control Law
	void feedbackLinController();
};


#endif
