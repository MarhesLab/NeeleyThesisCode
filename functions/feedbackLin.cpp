#include <math.h>

#include "feedbackLin.h"
#include "transforms.h" // quaternion to Euler angles


// This set of calculations is quite dense. For an idea of what's even going on here,
// read these two papers first:
//	- O. Fritsch, P. De Monte, M. Buhl, and B. Lohmann, 
//		"Quasi-static feedback linearization for the translational dynamics 
//		 of a quadrotor helicopter"
// 	- M. Buhl and B. Lohmann, 
//		"Control with exponentially decaying Lyapunov functions and its use 
//		 for systems with input saturation"

#ifdef DO_INTEGRAL
	double intSatVal = 1.0;
#endif


// DEBUG "can" be defined in "feedbackLin.h", so this section
// must come after "feedbackLin.h" is included
#ifdef DEBUG
	#include <iostream>
	#include <iomanip>

	using namespace std;
#endif




// Populates important elements of the struct
void initializeStruct(struct FeedbackLinStruct &f, int quadNum)
{
	// Sets general parameters (from "../defs.cpp")
	f.Ts = Ts;
	f.g  = G;

	// Sets quadrotor parameters (from "../quad/parameters.cpp")
	f.Mq  = Mq_meas[quadNum];
	f.Jqx = Jqx_meas[quadNum];
	f.Jqy = Jqy_meas[quadNum];
	f.Jqz = Jqz_meas[quadNum];

	// Zeros out the various arrays
	for(int i = 0; i < POS_REFS; i++) {
		f.XqPos[i]         = 0;
		f.XqPosError[i]    = 0;
		f.XqPosErrorInt[i] = 0;
	}
}


// Divides the quaternion into two separate quaternions: one for XY rotation,
// and one for Z rotation
void splitQuaternion(double Xq[], struct FeedbackLinStruct &f)
{	
	// Qp = sqrt( Q0^2 + Qk^2 )
	f.qp = sqrt(Xq[Q0]*Xq[Q0] + Xq[QK]*Xq[QK]);
	
	// Qw = Q0/Qp 
	// Qz = Qk/Qp
	f.qw = Xq[Q0]/f.qp;
	f.qz = Xq[QK]/f.qp;
	
	// Qx = Qw*Qi - Qz*Qj
	// Qy = Qz*Qi + Qw*Qj
	f.qx = ( f.qw * Xq[QI] ) - ( f.qz * Xq[QJ] );
	f.qy = ( f.qz * Xq[QI] ) + ( f.qw * Xq[QJ] );
}


// Converts the Body frame angular velocities to auxiliary frame angular velocities
void getAuxAngVels(double Xq[], struct FeedbackLinStruct &f)
{
	double qw = f.qw, qz = f.qz;
	
	// Wxa = (Qw^2 - Qz^2)*Wx - 2*Qw*Qz*Wy
	// Wya = 2*Qw*Qz*Wx + (Qw^2 - Qz^2)*Wy
	// Wza = Wz
	f.wxa = ( qw*qw - qz*qz )*Xq[WX] -           2*qw*qz*Xq[WY];
	f.wya =           2*qw*qz*Xq[WX] + ( qw*qw - qz*qz )*Xq[WY];
	f.wza =                   Xq[WZ];
}


// Calculates the derivatives of the XY quaternion
void getSplitQuatDerivs(struct FeedbackLinStruct &f)
{
	// dQx = -0.5*( (Qx^2 - 1)*Wxa + Qx*Qy*Wya )/Qp
	// dQy = -0.5*( Qx*Qy*Qxa + (Qy^2 - 1)*Wya )/Qp
	// dQp = -1.0*( Qx*dQx + Qy*dQy)/Qp
	f.dqx = -0.5*( (f.qx*f.qx - 1) * f.wxa 
	             + (f.qx*f.qy)     * f.wya )/f.qp;
	f.dqy = -0.5*( (f.qx*f.qy)     * f.wxa
	             + (f.qy*f.qy - 1) * f.wya )/f.qp;
	f.dqp = -1.0*( f.dqx*f.qx + f.dqy*f.qy )/f.qp;
}


