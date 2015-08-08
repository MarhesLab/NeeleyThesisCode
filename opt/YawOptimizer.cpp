#include "YawOptimizer.h"

#include <iostream>
#include <stdio.h>

#include "../defs.h"

using namespace std;

YawOptimizer::YawOptimizer()
{
	lp = NULL;	// initialize problem with null pointer

	glp_init_smcp(&parm_sim);		// set up simplex settings
	parm_sim.msg_lev = GLP_MSG_OFF; 	// turns off messages

	glp_init_iptcp(&parm_int);		// set up int.pnt. settings
	parm_int.msg_lev = GLP_MSG_OFF;		// turns off messages

	useSimplex = false;
	useIntPnt = false;

	yawMax = 0;
	yawMin = 0;
	validMax = false;
	validMin = false;
}


YawOptimizer::~YawOptimizer()
{
	glp_delete_prob(lp);
}


void YawOptimizer::makeModel(double Bp, double Kp, double Lq)
{
	// If a problem has already been set up, delete it
	if(lp != NULL)
		glp_delete_prob(lp);	

	// sets up equality coefficients
	// T  = [  Bp  Bp  Bp  Bp ]*W
	// tx = [ 0 Lq*Bp 0 -Lq*Bp]*W
	// ty = [ -Lq*Bp 0 Lq*Bp 0]*W

	int ia[1+12] = {0, 
		1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3};
	int ja[1+12] = {0, 
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4};
	double ar[1+12] = {0, 
		Bp, Bp, Bp, Bp,
		0, Lq*Bp, 0, -Lq*Bp,
		-Lq*Bp, 0, Lq*Bp, 0};

	// optimization parameters
	lp = glp_create_prob();
	glp_set_prob_name(lp, "yawtorque");
	
	// adds constraint parameters
	glp_add_rows(lp, 3);
	glp_set_row_name(lp, 1, "thrust");
	glp_set_row_name(lp, 2, "torque_x");
	glp_set_row_name(lp, 3, "torque_y");
	
	// sets the equality constraints
	glp_set_row_bnds(lp, 1, GLP_FX, 0, 0);			
	glp_set_row_bnds(lp, 2, GLP_FX, 0, 0); 
	glp_set_row_bnds(lp, 3, GLP_FX, 0, 0);

	// adds the variables into the mix
	glp_add_cols(lp, 4); 
	glp_set_col_name(lp, 1, "wsq1");
	glp_set_col_name(lp, 2, "wsq2");
	glp_set_col_name(lp, 3, "wsq3");
	glp_set_col_name(lp, 4, "wsq4");
	
	// sets lower and upper bounds for squared motor speed
	glp_set_col_bnds(lp, 1, GLP_DB, omega_sq_min, omega_sq_max);
	glp_set_col_bnds(lp, 2, GLP_DB, omega_sq_min, omega_sq_max);
	glp_set_col_bnds(lp, 3, GLP_DB, omega_sq_min, omega_sq_max);
	glp_set_col_bnds(lp, 4, GLP_DB, omega_sq_min, omega_sq_max);
	
	// sets the objective function values
	// [  Kp -Kp  Kp -Kp]
	glp_set_obj_coef(lp, 1,  Kp);
	glp_set_obj_coef(lp, 2, -Kp);
	glp_set_obj_coef(lp, 3,  Kp);
	glp_set_obj_coef(lp, 4, -Kp);
				
	glp_load_matrix(lp, 12, ia, ja, ar);
}


