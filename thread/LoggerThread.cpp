/*
	For more information on multithreading, see the PDF titled
	"POSIX Threading Programming Resource" in this folder.
*/

#include "LoggerThread.h"

#include <iostream>	// printing error messages
#include <iomanip>	// setprecision
#include <stdlib.h>	// exit()
#include <time.h>	// date info
#include <unistd.h>	// usleep

using namespace std;

// Global Variable Mutex Definition
// NOTE: This should get shared across all instances of LoggerThread classes!
pthread_mutex_t logger_mutex;

// Default constructor
LoggerThread::LoggerThread()
{
	nameSet = false;
	startOfLine = true;

	queue.str( string() ); // ensures it's empty
	record = false;
	
	running = true;
}


// Constructor with file name
LoggerThread::LoggerThread(string name)
{
	fileName = name;
	nameSet = true;
	startOfLine = true;

	queue.str( string() );
	record = false;
	
	running = true;
}


// Destructor
LoggerThread::~LoggerThread()
{
	// flags the thread for shutdown
	running = false;

	// Joins the thread to wait for it to terminate
	void *status;
	pthread_join(logging_thread, &status);

	// Closes the file
	fclose( filePointer );
}


// Sets the name (if it wasn't set at object creation)
void LoggerThread::setName(string name)
{
	fileName = name;
	nameSet = true;
}


// Initializes the logging file and starts the recording thread
void LoggerThread::initialize()
{
	// Exits the program if the file name's not set
	if(!nameSet) {
		cout << "Could not open LoggerThread file; name not set!" << endl;
		exit(2);
	}

	// Opens the file; exits if it can't
	filePointer = fopen(fileName.c_str(),"w");
	if(filePointer == NULL) {
		cout << "LoggerThread could not open " << fileName << "; Exiting..." << endl;
		exit(1);
	}

	// Sets the buffer to 0 so it writes immediately
	setvbuf(filePointer, NULL, _IONBF, 0);

	// Populates the struct for sending data to the thread
	lstruct.struct_file_pointer = filePointer;
	lstruct.struct_log_string	= &log_string;
	lstruct.struct_record		= &record;
	lstruct.struct_running		= &running;
}


// Actually starts the threaded process
bool LoggerThread::startThread()
{
	running = true;
	
	if(pthread_create(&logging_thread, NULL, loggerThreadFunction, (void *)&lstruct))
	{
		std::cout << "Could not create Logger Thread. \n\r";
		return false;
	}

	return true;
}


// Adds a chunk of text to the queue
void LoggerThread::addTextBlock(string block)
{
	queue << block;
}


// Adds a line of text to the queue, then creates a new line
void LoggerThread::addTextLine(string line)
{
	queue << line;
	newLine();
}


// Adds a line of info about the current time, then creates a new line
void LoggerThread::addLogTimeLine()
{
	// Types required to get the time
	time_t		now = time(0);
	struct tm	*tstruct;

	// Obtains the time
	tstruct = localtime(&now);

	// Extracts the desired values out of the struct
	int year 	= tstruct->tm_year + 1900; 	// tm_year = year since 1900
	int month 	= tstruct->tm_mon + 1; 		// tm_mon = months since January
	int day 	= tstruct->tm_mday;			// tm_mday = day of the month
	int hour 	= tstruct->tm_hour;			// tm_hour = hours since midnight
	int min 	= tstruct->tm_min;			// tm_min = minutes after the hour
	int sec 	= tstruct->tm_sec;			// tm_sec = seconds after the minute

	// Puts the info in the queue
	queue << "Log File Start Time: " << year << "-" << month << "-" << day << "."
		<< hour << ":" << min << ":" << sec;

	// Creates the new line
	newLine();
}


// Adds an end-of-line character, then passes the queue into
// the thread for writing to the file
void LoggerThread::newLine()
{
	// Adds the end-of-line character
	queue << "\n";

	// Locks the mutex, flags that there's info to record,
	// passes in the info to record, then unlocks the mutex
	pthread_mutex_lock(&logger_mutex);
	record = true;
	log_string += queue.str();
	pthread_mutex_unlock(&logger_mutex);
	
	// Clears the queue once it's been offloaded, then flags that it's a new line
	queue.str( string() );
	startOfLine = true;
}


