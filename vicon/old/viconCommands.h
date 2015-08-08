#ifndef VICON_COMMANDS_H
#define VICON_COMMANDS_H

#include <string>
#include <iostream>
#include "vicon_pos.h" // getPositionData()

void waitForViconUpdate(vicon_pos *vicon); 

void getPositionData(vicon_pos *vicon, double XquadVicon[], std::string quadName);

void getTimeStampInformation(vicon_pos *vicon);

double getTotalViconLatency(vicon_pos *vicon);

#endif
