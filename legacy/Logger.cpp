#include "Logger.h"

#include <iostream>
#include <stdlib.h> // for exit()
#include <time.h> // for getting system time and date

using namespace std;

Logger::Logger(string name)
{
	fileName = name;
	nameSet = true;
	newline = true;
}

Logger::Logger()
{
	nameSet = false;
	newline = true;
}


Logger::~Logger()
{
	fileObject.close();
}


void Logger::setName(string name)
{
	fileName = name;
	nameSet = true;
}


void Logger::initialize()
{
	if(nameSet)
		fileObject.open(fileName);
	else {
		cout << "Could not open Logger; name not set!" << endl;
		exit(1);
	}
}

void Logger::addTextBlock(string block)
{
	fileObject << block;
}


void Logger::addTextLine(string line)
{
	// Writes the given line of text, adds a new line character, and flushes the writer object
	fileObject << line << "\n";
	fileObject.flush();
	
	// Flags that a new line was created
	newline = true;
}


void Logger::newLine()
{
	// Write a new line character to the file and flushes the writer object
	fileObject << "\n";
	fileObject.flush();
	
	// Flags that a new line was created
	newline = true;
}


void Logger::addLogTimeLine()
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
	fileObject << "Log File Start Time: " << year << "-" << month << "-" << day << "."
		<< hour << ":" << min << ":" << sec << "\n";
	fileObject.flush();
	
	// Flags that a new line was created
	newline = true;
}


void Logger::logInt(int value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the value
	fileObject << value;
}


void Logger::logIntArray(int values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the first value
	fileObject << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		fileObject << "," << values[i];
}


void Logger::logDouble(double value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the value
	fileObject << value;
}


void Logger::logDoubleArray(double values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the first value
	fileObject << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		fileObject << "," << values[i];
}


void Logger::logBool(bool value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the value; 1 if true, 0 if false
	if(value)
		fileObject << "1";
	else
		fileObject << "0";
}


void Logger::logBoolArray(bool values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the first value; 1 if true, 0 if false
	if(values[0])
		fileObject << "1";
	else
		fileObject << "0";

	// Writes subsequent values and commas
	for(int i = 1; i < size; i++)
	{
		if(values[i])
			fileObject << ",1";
		else
			fileObject << ",0";
	}
}


void Logger::logString(string text)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the text
	fileObject << text;
}


void Logger::logStringArray(string text[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the first bit of text
	fileObject << text[0];

	// Writes subsequent text and commas
	for(int i = 0; i < size; i++)
		fileObject << "," << text[i];
}


void Logger::logUChar(unsigned char value)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";
	
	// Writes the value
	fileObject << (unsigned int)value;
}


void Logger::logUCharArray(unsigned char values[], int size)
{
	// If the first object in a new line, don't write a comma
	if(newline)
		newline = false;
	else
		fileObject << ",";

	// Writes the first value
	fileObject << (unsigned int)values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		fileObject << "," << (unsigned int)values[i];
}