// Calculates second derivatives of position
void getSecondDerivs(double Xq[], double XqTraj[],  struct FeedbackLinStruct &f)
{
	double vz;

	// Populates position and velocity values
	f.XqPos[ X_t] = Xq[ X];
	f.XqPos[ Y_t] = Xq[ Y];
	f.XqPos[ Z_t] = Xq[ Z];
	f.XqPos[DX_t] = Xq[DX];
	f.XqPos[DY_t] = Xq[DY];
	f.XqPos[DZ_t] = Xq[DZ];
	
	// Calculates position and velocity errors
	f.XqPosError[ X_t] = XqTraj[ X_t] - f.XqPos[ X_t];
	f.XqPosError[ Y_t] = XqTraj[ Y_t] - f.XqPos[ Y_t];
	f.XqPosError[ Z_t] = XqTraj[ Z_t] - f.XqPos[ Z_t];
	f.XqPosError[DX_t] = XqTraj[DX_t] - f.XqPos[DX_t];
	f.XqPosError[DY_t] = XqTraj[DY_t] - f.XqPos[DY_t];
	f.XqPosError[DZ_t] = XqTraj[DZ_t] - f.XqPos[DZ_t];
	
	// If applicable calculates and saturates the integral terms
	#ifdef DO_INTEGRAL
		f.XqPosErrorInt[X_t] = f.XqPosErrorInt[X_t] + f.Ts*f.XqPosError[X_t];
		f.XqPosErrorInt[Y_t] = f.XqPosErrorInt[Y_t] + f.Ts*f.XqPosError[Y_t];
		f.XqPosErrorInt[Z_t] = f.XqPosErrorInt[Z_t] + f.Ts*f.XqPosError[Z_t];

		if(f.XqPosErrorInt[X_t] >  intSatVal)
			f.XqPosErrorInt[X_t] = intSatVal;
		if(f.XqPosErrorInt[X_t] < -intSatVal)
			f.XqPosErrorInt[X_t] = -intSatVal;

		if(f.XqPosErrorInt[Y_t] >  intSatVal)
			f.XqPosErrorInt[Y_t] = intSatVal;
		if(f.XqPosErrorInt[Y_t] < -intSatVal)
			f.XqPosErrorInt[Y_t] = -intSatVal;

		if(f.XqPosErrorInt[Z_t] >  intSatVal)
			f.XqPosErrorInt[Z_t] = intSatVal;
		if(f.XqPosErrorInt[Z_t] < -intSatVal)
			f.XqPosErrorInt[Z_t] = -intSatVal;
	#endif 
	
	// calculates gamma for use in the thrust calculation
	// gamma = 1 - 2*Qx^2 - 2*Qy^2
	f.gamma = 1 - 2*f.qx*f.qx - 2*f.qy*f.qy;
	f.gamma_inv = 1/f.gamma;
	
	// calculates the "input" vz for calculating thrust
	// vz = ddz_d + a1*( dz_d - dz ) + a0*( z_d - z)
	// optionally, + a_int*int_z_error
	vz = XqTraj[DDZ_t] + gains_z[2]*f.XqPosError[DZ_t] + gains_z[1]*f.XqPosError[Z_t];
	#ifdef DO_INTEGRAL
		vz = vz + gains_z[0]*f.XqPosErrorInt[Z_t];
	#endif
	
	// calculates the thrust value 
	// T = (m/gamma)*(g + vz)
	double T_div_m = (f.g + vz)*f.gamma_inv;
	f.T = f.Mq*T_div_m;
	
	// calculates second derivatives of position
	// ddX = (T/Mq)*( 2*Qp*Qy)
	// ddY = (T/Mq)*(-2*Qp*Qx)
	// ddZ = vz
	f.XqPos[DDX_t] = T_div_m * ( 2*f.qp*f.qy );
	f.XqPos[DDY_t] = T_div_m * (-2*f.qp*f.qx );
	f.XqPos[DDZ_t] = vz;
}


