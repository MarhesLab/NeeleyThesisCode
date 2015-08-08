#include <cstdlib> // system
#include <iostream> // cout
#include <unistd.h> // sleep and usleep

#include "autopilot.h"
#include "../defs.h" // AP_SERIAL, BAUD_RATE, maxDMCchar, and conversion definitions

using namespace std;

/**************************************************************************************
	AutoPilot Port Initialization

	by: Paul Groves
	August 2014

	Function Dependencies:
	None

	This function receives a FILE pointer and initializes it with the correct 
	conditions to communicate with the quadrotor. The relevant Linux serial port and baud
	rate are set in "defs.h" by AP_SERIAL and BAUD_RATE, respectively. 

	A relevant URL for figuring out how to change the file pointer after passing
	it into a function:
	http://stackoverflow.com/questions/6965439/changing-the-value-a-file-pointer-points-to-inside-a-c-function
**************************************************************************************/

int initSerialPort(FILE ** fp)
{	return initSerialPort(fp, true);}

int initSerialPort(FILE ** fp, bool text)
{
	// Configuring the AutoPilot Serial Port
	if(text)
		cout << "\n\rConfiguring the AutoPilot Serial Port...\n\r";
	system(("stty -F " + AP_SERIAL + " " + BAUD_RATE + 
		" raw -iexten -echo -echoe -echok -echoctl -echoke").c_str());   // AP_SERIAL and BAUD_RATE found in "defs.h"
	sleep(0.005); // short sleep to let the command take effect
	
	// Opening the AutoPilot Serial Port
	if(text)
		cout << "Opening the AutoPilot's Serial Port...\n\r";
	*fp = fopen((AP_SERIAL).c_str(),"rw+");
	
	// If it wasn't set up correctly, return 0
	if(!*fp)
	{
		cout << "Could not open AutoPilot port!\n\r\n\r";
		return 0;
	}
	
	// otherwise, turn off buffering and return 1
	setvbuf(*fp, NULL, _IONBF, 0);
	cout << "Autopilot port opened\n\r";
	return 1;
}

/**************************************************************************************


**************************************************************************************/


unsigned char rpmToDMC(const double speed_in_rpms)
{
	// from sdk.h in ASCTEC 3.0 SDK: RPM = (25 + (motor*175)/200)*43
	return (int)((((double)speed_in_rpms/43.0)-25.0)*(200.0/175.0) + 0.5); // 0.5 added for rounding
}

unsigned char radsToDMC(const double speed_in_rads)
{
	return rpmToDMC(rad2rpm*speed_in_rads);
}



/**************************************************************************************
	Sending Motor Speeds to the Quadrotor

	by: Paul Groves
	June 2014

	This function receives a "file" (in this case, the port connected to the 
	communication link to the quadrotor, at present through XBee) to which it will
	send commands, as well as the four motor speeds (in radians/second) to send.
	The function changes the motor speeds to units of rpm, then converts these values
	to the ones the Asctec Hummingbird SDK is expecting. Finally, it places these
	computed values into a frame and sends the frame out of the port.

	NOTES:

	-A sent value of 0 will completely shut the motor off. It takes a while for the
	 motor to turn back on, and it will likely stutter.
	-The physical model used throughout the program and the Asctec SDK have a different
	 motor naming convention. Starting at the orange marker and moving counter clockwise:
		What the physical model uses: 1, 2, 3, 4
		What the Asctec SDK expects:  1, 3, 2, 4
**************************************************************************************/

void setMotorSpeeds(FILE *fp_write, const double speeds[], bool printToScreen)
{
	double rpms[4];
	unsigned char *dmc; // direct motor commands
	unsigned char swap; // Motor 1 = 1st, 2 = 3rd, 3 = 2nd, 4 = 4th
	unsigned char frame[8] = {SOF, DMC, 0, 0, 0, 0, 0, EOFm};
	
	dmc = frame + 2;
	
	for(int i = 0; i < 4; i++) {
		// Convert given speeds from radians per second to RPM
		rpms[i] = speeds[i]*rad2rpm;
		
		// Now convert from RPMs to Direct Motor Commands
		if(rpms[i] < 1075)
			dmc[i] = 0;
		else
			dmc[i] = rpmToDMC(rpms[i]);
			
		// check upper limit
		if(dmc[i] > maxDMCchar) // maxDMCchar defined in "defs.h"
			dmc[i] = maxDMCchar;
	}
	
	swap = dmc[1];
	dmc[1] = dmc[2];
	dmc[2] = swap; // Swap model-based motor labels to match the AscTec-based motor labels
	
	if(printToScreen) {
		printf("\n\rFrame sent: %i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i \n\r", //prints the frame to the screen for viewing
			frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7]);
	}
	
	fwrite(frame, 1, 8, fp_write);
}