void YawOptimizer::makeModelStd(double Bp, double Kp, double Lq)
{
	// If a problem has already been set up, delete it
	if(lp != NULL)
		glp_delete_prob(lp);	

	const int vars = 12; // the number of free, adaptable variables
	const int cons = 11; // the number of equality constraints

	// row indices
	int ia[1+vars*cons] = {0, 
		 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 
		 3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 
		 4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 
		 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 
		 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 
		 7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 
		 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 
		 9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
		11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, };
				
	// column indices
	int ja[1+vars*cons] = {0, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12};
				 
	// values
	double ar[1+vars*cons] = {0, 
		Bp, Bp, Bp, Bp, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, Lq*Bp, 0, -Lq*Bp, 0, 0, 0, 0, 0, 0, 0, 0, 
		-Lq*Bp, 0, Lq*Bp, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 
		0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 
		0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 
		1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
		0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};
	
	// optimization parameters
	lp = glp_create_prob();
	glp_set_prob_name(lp, "yawtorque");
	
	// adds constraint parameters
	glp_add_rows(lp, cons);
	glp_set_row_name(lp,  1, "thrust");
	glp_set_row_name(lp,  2, "torque_x");
	glp_set_row_name(lp,  3, "torque_y");
	glp_set_row_name(lp,  4, "minspeed_1");
	glp_set_row_name(lp,  5, "minspeed_2");
	glp_set_row_name(lp,  6, "minspeed_3");
	glp_set_row_name(lp,  7, "minspeed_4");
	glp_set_row_name(lp,  8, "maxspeed_1");
	glp_set_row_name(lp,  9, "maxspeed_2");
	glp_set_row_name(lp, 10, "maxspeed_3");
	glp_set_row_name(lp, 11, "maxspeed_4");
	
	// sets the equality constraints
	glp_set_row_bnds(lp,  1, GLP_FX, 0, 0);
	glp_set_row_bnds(lp,  2, GLP_FX, 0, 0); 
	glp_set_row_bnds(lp,  3, GLP_FX, 0, 0);
	glp_set_row_bnds(lp,  4, GLP_FX, omega_sq_min, omega_sq_min);
	glp_set_row_bnds(lp,  5, GLP_FX, omega_sq_min, omega_sq_min);
	glp_set_row_bnds(lp,  6, GLP_FX, omega_sq_min, omega_sq_min);
	glp_set_row_bnds(lp,  7, GLP_FX, omega_sq_min, omega_sq_min);
	glp_set_row_bnds(lp,  8, GLP_FX, omega_sq_max, omega_sq_max);
	glp_set_row_bnds(lp,  9, GLP_FX, omega_sq_max, omega_sq_max);
	glp_set_row_bnds(lp, 10, GLP_FX, omega_sq_max, omega_sq_max);
	glp_set_row_bnds(lp, 11, GLP_FX, omega_sq_max, omega_sq_max);
	
	// adds the variables into the mix
	glp_add_cols(lp, vars); 
	glp_set_col_name(lp,  1, "wsq1");
	glp_set_col_name(lp,  2, "wsq2");
	glp_set_col_name(lp,  3, "wsq3");
	glp_set_col_name(lp,  4, "wsq4");
	glp_set_col_name(lp,  5, "v1");
	glp_set_col_name(lp,  6, "v2");
	glp_set_col_name(lp,  7, "v3");
	glp_set_col_name(lp,  8, "v4");
	glp_set_col_name(lp,  9, "w1");
	glp_set_col_name(lp, 10, "w2");
	glp_set_col_name(lp, 11, "w3");
	glp_set_col_name(lp, 12, "w4");
	
	// sets lower and upper bounds for squared motor speed
	glp_set_col_bnds(lp,  1, GLP_LO, 0.0, 0.0); 	
	glp_set_col_bnds(lp,  2, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  3, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  4, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  5, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  6, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  7, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  8, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp,  9, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp, 10, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp, 11, GLP_LO, 0.0, 0.0);
	glp_set_col_bnds(lp, 12, GLP_LO, 0.0, 0.0);
	
	// sets the objective function values
	// [ Kp -Kp  Kp -Kp]
	glp_set_obj_coef(lp,  1,  Kp);	
	glp_set_obj_coef(lp,  2, -Kp);
	glp_set_obj_coef(lp,  3,  Kp);
	glp_set_obj_coef(lp,  4, -Kp);
	glp_set_obj_coef(lp,  5, 0);
	glp_set_obj_coef(lp,  6, 0);
	glp_set_obj_coef(lp,  7, 0);
	glp_set_obj_coef(lp,  8, 0);
	glp_set_obj_coef(lp,  9, 0);
	glp_set_obj_coef(lp, 10, 0);
	glp_set_obj_coef(lp, 11, 0);
	glp_set_obj_coef(lp, 12, 0);
	
	// loads in the A matrix information
	glp_load_matrix(lp, vars*cons, ia, ja, ar);
}


void YawOptimizer::inputUpdate(double Uq[])
{
	// sets the equality constraints	
	glp_set_row_bnds(lp,  1, GLP_FX, Uq[Th], Uq[Th]);
	glp_set_row_bnds(lp,  2, GLP_FX, Uq[Tx], Uq[Tx]); 
	glp_set_row_bnds(lp,  3, GLP_FX, Uq[Ty], Uq[Ty]);
}


