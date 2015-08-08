#include <iostream>
#include <stdlib.h>
#include <string>

#include "LoggerThread.h"

using namespace std;

int main()
{
	// Settings for the file
	string name = "logfile_example.csv";

	// Creates the logger
	LoggerThread lthread(name);

	// Sets up and starts the logger
	lthread.initialize();
	if(!lthread.startThread())
		exit(-1);

	// Adds some information to the top
	lthread.addLogTimeLine();	// current time

	string lineOfText = "This is an example. It's fairly simple.";
	lthread.addTextLine( lineOfText );	

	// Example data points (for starters)
	int integer = 0;
	int ints[5] = { 1, 2, 3, 4, 5};
	double dub = 0.1;
	double dubs[5] = { 0.2, 0.3, 0.4, 0.5, 0.6};
	bool boolie = false;
	bool boolies[5] = {true, false, true, false, true};
	string da_string = "string";
	string da_strings[5] = {"string1","string2","string3","string4","string5"};
	unsigned char uchar = 0;
	unsigned char uchars[5] = { 1, 2, 3, 4, 5};

	// Start adding data
	for( int i = 0; i < 100; i++) {
		// Logs the values
		lthread.logInt(integer);
		lthread.logIntArray(ints, 5);
		lthread.logDouble(dub, 4);
		lthread.logDoubleArray(dubs, 5, 6);
		lthread.logBool(boolie);
		lthread.logBoolArray(boolies, 5);
		lthread.logString(da_string);
		lthread.logStringArray(da_strings, 5);
		lthread.logUChar(uchar);
		lthread.logUCharArray(uchars, 5);
		lthread.newLine();

		// Updates (some of) the values
		integer++;
		for(int i = 0; i < 5; i++) 		ints[i]++;
		dub += 0.1;
		for(int i = 0; i < 5; i++)		dubs[i] += 0.1;
		boolie = !boolie;
		for(int i = 0; i < 5; i++)		boolies[i] = !boolies[i];
		uchar++;
		for(int i = 0; i < 5; i++)		uchars[i]++;
	}
	
	return 0;
}
