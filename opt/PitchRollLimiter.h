#ifndef PITCH_ROLL_LIMITER_H
#define PITCH_ROLL_LIMITER_H

// Must install GLPK before this will work
#include <glpk.h>

// Needed to get the number of iterations
#include "GLPK_Headers/prob.h"

class PitchRollLimiter {
  protected:
	// Optimization Problem object
	glp_prob *lp;
	
	// Keeps track of whether or not it's in standard form
	bool is_std_form;
	
	// Coefficient values
	double Bp;
	double Kp;
	double Lq;
	
	// Simplex solver values
	bool useSimplex;
	glp_smcp parm_sim;	// simplex settings
	int max_result_sim; 	// holds result of simplex optimization
	int min_result_sim;
	
	// Interior Point solver values
	bool useIntPnt;
	glp_iptcp parm_int; 	// interior point settings
	int max_result_int;	// holds results of int. pnt. optimization
	int min_result_int;
	

	// Results values
	double 	yawMax;
	double 	yawMin;
	bool 	validMax;
	bool 	validMin;
	int 	maxIters;
	int 	minIters;
  public:
	PitchRollLimiter();
	~PitchRollLimiter();
	void makeModel(double Bp, double Kp, double Lq);
	void makeModelStd(double Bp, double Kp, double Lq);	// mostly just used for testing purposes
	void inputUpdate(double Uq[]);

	void useSimplexPrimal();	// works the fastest
	void useSimplexDual();
	void useIntPntNatural();
	void useIntPntQuotient();
	void useIntPntApprox();
	void useIntPntSYMAMD();

	void 	findMax();
	void 	findMin();
	bool 	isValidMax();
	bool 	isValidMin();
	double 	getMax();
	double 	getMin();
	int 	getMaxIters();
	int 	getMinIters();
	int 	getMaxStatus();
	int	getMinStatus();
};

#endif