void setMotorChars(FILE *fp_write, unsigned char speeds[], bool printToScreen)
{
	unsigned char *dmc; // direct motor commands
	unsigned char swap; // Motor 1 = 1st, 2 = 3rd, 3 = 2nd, 4 = 4th
	unsigned char frame[8] = {SOF, DMC, 0, 0, 0, 0, 0, EOFm};
	
	dmc = frame + 2;

	for(int i = 0; i < 4; i++) {
		dmc[i] = speeds[i];

		if(dmc[i] > maxDMCchar) {// maxDMCchar defined in "defs.h"
			dmc[i] = maxDMCchar;
		}
	}

	swap = dmc[1];
	dmc[1] = dmc[2];
	dmc[2] = swap; // Swap model-based motor labels to match the AscTec-based motor labels
	
	if(printToScreen) {
		printf("\n\rFrame sent: %i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i \n\r", //prints the frame to the screen for viewing
			frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7]);
	}
	
	fwrite(frame, 1, 8, fp_write);
}


/**************************************************************************************
	Stopping Quadrotor Motors

	by: Paul Groves
	August 2014

	Function Dependencies:
	None

	This function receives a FILE pointer for sending commands to the quadrotor,
	then repeatedly sends commands to shut down the motors. It sends 100 commands in
	approximately one second. Since the serial port communication is susceptible
	to potential delays and frame loss, a brute force approach is taken to make sure 
	the proper shut-down command is received. 
**************************************************************************************/

void stopMotors(FILE *fp_write)
{
	double Omegas[4] = {0, 0, 0, 0};
	
	printf("\n\r***  SHUTTING DOWN THE MOTORS  ***\n\r");

	for (int i = 0; i < 10; i++) 
	{	
		setMotorSpeeds(fp_write, Omegas, false);
		usleep(10000); //Sleep for 10 ms.
	}

	printf("*** 10 SHUTDOWN COMMANDS SENT ***\n\r");
}


/**************************************************************************************
	Getting High Level Controller Version

	by: Wil Neeley
	February 2015

	This function sends a carefully constructed frame to the quadrotor's high level
	controller on the Autopilot board. The Autopilot will send back its version
	number for processing. 
**************************************************************************************/

void getHLVersion(FILE *fp_write)
{
	unsigned char frame[8] = {SOF, VERSION, 0, 0, 0, 0, 0, EOFm};

	fwrite(frame, 1, 8, fp_write);
}


/**************************************************************************************
	Setting GPIO Pin State

	by: Paul Groves
	August 2014

	Function Dependencies:
	None

	This function receives a FILE pointer for sending commands to the quadrotor and
	a particular GPIO pin state. It sends the appropriate frame of information to
	set the GPIO pin to the desired state (1 for on, 0 for off). 
**************************************************************************************/

void setGPIO(FILE *fp_write, int state)
{
	unsigned char frame[8] = {SOF, GPIOSET, 0, 0, 0, 0, 0, EOFm};

	if (state != 0)
		frame[2] = 1; //1 = ON, 0 = OFF.

	fwrite(frame, 1, 8, fp_write);
}

void toggleGPIO(FILE *fp_write)
{
	unsigned char frame[8] = {SOF, GPIOTOGGLE, 0, 0, 0, 0, 0, EOFm};

	fwrite(frame, 1, 8, fp_write);
}


/**************************************************************************************
	Echo Frame Construction

	by: Wil Neeley
	January 2015


	Function Dependencies:
	None

	This function receives a FILE pointer for sending a frame to the quadrotor and
	a character array containing five characters. The five characters are sent in
	a special frame format, which the quadrotor will echo back. 

**************************************************************************************/

