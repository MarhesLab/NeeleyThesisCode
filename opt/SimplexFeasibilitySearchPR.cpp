#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <glpk.h>

#include "PitchRollLimiter.h"
#include "../thread/LoggerThread.h"
#include "../functions/Timer.h"
#include "../quad/parameters.h"

string loc = "Results/";
string name = "simplexPR";
string currentsys = "_cmp";
string experiment = "";
string file = ".csv";
string fullfilename = "";

using namespace std;

void RunAndLogSimplex(PitchRollLimiter *pr_limit, string filename);
void GetDefFile();

int main(void) 
{
	// gets a file defining the different status returns
	//GetDefFile();

	// Sets up optimizer object
	PitchRollLimiter pr_limit;

	// Nonstandard Form, Primal problem
	pr_limit.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	pr_limit.useSimplexPrimal();

	experiment = "_nonstd_primal";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&pr_limit, fullfilename);


	// Nonstandard Form, Dual Problem
	pr_limit.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	pr_limit.useSimplexDual();

	experiment = "_nonstd_dual";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&pr_limit, fullfilename);


	// Standard Form, Primal Problem
	pr_limit.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	pr_limit.useSimplexPrimal();

	experiment = "_std_primal";	
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&pr_limit, fullfilename);


	// Standard Form, Dual Problem
	pr_limit.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	pr_limit.useSimplexDual();

	experiment = "_std_dual";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogSimplex(&pr_limit, fullfilename);

	return 0;
}


void RunAndLogSimplex(PitchRollLimiter *pr_limit, string filename)
{
	// Logging-related objects
	LoggerThread logfile(filename);
	Timer maxtime, mintime;

	logfile.initialize();	// initialize and create column titles
	logfile.startThread(); 
	logfile.addTextBlock("Thrust,Xtorque,Ytorque,Ztorque,");
	logfile.addTextBlock("maxXtorque,maxMicros,maxFeas,maxStatus,maxIters,");
	logfile.addTextBlock("minXtorque,minMicros,minFeas,minStatus,minIters");
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
		
		Uq[Tz] = 0;
		pr_limit->inputUpdate(Uq);

		//tz_max = -55;
		//tz_min = -55;

		// maximization
		maxtime.tic();
		pr_limit->findMax();		
		maxtime.toc();		
		
		// minimization
		mintime.tic();
		pr_limit->findMin();
		mintime.toc();
		
		// log results	
		logfile.logDoubleArray(Uq,4,4);	// log inputs

		logfile.logDouble(pr_limit->getMax(),4); // max values
		logfile.logDouble(maxtime.micros(),5);
		logfile.logBool(pr_limit->isValidMax());
		logfile.logInt(pr_limit->getMaxStatus());
		logfile.logInt(pr_limit->getMaxIters());

		logfile.logDouble(pr_limit->getMin(),4); // max values
		logfile.logDouble(mintime.micros(),5);
		logfile.logBool(pr_limit->isValidMin());
		logfile.logInt(pr_limit->getMinStatus());
		logfile.logInt(pr_limit->getMinIters());
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
