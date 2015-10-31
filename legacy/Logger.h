#ifndef LOGGER_OBJECT_H
#define LOGGER_OBJECT_H

#include <fstream>
#include <string>
//#include <iomanip>

using namespace std;

class Logger {
  protected:
	string fileName;
	ofstream fileObject;
	bool nameSet;
	bool newline;

  public:
	Logger();
	Logger(string name);
	~Logger();

	void setName(string name);
	void initialize();
	void addTextBlock(string block);
	void addTextLine(string line);
	void addLogTimeLine();
	//void addColumnTitles(string titles);
	void newLine();

	void logInt(int value);
	void logIntArray(int values[], int size);
	void logDouble(double value);
	void logDoubleArray(double values[], int size);
	void logBool(bool value);
	void logBoolArray(bool values[], int size);
	void logString(string text);
	void logStringArray(string text[], int size);
	void logUChar(unsigned char value);
	void logUCharArray(unsigned char values[], int size);
};

#endif
