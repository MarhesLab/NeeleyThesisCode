#ifndef TIMER_OBJECT_H
#define TIMER_OBJECT_H

#include <sys/time.h>

class Timer {
  protected:
	struct timeval startTime;
	struct timeval stopTime;
	double start;
	double stop;
	double diff;
  public:
	Timer();
	~Timer();

	void tic();	// start recording time
	void toc();  // find time since start
	void toctic(); // find time since start, then reset the start time
	double secs();
	double millis();
	double micros();
};


#endif