// Calculates third derivatives of position
void getThirdDerivs(double Xq[], double XqTraj[], struct FeedbackLinStruct &f)
{
	double dvz;

	// Calculates acceleration errors
	f.XqPosError[DDX_t] = XqTraj[DDX_t] - f.XqPos[DDX_t];
	f.XqPosError[DDY_t] = XqTraj[DDY_t] - f.XqPos[DDY_t];
	f.XqPosError[DDZ_t] = XqTraj[DDZ_t] - f.XqPos[DDZ_t];
	
	// calculates the derivative of the "input" vz for calculating thrust
	// dvz = dddz_d + a1*( ddz_d - ddz ) + a0*( dz_d - dz )
	// optionally, + a_int*( z_d - z )
	dvz = XqTraj[DDDZ_t] + gains_z[2]*f.XqPosError[DDZ_t] + gains_z[1]*f.XqPosError[DZ_t];
	#ifdef DO_INTEGRAL
		dvz = dvz + gains_z[0]*f.XqPosError[Z_t];
	#endif
	
	
	// calculates the derivative of thrust
	// dT = ( T * (4*dQx*Qx + 4*dQy*Qy) + dvz*Mq )/gamma
	f.dT = ( ( 4*f.dqx*f.qx + 4*f.dqy*f.qy )*f.T + (f.Mq)*dvz )*f.gamma_inv;
	
	// calcualtes third derivatives of position
	// dddX =    (2*T/m)*(Qp*dQy + Qy*dQp) + (dT/m)*(2*Qp*Qy)
	// dddY = -1*(2*T/m)*(Qp*dQx + Qx*dQp) - (dT/m)*(2*Qp*Qx)
	// dddZ = -2*(2*T/m)*(Qx*dQx + Qy*dQy) + (dT/m)*gamma
	double T_2_div_m = 2*f.T/f.Mq;
	double  dT_div_m = f.dT/f.Mq;
	f.XqPos[DDDX_t] =    T_2_div_m*(f.qp*f.dqy + f.qy*f.dqp)
			           +  dT_div_m*(2*f.qp*f.qy);
	f.XqPos[DDDY_t] = -1*T_2_div_m*(f.qp*f.dqx + f.qx*f.dqp)
		               -  dT_div_m*(2*f.qp*f.qx);
	f.XqPos[DDDZ_t] = -2*T_2_div_m*(f.qx*f.dqx + f.qy*f.dqy)
			           +  dT_div_m*(f.gamma);
}


