#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <glpk.h>

#include "YawOptimizer.h"
#include "../thread/LoggerThread.h"
#include "../functions/Timer.h"
#include "../quad/parameters.h"

string loc = "Results/";
string name = "simplex";
string currentsys = "_cmp";
string experiment = "";
string file = ".csv";
string fullfilename = "";

using namespace std;

void RunAndLogSimplex(YawOptimizer *yaw_opt, string filename);
void GetDefFile();

int main(void) 
{
	// gets a file defining the different status returns
	//GetDefFile();

	// Sets up optimizer object
	YawOptimizer yaw_opt;

	// Nonstandard Form, Primal problem
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useSimplexPrimal();

	experiment = "_nonstd_primal";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&yaw_opt, fullfilename);


	// Nonstandard Form, Dual Problem
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useSimplexDual();

	experiment = "_nonstd_dual";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&yaw_opt, fullfilename);


	// Standard Form, Primal Problem
	yaw_opt.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useSimplexPrimal();

	experiment = "_std_primal";	
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&yaw_opt, fullfilename);


	// Standard Form, Dual Problem
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useSimplexDual();

	experiment = "_std_dual";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&yaw_opt, fullfilename);

	return 0;
}


void RunAndLogSimplex(YawOptimizer *yaw_opt, string filename)
{
	// Logging-related objects
	LoggerThread logfile(filename);
	Timer maxtime, mintime;

	logfile.initialize();	// initialize and create column titles
	logfile.startThread();
	logfile.addTextBlock("Thrust,Xtorque,Ytorque,Ztorque,");
	logfile.addTextBlock("maxZtorque,maxMicros,maxFeas,maxStatus,maxIters,");
	logfile.addTextBlock("minZtorque,minMicros,minFeas,minStatus,minIters");
	logfile.newLine();
	

	// Input location defintions
	const int Th = 0, Tx = 1, Ty = 2;

	// variable declaration
	double Uq[] = {5, 0, 0, 0};
	long i = 0;

	// loops over all the thrust, X torque, and Y torque values
	for(Uq[Th] = 0;    Uq[Th] <= 22;  Uq[Th] += 1) {
	for(Uq[Tx] = -1.5; Uq[Tx] <= 1.5; Uq[Tx] += 0.125) {
	for(Uq[Ty] = -1.5; Uq[Ty] <= 1.5; Uq[Ty] += 0.125) {
		if((i%1000 == 0 ) && (i != 0))
			cout << "Finished " << i << " Points\n";
		
		yaw_opt->inputUpdate(Uq);

		//tz_max = -55;
		//tz_min = -55;

		// maximization
		maxtime.tic();
		yaw_opt->findMax();		
		maxtime.toc();		
		
		// minimization
		mintime.tic();
		yaw_opt->findMin();
		mintime.toc();
		
		// log results	
		logfile.logDoubleArray(Uq,4,4);	// log inputs

		logfile.logDouble(yaw_opt->getMax(),4); // max values
		logfile.logDouble(maxtime.micros(),6);
		logfile.logBool(yaw_opt->isValidMax());
		logfile.logInt(yaw_opt->getMaxStatus());
		logfile.logInt(yaw_opt->getMaxIters());

		logfile.logDouble(yaw_opt->getMin(),4); // max values
		logfile.logDouble(mintime.micros(),6);
		logfile.logBool(yaw_opt->isValidMin());
		logfile.logInt(yaw_opt->getMinStatus());
		logfile.logInt(yaw_opt->getMinIters());
		logfile.newLine();

		// Increase evaluation point counter
		i++;
	}
	}
	}
}

void GetDefFile()
{
	// log file setup
	ofstream deffile;
	
	deffile.open("lpSolverStatus.csv");
	deffile << "GLP_OPT,GLP_FEAS,GLP_INFEAS,"
		<< "GLP_NOFEAS,GLP_UNBND,GLP_UNDEF" 
		<< GLP_OPT << "," << GLP_FEAS << "," 
		<< GLP_INFEAS << "," << GLP_NOFEAS << "," 
		<< GLP_UNBND << "," << GLP_UNDEF << "\n";
	deffile.close();
}