void sendEchoFrame(FILE *fp_write, char fiveChars[], bool printToScreen)
{
	unsigned char frame[8] = {SOF, ECHO, 0, 0, 0, 0, 0, EOFm};

	for(int i = 0; i < 5; i++)
		frame[2+i] = fiveChars[i];

	if(printToScreen) {
		printf("\n\rFrame sent: %i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i \n\r", //prints the frame to the screen for viewing
			frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7]);
	}

	fwrite(frame, 1, 8, fp_write);
}



/**************************************************************************************
	Obtaining Data Frame from Quadrotor

	by: Wil Neeley
	September 2014

	Function Dependencies:
	None

	EXPLANATION
**************************************************************************************/

void getAutopilotFrame(FILE *fp_read, unsigned char frame[], unsigned char &frametype, unsigned char &timestamp)
{
	// Frame checking values
	bool validFrame = false;
	unsigned char buffer[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static int charsInBuffer = 0;
	int charsRead = 0;
	int SOFindex = 0;
	
	// Keeps scanning the port until a valid frame is received
	while(!validFrame)
	{
		// keep reading from the port until a SOF is received in the first spot
		while(buffer[0] != SOF)		
		{
			buffer[0] = fgetc(fp_read);	// tries to grab a character; returns EOF immediately if it can't find anything
			usleep(10);
		}
		
		// on success, indicate there's one character in the buffer
		charsInBuffer = 1;
		
		// kicks out of this loop when 1) a valid frame is received, or 2) a new SOF character needs to be found
		while(charsInBuffer < 16 && charsInBuffer != 0)
		{
			charsRead = fread(&buffer[charsInBuffer], 1, 16 - charsInBuffer, fp_read);
			charsInBuffer += charsRead;
			
			// if EOFm's in the right spot and the "command" value is valid,
			// assume all characters are there and everything's valid.
			if(buffer[15] == EOFm && 
				( (buffer[14] == IMUFRAME) || (buffer[14] == STATUSFRAME) || 
				(buffer[14] == ECHOFRAME) ||  (buffer[14] == VERSIONFRAME) ))
			{
				validFrame = true;

				break;
			}
			// otherwise, if a full frame isn't intact, try to scootch the next SOF character to the front
			else
			{
				// scan for the last SOF character
				SOFindex = 0;
				for(int i = 0; i < charsInBuffer; i++)
				{
					if(buffer[i] == SOF)
						SOFindex = i;
				}
				
				// if a new SOF char was not found, start the "while(!validFrame)" loop over again
				if(SOFindex == 0)
				{
					charsInBuffer = 0;
					buffer[0] = 0;
				}
				// if one was found, scootch it al down and keep track of how many chars were lost
				else 
				{
					charsInBuffer -= SOFindex;
					for(int i = 0; (i + SOFindex) < 16; i++)
						buffer[i] = buffer[i + SOFindex];
				}
			}
		}
		
		
	}
	
	
	/****************************************************
		PROCESSING A VALID FRAME
	****************************************************/
	
	timestamp = buffer[1];
	frametype = buffer[14];
	for(int i = 0; i < 12; i++)
		frame[i] = buffer[i+2];
}


void processOldImuFrame(double frame[], double data_array[])
{
	int roll_vel, pitch_vel, yaw_vel;
	
	roll_vel  = (int)(((unsigned int)frame[ 0] << 8*3) + ((unsigned int)frame[ 1] << 8*2) 
		+ ((unsigned int)frame[ 2] << 8*1) + ((unsigned int)frame[ 3]));
	pitch_vel = (int)(((unsigned int)frame[ 4] << 8*3) + ((unsigned int)frame[ 5] << 8*2) 
		+ ((unsigned int)frame[ 6] << 8*1) + ((unsigned int)frame[ 7]));
	yaw_vel   = (int)(((unsigned int)frame[ 8] << 8*3) + ((unsigned int)frame[ 9] << 8*2) 
		+ ((unsigned int)frame[10] << 8*1) + ((unsigned int)frame[11]));
		
	pitch_vel = -pitch_vel;
	yaw_vel   = -yaw_vel;
	
	data_array[0] = 0.0154*deg2rad*roll_vel;
	data_array[1] = 0.0154*deg2rad*pitch_vel;
	data_array[2] = 0.0154*deg2rad*yaw_vel;
}


void processImuFrame(unsigned char frame[], double data_array[])
{
	// temporary velocity values
	short x_acc, y_acc, z_acc;			// translational
	short roll_vel, pitch_vel, yaw_vel; 	// rotational

	roll_vel  = (short)( ((unsigned int)frame[0] << 8) | (unsigned int)frame[1] );
	pitch_vel = (short)( ((unsigned int)frame[2] << 8) | (unsigned int)frame[3] );
	yaw_vel   = (short)( ((unsigned int)frame[4] << 8) | (unsigned int)frame[5] );
	x_acc     = (short)( ((unsigned int)frame[6] << 8) | (unsigned int)frame[7] );
	y_acc     = (short)( ((unsigned int)frame[8] << 8) | (unsigned int)frame[9] );
	z_acc     = (short)(((unsigned int)frame[10] << 8) | (unsigned int)frame[11]);
	
	// AscTec uses a Z-down orientation, while we use a Z-up orientation with X
	// in the same direction; this requires changing the sign of the Y and Z
	// (y_vel, z_vel, pitch_vel, and yaw_vel) rates
	pitch_vel = -pitch_vel;
	yaw_vel   = -yaw_vel;
	y_acc = -y_acc;
	z_acc = -z_acc;
	
	// AscTec definition of angular velocity integer representation: 0.0154 degree/s, bias free
	data_array[WXi] = 0.0154*deg2rad*roll_vel;
	data_array[WYi] = 0.0154*deg2rad*pitch_vel;
	data_array[WZi] = 0.0154*deg2rad*yaw_vel;

	// AscTec definition of acceleration integer representation: -10000..+10000 = -1g..+1g
	data_array[AXi] = G*x_acc/10000.0;
	data_array[AYi] = G*y_acc/10000.0;
	data_array[AZi] = G*z_acc/10000.0;
}


// Status data from the AscTec SDK:
/*
 * 0x01 ATTITUDE CONTROL
 * 0x02 HEIGHT CONTROL
 * 0x04 POSITION CONTROL
 * 0x10 COMPASS FAILURE
 * 0x20 SERIAL INTERFACE ENABLED
 * 0x40 SERIAL INTERFACE ACTIVE //is active when control commands are sent to the LL
 * 0x80 EMERGENCY MODE //when RC link is lost -> serial interface disabled
 * 0x100 CALIBRATION ERROR
 * 0x200 GYRO CALIBRATION ERROR
 * 0x400 ACC CALIBRATION ERROR
 * 0x4000 MAGNETIC FIELD STRENGTH ERROR
 * 0x8000 MAGNETIC INCLINATION ERROR
 *
 * example: 0x07 means GPS Mode activated (ATTITUDE-, HEIGHT- and POSITION CONTROL active)
 */
void processStatusFrame(unsigned char frame[], short &status, double &battery_voltage, double &cpu_load, double motor_rpms[])
{
		
	status = ((unsigned short)frame[0] << 8) | (unsigned short)frame[1];
	battery_voltage = ( ((unsigned int)frame[2] << 8) | ((unsigned int)frame[3]) )/1000.0;

	// from AscTec SDK code:
	// load = "timer cycles" / "timer cycles per controller cycle" * 1000
	// dividing by 1000, then multiplying by 100%, this yields a division by 10.
	cpu_load = ( ((unsigned int)frame[4] << 8) | ((unsigned int)frame[5]) )/10.0;
	
	// Swaps from AscTec motor labeling convention
	motor_rpms[0] = 64*frame[6];
	motor_rpms[1] = 64*frame[8];
	motor_rpms[2] = 64*frame[7];
	motor_rpms[3] = 64*frame[9];
}



void processEchoFrame(unsigned char frame[], char text[])
{
	for(int i = 0; i < 5; i++)
		text[i] = frame[i];
}


void processVersionFrame(unsigned char frame[], char text[])
{
	for(int i = 0; i < 12; i++)
		text[i] = frame[i];
}