// Gets the pseudo-inputs ddQx and ddQy
void getQuatAccels(double Xq[], double XqTraj[],  struct FeedbackLinStruct &f)
{
	double vx, vy, ddvz;
	
	// Calculates jerk errors
	f.XqPosError[DDDX_t] = XqTraj[DDDX_t] - f.XqPos[DDDX_t];
	f.XqPosError[DDDY_t] = XqTraj[DDDY_t] - f.XqPos[DDDY_t];
	f.XqPosError[DDDZ_t] = XqTraj[DDDZ_t] - f.XqPos[DDDZ_t];
	
	
	// calculates feedback linearization "inputs"
	  vx =            XqTraj[DDDDX_t]
		+ gains_xy[4]*f.XqPosError[DDDX_t]
		+ gains_xy[3]*f.XqPosError[ DDX_t]
		+ gains_xy[2]*f.XqPosError[  DX_t]
		+ gains_xy[1]*f.XqPosError[   X_t];
	  vy =            XqTraj[DDDDY_t]
		+ gains_xy[4]*f.XqPosError[DDDY_t]
		+ gains_xy[3]*f.XqPosError[ DDY_t]
		+ gains_xy[2]*f.XqPosError[  DY_t]
		+ gains_xy[1]*f.XqPosError[   Y_t];
	ddvz =            XqTraj[DDDDZ_t]
		+  gains_z[2]*f.XqPosError[DDDZ_t]
		+  gains_z[1]*f.XqPosError[ DDZ_t];
	#ifdef DO_INTEGRAL
		  vx =   vx + gains_xy[0]*f.XqPosErrorInt[X_t];
		  vy =   vy + gains_xy[0]*f.XqPosErrorInt[Y_t];
		ddvz = ddvz +  gains_z[0]*f.XqPosError[DZ_t];
	#endif
	
	// Splits the calculations up based on the following Lie derivative-based
	// calculation: 
	// [ddqx ddqy]' = (LgLf^3)^-1 * (-Lf^4 + [vx vy]')    2-dim vector
	// Lf^4 = ddvz*H + (4*dT/m)*N + (2*T/m)*M             2-dim vector 
	// LgLf^3 = (2*T/m)*O								  2x2 matrix
	double H[2], N[2], M[2], Lf_4[2], O_inv[2][2];
	double  qp = f.qp,   qx = f.qx,   qy = f.qy; // shortens the following equations
	double dqp = f.dqp, dqx = f.dqx, dqy = f.dqy;
	double g_inv = f.gamma_inv;
	double qp_inv = 1/qp;
	double m_inv = 1/f.Mq;
	
	// Starts by calculating the values for H
	// H = [ 2*Qp*Qy/gamma   
	//      -2*Qp*Qx/gamma ]
	H[0] = ( 2*qp*qy)*g_inv;
	H[1] = (-2*qp*qx)*g_inv;
	
	// Moves on to the values for N
	// N = [  Qy*dQp + (4*Qp*Qx*Qy/gamma)*dQx + Qp*( (4*Qy^2/gamma) + 1)*dQy
	//       -Qx*dQp - Qp*( (4*Qx^2/gamma) + 1)*dQx - (4*Qp*Qx*Qy/gamma)*dQy ]
	N[0] =  qy*dqp + (4*qp*qx*qy*g_inv)*dqx + qp*( (4*qy*qy*g_inv) + 1)*dqy;
	N[1] = -qx*dqp - qp*( (4*qx*qx*g_inv) + 1)*dqx - (4*qp*qx*qy*g_inv)*dqy;
	
	// Moves on to the values for M
	// M = [ ( dQy - dQp*Qy/Qp )*dQp + ( 4*dQx*Qp*Qy/gamma - dQx*Qy/Qp )*dQx ...
	//       		+ ( dQp - dQy*Qy/Qp + 4*Qp*Qy*dQy/gamma )*dQy
	//       (-dQx + dQp*Qy/Qp )*dQp + ( dQx*Qx/Qp - dQp - 4*dQx*Qp*Qx/gamma)*dQx ...
	//              + ( dQy*Qx/Qp - 4*Qp*Qx*dQy/gamma )*dQy ]
	M[0] = ( dqy - dqp*qy*qp_inv )*dqp + ( 4*dqx*qp*qy*g_inv - dqx*qy*qp_inv )*dqx
	     + ( dqp - dqy*qy*qp_inv + 4*qp*qy*dqy*g_inv )*dqy;
	M[1] = (-dqx + dqp*qy*qp_inv )*dqp + ( dqx*qx*qp_inv - dqp - 4*dqx*qp*qx*g_inv )*dqx
	     + ( dqy*qx*qp_inv - 4*qp*qx*dqy*g_inv )*dqy;
		 
	// Calculates the Lie derivative Lf^4
	Lf_4[0] = ddvz*H[0] + (4*f.dT*m_inv)*N[0] + (2*f.T*m_inv)*M[0];
	Lf_4[1] = ddvz*H[1] + (4*f.dT*m_inv)*N[1] + (2*f.T*m_inv)*M[1];
	
	// Calculates the inverse of O
	// O_inv = [ ( Qx*Qy*(gamma - 4*Qp^2) )            ( -4*Qp^2*Qy^2 - gamma*(Qp^2 + Qy^2) )
	//           ( 4*Qp^2*Qx^2 + gamma*(Qp*2 - Qx^2) ) ( -Qx*Qy*(gamma - 4*Qp^2) ) ]
	// all divided by ( qp*(4*Qp^2*(1 - Qp^2) + gamma^2) )
	// for LgLf^3, don't forget to multiply by (m/(2*T))
	double denom_inv = 1 / ( qp * (4*qp*qp*(1 - qp*qp) + f.gamma*f.gamma) );
	O_inv[0][0] = (  qx*qy*(f.gamma - 4*qp*qp) ) * denom_inv;
	O_inv[0][1] = ( -4*qp*qp*qy*qy - f.gamma*(qp*qp + qy*qy) ) * denom_inv;
	O_inv[1][0] = (  4*qp*qp*qx*qx + f.gamma*(qp*qp - qx*qx) ) * denom_inv;
	O_inv[1][1] = ( -qx*qy*(f.gamma - 4*qp*qp) ) * denom_inv;
	
	// Finally calculates ddqx and ddqy!
	double m_div_2_T = f.Mq/(2*f.T);
	f.ddqx = m_div_2_T * ( O_inv[0][0]*( -Lf_4[0] + vx ) + O_inv[0][1]*( -Lf_4[1] + vy) );
	f.ddqy = m_div_2_T * ( O_inv[1][0]*( -Lf_4[0] + vx ) + O_inv[1][1]*( -Lf_4[1] + vy) );

	// Debug outputs to verify things are being calculated correctly
	#ifdef DEBUG
		cout << setprecision(6)
			<< "H values:    \t" <<    H[0] << "\t" <<    H[1] << "\n\n\r";

		cout << setprecision(6)
			<< "N values:    \t" <<    N[0] << "\t" <<    N[1] << "\n\n\r";

		cout << setprecision(6)
			<< "M values:    \t" <<    M[0] << "\t" <<    M[1] << "\n\n\r";

		cout << setprecision(6)
			<< "Lf_4 values: \t" << Lf_4[0] << "\t" << Lf_4[1] << "\n\n\r";

		cout << setprecision(6)
			<< "Oinv values: \t" << O_inv[0][0] << "\t" << O_inv[0][1] << "\n\r"
			<< "             \t" << O_inv[1][0] << "\t" << O_inv[1][1] << "\n\n\r";
	#endif
}


