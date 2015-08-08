#ifndef LOGGER_THREAD_H
#define LOGGER_THREAD_H

#include <pthread.h>	// multi-threading
#include <sstream>		// stringstream
#include <string>		// string

using namespace std;

// The main recording function to be transferred to its own thread
void* loggerThreadFunction(void *threadarg);

// Struct needed for passing data to the thread
struct LoggerStruct
{
	FILE* struct_file_pointer;
	string* struct_log_string;
	bool* struct_record;
	bool* struct_running;
};

// Class for running everything
class LoggerThread
{
  protected:
	struct LoggerStruct lstruct;
	pthread_t logging_thread;

	string fileName;
	FILE *filePointer;
	bool nameSet;
	bool startOfLine;
	
	stringstream queue;
	string log_string;
	bool record;

	bool running;

  public:
	LoggerThread();
	LoggerThread(string name);
	~LoggerThread();

	void setName(string name);
	void initialize();
	bool startThread();

	void addTextBlock(string block);
	void addTextLine(string line);
	void addLogTimeLine();
	void newLine();
	void commaCheck();

	void logInt(int value);
	void logIntArray(int values[], int size);
	void logDouble(double value, int prec);
	void logDoubleArray(double values[], int size, int prec);
	void logBool(bool value);
	void logBoolArray(bool values[], int size);
	void logString(string text);
	void logStringArray(string text[], int size);
	void logUChar(unsigned char value);
	void logUCharArray(unsigned char values[], int size);
};


#endif
