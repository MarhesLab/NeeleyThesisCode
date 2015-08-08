#ifndef AUTOPILOT_COMM_H
#define AUTOPILOT_COMM_H

#include <stdio.h> // FILE

// Character Definitions for Custom Data Frames
#define SOF 234 //Start of Frame
#define EOFm 213 //End of Frame
#define DMC 201 //Direct Motor Command
#define VERSION 202 //HL Code Version Number
#define DEBUGMODE 203 //Debug Mode = data[0]  (not implemented currently)
#define GPIOSET 204 //GPIO SSSS (0 = off, !0 = on)
#define GPIOTOGGLE 205
#define ECHO 206

#define IMUFRAME 0
#define STATUSFRAME 1
#define ECHOFRAME 2
#define VERSIONFRAME 3

// Serial Port Initialization

int initSerialPort(FILE ** fp);

int initSerialPort(FILE ** fp, bool text);

// Conversion functions
unsigned char rpmToDMC(const double speed_in_rpms);

unsigned char radsToDMC(const double speed_in_rads);

// Writing to the Autopilot
void setMotorSpeeds(FILE *fp_write, const double speeds[], bool printToScreen);

void setMotorChars(FILE *fp_write, unsigned char speeds[], bool printToScreen);

void stopMotors(FILE *fp_write);

void getHLVersion(FILE *fp_write);

void setGPIO(FILE *fp_write, int state);

void toggleGPIO(FILE *fp_write);

void sendEchoFrame(FILE *fp_write, char fiveChars[], bool printToScreen);


// Reading from the Autopilot

void getAutopilotFrame(FILE *fp_read, unsigned char frame[], unsigned char &frametype, unsigned char &timestamp);

void processOldImuFrame(unsigned char frame[], double data_array[]);

void processImuFrame(unsigned char frame[], double data_array[]);

void processStatusFrame(unsigned char frame[], short &status, double &battery_voltage, double &cpu_load, double motor_rpms[]);

void processEchoFrame(unsigned char frame[], char text[]);

void processVersionFrame(unsigned char frame[], char text[]);

#endif