// Converts the desired ddqx and ddqy into the appropriate dWaux
void getWauxDots(struct FeedbackLinStruct &f)
{
	// [dwxa dwya]' = qp*(Z)^-1 * ( [ddqx ddqy]' 
	//              - (1/qp)*dZ*[wxa wya]' - (dqp/qp^2)*Z*[wxa wya]' )
	// where Z = 0.5*[ ( 1 - qx^2 )  (-qx*qy)
	//                 (-qx*qy)  (1 - qy^2) ]
	
	// Breaks it apart
	double Z[2][2], Zinv[2][2], Zdot[2][2];
	double right_1, right_2;
	double qp = f.qp, qx = f.qx, qy = f.qy;
	double qp_inv = 1/qp;
	double qp_inv_4 = qp_inv/4;

	// Calculates Z first
	Z[0][0] = (1 - qx*qx);
	Z[0][1] = (-qx*qy);
	Z[1][0] = Z[0][1];
	Z[1][1] = (1 - qy*qy);
	
	// Calculates Z inverse
	double qp_inv_sq_2 = 2*qp_inv*qp_inv;
	Zinv[0][0] =  qp_inv_sq_2*Z[1][1];
	Zinv[0][1] = -qp_inv_sq_2*Z[0][1];
	Zinv[1][0] = Zinv[0][1];
	Zinv[1][1] =  qp_inv_sq_2*Z[0][0];

	// Calculates Zdot next
	Zdot[0][0] = qp_inv_4*(2*qx*(qx*qx - 1)*f.wxa +       2*qx*qx*qy*f.wya);
	Zdot[0][1] = qp_inv_4*(qy*(2*qx*qx - 1)*f.wxa + qx*(2*qy*qy - 1)*f.wya);
	Zdot[1][0] = Zdot[0][1];
	Zdot[1][1] = qp_inv_4*(      2*qx*qy*qy*f.wxa + 2*qy*(qy*qy - 1)*f.wya);

	// Calculates the right side in parentheses
	right_1 = f.ddqx - qp_inv*(    Zdot[0][0]*f.wxa + Zdot[0][1]*f.wya )
	      - (qp_inv*qp_inv*f.dqp/2)*( Z[0][0]*f.wxa +    Z[0][1]*f.wya );
	right_2 = f.ddqy - qp_inv*(    Zdot[1][0]*f.wxa + Zdot[1][1]*f.wya )
	      - (qp_inv*qp_inv*f.dqp/2)*( Z[1][0]*f.wxa +    Z[1][1]*f.wya );
		  
	// Calculates the final answer
	f.dwxa = qp*( Zinv[0][0]*right_1 + Zinv[0][1]*right_2 );
	f.dwya = qp*( Zinv[1][0]*right_1 + Zinv[1][1]*right_2 );
}