void YawOptimizer::useSimplexPrimal()
{
	useSimplex = true;
	useIntPnt  = false;
	parm_sim.meth = GLP_PRIMAL;
}

void YawOptimizer::useSimplexDual()
{
	useSimplex = true;
	useIntPnt  = false;
	parm_sim.meth = GLP_DUAL;
}

void YawOptimizer::useIntPntNatural()
{
	useSimplex = false;
	useIntPnt  = true;
	parm_int.ord_alg = GLP_ORD_NONE;
}

void YawOptimizer::useIntPntQuotient()
{
	useSimplex = false;
	useIntPnt  = true;
	parm_int.ord_alg = GLP_ORD_QMD;
}

void YawOptimizer::useIntPntApprox()
{
	useSimplex = false;
	useIntPnt  = true;
	parm_int.ord_alg = GLP_ORD_AMD;
}

void YawOptimizer::useIntPntSYMAMD()
{
	useSimplex = false;
	useIntPnt  = true;
	parm_int.ord_alg = GLP_ORD_SYMAMD;
}


void YawOptimizer::findMax()
{
	glp_set_obj_dir(lp, GLP_MAX);

	if(useSimplex) {
		glp_simplex(lp, &parm_sim);
		yawMax = glp_get_obj_val(lp);
		max_result_sim = glp_get_status(lp);
		
		// if optimal or feasible, mark as valid
		if((max_result_sim == GLP_OPT) || (max_result_sim == GLP_FEAS))
			validMax = true;
		// otherwise, mark as invalid
		// infeasible, no feasible solution, unbounded, or undefined.
		else
			validMax = false;
	}
	else if(useIntPnt) {
		glp_interior(lp, &parm_int);
		yawMax = glp_ipt_obj_val(lp);
		max_result_int = glp_ipt_status(lp);
		

		// if optimal or feasible, return true
		if((max_result_int == GLP_OPT) || (max_result_int == GLP_FEAS))
			validMax = true;
		// returns false if infeasible, no feasible solution, unbounded, or undefined.
		else
			validMax = false;
	}
	else {
		cout << "Optimization algorithm not set; Exiting!" << endl;
		exit(1);
	}

	// Gets the number of iterations to convergence
	maxIters = lp->it_cnt;
	lp->it_cnt = 0;
}


void YawOptimizer::findMin()
{
	glp_set_obj_dir(lp, GLP_MIN);

	if(useSimplex) {
		glp_simplex(lp, &parm_sim);
		yawMin = glp_get_obj_val(lp);
		min_result_sim = glp_get_status(lp);
		

		// if optimal or feasible, mark as valid
		if((min_result_sim == GLP_OPT) || (min_result_sim == GLP_FEAS))
			validMin = true;
		// otherwise, mark as invalid
		// infeasible, no feasible solution, unbounded, or undefined.
		else
			validMin = false;
	}
	else if(useIntPnt) {
		glp_interior(lp, &parm_int);
		yawMin = glp_ipt_obj_val(lp);
		min_result_int = glp_ipt_status(lp);
		

			// if optimal or feasible, return true
		if((min_result_int == GLP_OPT) || (min_result_int == GLP_FEAS))
			validMin = true;
	// returns false if infeasible, no feasible solution, unbounded, or undefined.
		else
			validMin = false;
	}
	else {
		cout << "Optimization algorithm not set; Exiting!" << endl;
		exit(1);
	}

	// Gets the number of iterations to convergence
	minIters = lp->it_cnt;
	lp->it_cnt = 0;
}


bool   YawOptimizer::isValidMax() 	{ return validMax;}
bool   YawOptimizer::isValidMin() 	{ return validMin;}
double YawOptimizer::getMax() 		{ return yawMax;}
double YawOptimizer::getMin()		{ return yawMin;}
int    YawOptimizer::getMaxIters()	{ return maxIters;}
int    YawOptimizer::getMinIters()	{ return minIters;}
int    YawOptimizer::getMaxStatus()
{
	if(useSimplex) 	return max_result_sim;
	if(useIntPnt)	return max_result_int;
	return -1;
}
int    YawOptimizer::getMinStatus()
{
	if(useSimplex)	return min_result_sim;
	if(useIntPnt)	return min_result_int;
	return -1;
}
