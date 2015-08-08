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
string name = "interior";
string currentsys = "_cmp";
string experiment = "";
string file = ".csv";
string fullfilename = "";

using namespace std;

void RunAndLogInterior(YawOptimizer *yaw_opt, string filename);
void GetDefFile();

int main(void) 
{
	// gets a file defining the different status returns
	//GetDefFile();

	// Sets up optimizer object
	YawOptimizer yaw_opt;

	// Nonstandard Form, Natural Ordering
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntNatural();

	experiment = "_nonstd_nat";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Nonstandard Form, Quotient Minimum Degree Ordering
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntQuotient();

	experiment = "_nonstd_qmd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Nonstandard Form, Approx. Minimum Degree Ordering
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntApprox();

	experiment = "_nonstd_amd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Nonstandard Form, Approx. Minimum Degree SYMAMD
	yaw_opt.makeModel(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntSYMAMD();

	experiment = "_nonstd_symamd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);



	// Standard Form, Natural Ordering
	yaw_opt.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntNatural();

	experiment = "_std_nat";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Standard Form, Quotient Minimum Degree Ordering
	yaw_opt.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntQuotient();

	experiment = "_std_qmd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Standard Form, Approx. Minimum Degree Ordering
	yaw_opt.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntApprox();

	experiment = "_std_amd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);


	// Standard Form, Approx. Minimum Degree SYMAMD
	yaw_opt.makeModelStd(Bp_calc[0], Kp_calc[0], Lq_meas[0]);
	yaw_opt.useIntPntSYMAMD();

	experiment = "_std_symamd";
	fullfilename = loc + name + currentsys + experiment + file;

	RunAndLogInterior(&yaw_opt, fullfilename);

	return 0;
}


void RunAndLogInterior(YawOptimizer *yaw_opt, string filename)
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