// Converts the desired dWaux to the desired dW
void getWxyDots(double Xq[], struct FeedbackLinStruct &f)
{
	// [dwx dwy]' = Rqz^-1 ([dwxa dwya]' - wz*Rqzdot*[wx wy]' ),
	// where Rqz = [ (Qw^2 - Qz^2)   (-2*Qw*Qz)
	//               ( 2*Qw*Qz)   (Qw^2 - Qz^2) ]
	double Rqz_dot[2][2], Rqz_inv[2][2]; 
	double interior_1, interior_2;
	double qw = f.qw, qz = f.qz;
	
	// Calculates the time derivative of Rqz
	Rqz_dot[0][0] = -2*qw*qz;
	Rqz_dot[0][1] = qz*qz - qw*qw;
	Rqz_dot[1][0] = qw*qw - qz*qz;
	Rqz_dot[1][1] = Rqz_dot[0][0];
	
	// Calculates the inverse of Rqz
	Rqz_inv[0][0] = qw*qw - qz*qz;
	Rqz_inv[0][1] = 2*qw*qz;
	Rqz_inv[1][0] = -Rqz_inv[0][1];
	Rqz_inv[1][1] = qw*qw - qz*qz;
	
	// Calculates the component inside parentheses
	// Wza = Wz
	interior_1 = f.dwxa - f.wza*( Rqz_dot[0][0]*Xq[WX] + Rqz_dot[0][1]*Xq[WY] );
	interior_2 = f.dwya - f.wza*( Rqz_dot[1][0]*Xq[WX] + Rqz_dot[1][1]*Xq[WY] );
	
	// Finally calculates the appropriate values
	f.dwx = Rqz_inv[0][0]*interior_1 + Rqz_inv[0][1]*interior_2;
	f.dwy = Rqz_inv[1][0]*interior_1 + Rqz_inv[1][1]*interior_2;
}


// Performs heading control to figure out the desired dWz
void headingControl(double XqTraj[], struct FeedbackLinStruct &f)
{
	// Calculates the Euler angle related to a rotation around Z.
	// While Euler XYZ or YXZ would more technically match the way
	// the quaternion was split, Qz has no X or Y rotations, so 
	// using Euler ZYX's calculation process works fine.
	double euler[3], Qz[4] = {f.qw, 0, 0, f.qz};
	quat2EulerZYX(Qz, euler);
	
	// Populates heading and angular velocity values
	f.XqPos[ PSI_t] = minAngle(euler[2]);
	f.XqPos[DPSI_t] = f.wza; // same as Xq[WZ]
	
	// Calculates heading errors
	f.XqPosError[ PSI_t] = minAngle(XqTraj[ PSI_t] - f.XqPos[ PSI_t]);
	f.XqPosError[DPSI_t] =          XqTraj[DPSI_t] - f.XqPos[DPSI_t];	
	#ifdef DO_INTEGRAL
		f.XqPosErrorInt[PSI_t] = f.XqPosErrorInt[PSI_t] + f.Ts*f.XqPosError[PSI_t];
	#endif 
	
	// Calculates the desired Z angular velocity
	f.dwz =                  XqTraj[DDPSI_t]
		+ gains_psi[2]*f.XqPosError[ DPSI_t]
		+ gains_psi[1]*f.XqPosError[  PSI_t];
	#ifdef DO_INTEGRAL
		f.dwz = f.dwz + gains_psi[0]*f.XqPosErrorInt[PSI_t];
	#endif

	#ifdef DEBUG
		cout << setprecision(6)
			<< "Psi value:   " << f.XqPos[PSI_t] << "\n\r"
			<< "Errors: psi: " << f.XqPosError[PSI_t]
				<< "\tdpsi: "  << f.XqPosError[DPSI_t]
				<< "\tddpsi: " << XqTraj[DDPSI_t] << "\n\r";
	#endif
}


// Finally calculates the desired thrust and torques
void getTrueInputs(double Xq[], double Uq[], struct FeedbackLinStruct f)
{
	// Thrust was calculated earlier, which makes things easy
	Uq[Th] = f.T;
	
	// Calculates desired torques from Euler's equation of motion
	Uq[Tx] = f.Jqx*f.dwx + Xq[WY]*Xq[WZ]*(f.Jqz - f.Jqy);
    Uq[Ty] = f.Jqy*f.dwy + Xq[WX]*Xq[WZ]*(f.Jqx - f.Jqz);
    Uq[Tz] = f.Jqz*f.dwz + Xq[WX]*Xq[WY]*(f.Jqy - f.Jqx);
}
