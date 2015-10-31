#ifndef LOGGER_OBJECT_2_H
#define LOGGER_OBJECT_2_H

#include <stdio.h>
#include <string>
#include <sstream>
//#include <iomanip>

using namespace std;

class Logger2 {
  protected:
	string fileName;
	FILE *filePointer;
	bool nameSet;
	bool newline;
	stringstream queue;

  public:
	Logger2();
	Logger2(string name);
	~Logger2();

	void setName(string name);
	void initialize();
	void addTextBlock(string block);
	void addTextLine(string line);
	void addLogTimeLine();
	//void addColumnTitles(string titles);
	void newLine();

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
