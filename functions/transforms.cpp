#include "transforms.h"
#include "../defs.h"

#include <math.h>

// A function to guarantee the result of an angular subtraction is always somewhere between +/- pi.
double minAngle(double angle)
{
	return atan2(sin(angle),cos(angle));
}


/*
	The quaternion rotation uses unit quaternion multiplications to rotate a
	3x1 input vector to a 3x1 output vector as shown below
	
	[ 0; out] = quat * [0; in] * quat^-1

	which collapses to the following rotation matrix form

	out = R * in

	The resulting matrix for R can be seen below. It's a bit chaotic.
	 
[ q0^2 + qi^2 - qj^2 - qk^2,         2*(qi*qj - q0*qk),         2*(qi*qk + q0*qj)]
[         2*(qi*qj + q0*qk), q0^2 - qi^2 + qj^2 - qk^2,         2*(qj*qk - q0*qi)]
[         2*(qi*qk - q0*qj),         2*(qj*qk + q0*qi), q0^2 - qi^2 - qj^2 + qk^2]

	This process works for rotating position vectors (and their derivatives),
	as well as angular velocity vectors; hence, the generic "in" and "out" vectors.
*/
void quatRot(double in[], double out[], double quat[])
{
	// Extracts the quaternion from the array; assumes the quaternion is passed
	// in with the following form: [ q0 qi qj qk ]' 
	double q0 = quat[0], qi = quat[1], qj = quat[2], qk = quat[3];


	out[0] = ( q0*q0 + qi*qi - qj*qj - qk*qk ) * in[0]  
		   +               2*( qi*qj - q0*qk ) * in[1]  
		   +               2*( qi*qk + q0*qj ) * in[2];
	out[1] =               2*( qi*qj + q0*qk ) * in[0]  
		   + ( q0*q0 - qi*qi + qj*qj - qk*qk ) * in[1] 
		   +               2*( qj*qk - q0*qi ) * in[2];
	out[2] =               2*( qi*qk - q0*qj ) * in[0]  
		   +               2*( qj*qk + q0*qi ) * in[1]  
		   + ( q0*q0 - qi*qi - qj*qj + qk*qk ) * in[2];
}


// This function performs the inverse of the calculation performed above.
void quatRotInv(double in[], double out[], double quat[])
{
	// Extracts the quaternion from the array; assumes the quaternion is passed
	// in with the following form: [ q0 qi qj qk ]' 
	double q0 = quat[0], qi = quat[1], qj = quat[2], qk = quat[3];

	// Performs the rotation
	out[0] = ( q0*q0 + qi*qi - qj*qj - qk*qk ) * in[0]  
		   +               2*( qi*qj + q0*qk ) * in[1]  
		   +               2*( qi*qk - q0*qj ) * in[2];
	out[1] =               2*( qi*qj - q0*qk ) * in[0]  
		   + ( q0*q0 - qi*qi + qj*qj - qk*qk ) * in[1] 
		   +               2*( qj*qk + q0*qi ) * in[2];
	out[2] =               2*( qi*qk + q0*qj ) * in[0]  
		   +               2*( qj*qk - q0*qi ) * in[1]  
		   + ( q0*q0 - qi*qi - qj*qj + qk*qk ) * in[2];
}


// A function for converting a quaternion into Euler ZYX angles. 
// Euler ZYX standard: X^world = R_z(psi) * R_y(theta) * R_x(phi) * X^body
// quaternion form: [q0 qi qj qk]'
// Euler ZYX form: [phi theta psi]'
//
// Source for the conversions:
// http://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf

void quat2EulerZYX(double quat[], double euler[])
{
	// Extracts the quaternion from the array 
	double q0 = quat[0], qi = quat[1], qj = quat[2], qk = quat[3];

	// Calculates the corresponding Euler angles
	euler[0] = atan2( 2*(qj*qk + q0*qi), 1 - 2*(qi*qi + qj*qj) ); // phi
	double num = -2*(qi*qk - q0*qj); // value used in calculating theta
	euler[1] = atan2( num, sqrt( 1 - num*num ) ); // theta
	euler[2] = atan2( 2*(qi*qj + q0*qk), 1 - 2*(qj*qj + qk*qk) ); // psi
}


// A function for converting Euler ZYX angles into a quaternion. 
// Euler ZYX standard: X^world = R_z(psi) * R_y(theta) * R_x(phi) * X^body
// quaternion form: [q0 qi qj qk]'
// Euler ZYX form: [phi theta psi]'
//
// Source for the conversions:
// http://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf

