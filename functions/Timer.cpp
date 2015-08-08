#include <stdlib.h> // for NULL
#include "Timer.h"

// Constructor and Destructor (nothing special needed)
Timer::Timer() {}
Timer::~Timer() {}

void Timer::tic()
{
	gettimeofday(&startTime, NULL);
	start = startTime.tv_sec + (startTime.tv_usec/1000000.0);
}

void Timer::toc()
{
	gettimeofday(&stopTime, NULL);
	stop = stopTime.tv_sec + (stopTime.tv_usec/1000000.0);
	diff = stop - start;
}


void Timer::toctic()
{
	toc();
	tic();
}


double Timer::secs()
{
	return diff;
}


double Timer::millis()
{
	return 1000.0*diff;
}


double Timer::micros()
{
	return 1000000.0*diff;
}
