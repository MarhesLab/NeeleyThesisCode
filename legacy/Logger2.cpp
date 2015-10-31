#include "Logger2.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h> // for exit()
#include <time.h> // for getting system time and date

using namespace std;

Logger2::Logger2(string name)
{
	queue.str( string() ); // ensures it's empty
	fileName = name;
	nameSet = true;
	newline = true;
}

Logger2::Logger2()
{
	queue.str( string() );
	nameSet = false;
	newline = true;
}


Logger2::~Logger2()
{
	fclose(filePointer);
}


void Logger2::setName(string name)
{
	fileName = name;
	nameSet = true;
}


void Logger2::initialize()
{
	if(nameSet) {
		filePointer = fopen(fileName.c_str(),"w");
		if(filePointer == NULL) {
			cout << "Opening Logger failed!" << endl;
			exit(1);
		}
		
		setvbuf(filePointer, NULL, _IONBF, 0);
	}
	else {
		cout << "Could not open Logger; name not set!" << endl;
		exit(2);
	}
}

void Logger2::addTextBlock(string block)
{
	queue << block;
}


void Logger2::addTextLine(string line)
{
	// Writes the given line of text, adds a new line character, and flushes the writer object
	queue << line << "\n";
	string dummy = queue.str();
	fprintf(filePointer, "%s", dummy.c_str());
	queue.str( string() );

	// Flags that a new line was created
	newline = true;
}


void Logger2::newLine()
{
	// Write a new line character to the file and flushes the writer object
	queue << "\n";
	string dummy = queue.str();
	fprintf(filePointer,"%s", dummy.c_str());
	queue.str( string() );
	
	// Flags that a new line was created
	newline = true;
}


void Logger2::addLogTimeLine()
{
	// Types required to get the time
	time_t 		now = time(0);
	struct tm 	*tstruct;
	
	// Obtains the time
	tstruct = localtime(&now);

	// Extracts the desired values out of the struct
	int year = tstruct->tm_year + 1900; // tm_year = year since 1900
	int month = tstruct->tm_mon + 1; 	// tm_mon = months since January
	int day = tstruct->tm_mday;			// tm_mday = day of the month
	int hour = tstruct->tm_hour;		// tm_hour = hours since midnight
	int min = tstruct->tm_min;			// tm_min = minutes after the hour
	int sec = tstruct->tm_sec;			// tm_sec = seconds after the minute
	
	// Writes the time to the file
	queue << "Log File Start Time: " << year << "-" << month << "-" << day << "."
		<< hour << ":" << min << ":" << sec << "\n";
	string dummy = queue.str();
	fprintf(filePointer, "%s", dummy.c_str());
	queue.str( string() );
	
	// Flags that a new line was created
	newline = true;
}


void Logger2::logInt(int value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the value
	queue << value;
}


void Logger2::logIntArray(int values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the first value
	queue << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << values[i];
}


void Logger2::logDouble(double value, int prec)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the value
	queue << setprecision(prec) << fixed << value;
}


void Logger2::logDoubleArray(double values[], int size, int prec)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the first value
	queue << setprecision(prec) << fixed << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << values[i];
}


void Logger2::logBool(bool value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the value; 1 if true, 0 if false
	if(value)
		queue << "1";
	else
		queue << "0";
}


void Logger2::logBoolArray(bool values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the first value; 1 if true, 0 if false
	if(values[0])
		queue << "1";
	else
		queue << "0";

	// Writes subsequent values and commas
	for(int i = 1; i < size; i++)
	{
		if(values[i])
			queue << ",1";
		else
			queue << ",0";
	}
}


void Logger2::logString(string text)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the text
	queue << text;
}


void Logger2::logStringArray(string text[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the first string of text
	queue << text[0];

	// Writes subsequent text and commas
	for(int i = 0; i < size; i++)
		queue << "," << text[i];
}


void Logger2::logUChar(unsigned char value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";
	
	// Writes the value
	queue << (unsigned int)value;
}


void Logger2::logUCharArray(unsigned char values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		queue << ",";

	// Writes the first value
	queue << (unsigned int)values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << (unsigned int)values[i];
}