void eulerZYX2Quat(double euler[], double quat[])
{
	// Extracts angles from the array 
	double   phi_d2 = euler[0]/2;
	double theta_d2 = euler[1]/2;
	double   psi_d2 = euler[2]/2;

	// Calculates the corresponding quaternion values
	quat[0] =   sin(phi_d2)*sin(theta_d2)*sin(psi_d2)
			  + cos(phi_d2)*cos(theta_d2)*cos(psi_d2); // q0

	quat[1] = - cos(phi_d2)*sin(theta_d2)*sin(psi_d2)
			  + sin(phi_d2)*cos(theta_d2)*cos(psi_d2); // qi

	quat[2] =   sin(phi_d2)*cos(theta_d2)*sin(psi_d2)
			  + cos(phi_d2)*sin(theta_d2)*cos(psi_d2); // qj

	quat[3] =   cos(phi_d2)*cos(theta_d2)*sin(psi_d2)
			  - sin(phi_d2)*sin(theta_d2)*cos(psi_d2); // qk
}


// Receives a quaternion vector (quat) and returns a yaw angle corresponding to
// a coordinate system where the yaw rotation is the first rotation executed on
// Body frame to transition it to World frame (such as Euler XYZ or Euler YXZ).
double quat2Yaw(double quat[])
{
	// Extracts the individual quaternion terms for clarity
	double q0 = quat[0], qi = quat[1], qj = quat[2], qk = quat[3];

	// Calculates and returns the yaw angle
	double psi = atan2( -2*(qi*qj - q0*qk), (q0*q0 + qi*qi - qj*qj - qk*qk) );
	return psi;
}


// Receives a set of thrust and torques (Uq), the quadrotor arm length (Lq), 
// the propeller thrust coefficient (Bp), and the propeller drag coefficient (Kp). 
// From there, it calculates the motor speeds (omega) required to execute the desired input.
void inputsToMotorSpeeds(double Uq[], double omega[], double Lq, double Bp, double Kp)
{
	// Buffer variable for the square of the motor speeds
	double omegaSquared[4];
	

	// "defs.cpp" keeps track of the indices for Thrust, pitch/X torque, roll/Y torque, yaw/Z torque,
	// and the front, back, left, and right motors. 
	omegaSquared[Mfront] = (Uq[Th]/(4*Bp)) - (Uq[Ty]/(2*Bp*Lq)) + (Uq[Tz]/(4*Kp));
	omegaSquared[Mback]  = (Uq[Th]/(4*Bp)) + (Uq[Ty]/(2*Bp*Lq)) + (Uq[Tz]/(4*Kp));
	omegaSquared[Mleft]  = (Uq[Th]/(4*Bp)) + (Uq[Tx]/(2*Bp*Lq)) - (Uq[Tz]/(4*Kp));
	omegaSquared[Mright] = (Uq[Th]/(4*Bp)) - (Uq[Tx]/(2*Bp*Lq)) - (Uq[Tz]/(4*Kp));
	
	// Keep squared speeds in limited range, then take the square root.
	for(int i = 0; i < 4; i++)
	{
		if(omegaSquared[i] < omega_sq_min)
			omegaSquared[i] = omega_sq_min;
		else if(omegaSquared[i] > omega_sq_max)
			omegaSquared[i] = omega_sq_max;
			
		omega[i] = sqrt(omegaSquared[i]);
	}
}


// Receives a set of motor speeds (omega), the quadrotor arm length (Lq), 
// the propeller thrust coefficient (Bp), and the propeller drag coefficient (Kp). 
// From there, it calculates the resulting thrust and torques inputs (Uq).
void motorSpeedsToInputs(double omega[], double Uq[], double Lq, double Bp, double Kp)
{
	// Buffer variable for the square of the motor speeds
	double omegaSquared[4];
	
	for(int i = 0; i < 4; i++)
		omegaSquared[i] = omega[i]*omega[i];
		
	// "defs.cpp" keeps track of the indices for Thrust, pitch/X torque, roll/Y torque, yaw/Z torque,
	// and the front, back, left, and right motors. 
	Uq[Th] = Bp*   (omegaSquared[Mfront] + omegaSquared[Mleft] + omegaSquared[Mback] + omegaSquared[Mright]);
	Uq[Tx] = Bp*Lq*(omegaSquared[Mleft]  - omegaSquared[Mright]);
	Uq[Ty] = Bp*Lq*(omegaSquared[Mback]  - omegaSquared[Mfront]);
	Uq[Tz] = Kp*   (omegaSquared[Mfront] - omegaSquared[Mleft] + omegaSquared[Mback] - omegaSquared[Mright]);
}