// Checks if it should place a comma at the start of a set of values;
// doesn't place a comma if it's the start of a new line.
void LoggerThread::commaCheck()
{
	if(startOfLine)
		startOfLine = false;
	else
		queue << ",";
}


// Logs a single integer value
void LoggerThread::logInt(int value)
{
	// Checks if first value in line
	commaCheck();

	// Writes the value
	queue << value;
}


// Logs an array of integers
void LoggerThread::logIntArray(int values[], int size)
{
	// Checks if first value in line
	commaCheck();

	// Writes the first value
	queue << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << values[i];
}


// Logs a single double precision floating point number
// at a desired precision (general precision; not past the
// decimal point)
void LoggerThread::logDouble(double value, int prec)
{
	// Checks if first value in line
	commaCheck();

	// Writes the value
	queue << setprecision(prec) << fixed << value;
}


// Logs an array of double precision floating point numbers
// at a desired precision (general precision; not past the
// decimal point)
void LoggerThread::logDoubleArray(double values[], int size, int prec)
{
	// Checks if first value in line
	commaCheck();

	// Writes the first value
	queue << setprecision(prec) << fixed << values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << values[i];
}


// Logs a single Boolean value as either a 1 or a 0
void LoggerThread::logBool(bool value)
{
	// Checks if first value in line
	commaCheck();

	// Writes the value; 1 if true, 0 if false
	if(value)
		queue << "1";
	else
		queue << "0";
}


// Logs an array of Boolean values
void LoggerThread::logBoolArray(bool values[], int size)
{
	// Checks if first value in line
	commaCheck();

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


// Logs a single string
void LoggerThread::logString(string text)
{
	// Checks if first value in line
	commaCheck();

	// Writes the text
	queue << text;
}


// Logs an array of strings
void LoggerThread::logStringArray(string text[], int size)
{
	// Checks if first value in line
	commaCheck();

	// Writes the first string of text
	queue << text[0];

	// Writes subsequent text and commas
	for(int i = 0; i < size; i++)
		queue << "," << text[i];
}


// Logs a single unsigned character value
void LoggerThread::logUChar(unsigned char value)
{
	// Checks if first value in line
	commaCheck();

	// Writes the value
	queue << (unsigned int)value;
}


// Logs an array of unsigned character values
void LoggerThread::logUCharArray(unsigned char values[], int size)
{
	// Checks if first value in line
	commaCheck();

	// Writes the first value
	queue << (unsigned int)values[0];

	// Write subsequent values and commas
	for(int i = 1; i < size; i++)
		queue << "," << (unsigned int)values[i];
}


// The actual function that gets sent to its own thread. This function periodically
// checks to see if data is ready to be saved to the log file; if so, it grabs it,
// then tries to write it.
void* loggerThreadFunction(void *threadarg)
{
	// unpacking data passed in as a struct
	struct LoggerStruct *lstruct;
	lstruct = (struct LoggerStruct *) threadarg;

	// declaring the appropriate variables
	FILE* filePointer 	= lstruct->struct_file_pointer;
	string* log_string	= lstruct->struct_log_string;
	bool* record		= lstruct->struct_record;
	bool* running		= lstruct->struct_running;

	string buffer;
	
	// Keep checking for prepared values
	while(*running || *record)
	{
		// If data's prepared for being recorded...
		if(*record)
		{
			// lock the mutex, reset the flag, grab the data, and clear the data string
			pthread_mutex_lock(&logger_mutex);
			*record = false;
			buffer = *log_string;
			*log_string = string();
			pthread_mutex_unlock(&logger_mutex);

			// Write the data to the file. This part can periodically bog down
			// with the operating system.
			fprintf(filePointer, "%s", buffer.c_str());
		}
		
		// If nothing's ready, go to sleep for half a millisecond
		usleep(500);
	}

	// Kill the thread
	pthread_exit(NULL);
}
