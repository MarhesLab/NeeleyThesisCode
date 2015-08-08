#include "Filter3.h"

#include <iostream>
#include <math.h>

#include "transforms.h" // for angvelWorld2Body()
#include "../quad/parameters.h" // for obtaining mass and inertia values

using namespace std;

/*
	QUICK FILTERING PARAMETERS
*/

// Simplified C meshing matrix values; given as entries for a diagonal matrix,
// where all values are between 0 and 1.
// 0 = full trust of measurements
// 1 = full trust of predictions
// <0 or >1: ...what on Earth are you doing?
const double C_diag[STATES] = 
    { 0.25, 0.25, 0.25,        //  X,  Y,  Z
      0.50, 0.50, 0.50,        // dX, dY, dZ
      0.25, 0.25, 0.25, 0.25,   // Q0, QI, QJ, QK
      0.50, 0.50, 0.50};       // Wx, Wy, Wz
// Used to mesh finite differencing-based velocities with integrated-acceleration-based velocities.
// Sets the meshing percentage for the integration term.
const double alpha = 0.65;

/*
	KALMAN FILTERING PARAMETERS
*/

// Q and R matrix value definitions; given as entries for a diagonal matrix

// Initially configured for lack of trust in predictions
const double Q_diag[STATES] = 
	{ 1*pow(10,-3), 1*pow(10,-3), 1*pow(10,-3),                //   X,  Y,  Z
      1*pow(10,-3), 1*pow(10,-3), 1*pow(10,-3),                //  dX, dY, dZ
      1*pow(10,-3), 1*pow(10,-3), 1*pow(10,-3), 1*pow(10,-3),  //  Q0, QI, QJ, QK
      1*pow(10,-3), 1*pow(10,-3), 1*pow(10,-3)};               // Wx, Wy, Wz
// Obtained from measurement covariances while quadrotor was still
const double Rv_diag[VIC_STATES] = 
    { 1*pow(10,-6),   //   X 
      1*pow(10,-4),   //   Y
      1*pow(10,-6),   //   Z
      1*pow(10,-5),   //  Q0
      1*pow(10,-5),   //  QI
      1*pow(10,-5),   //  QJ
      1*pow(10,-5)};  //  QK

// Obtained from measurement covariances while quadrotor was still
const double Ri_diag[IMU_STATES] = 
    { 1*pow(10,-4),   // Wx
      1*pow(10,-4),   // Wy
      1*pow(10,-4),   // Wz
      1*pow(10,-3),   // ddX
      1*pow(10,-3),   // ddY
      1*pow(10,-3)};  // ddZ


Filter3::Filter3(double* Xk_in, double* Xkp1_in, double* Uk_in, 
	double* Xkp1_v_in, double* Xk_v_in, double* Xkp1_i_w_in, int quad_num):
Xkp1_est_mat(STATES,1,CV_64FC1,Xkp1_est),J(3,3,CV_64FC1),J_inv(3,3,CV_64FC1),
omega(3,1, CV_64FC1),
Ac(STATES,STATES,CV_64FC1),Ad(STATES,STATES,CV_64FC1),Ad_T(STATES,STATES,CV_64FC1),
Q(STATES,STATES,CV_64FC1),I(STATES,STATES,CV_64FC1),
P(STATES,STATES,CV_64FC1),Ppred(STATES,STATES,CV_64FC1),
Rv(VIC_STATES,VIC_STATES,CV_64FC1), 
Hv(VIC_STATES, STATES,CV_64FC1), Hv_T(STATES, VIC_STATES,CV_64FC1),
Ri(IMU_STATES,IMU_STATES,CV_64FC1), 
Hi(IMU_STATES, STATES,CV_64FC1), Hi_T(STATES, IMU_STATES,CV_64FC1),
Rb(VIC_STATES + IMU_STATES, VIC_STATES + IMU_STATES, CV_64FC1),
Hb(VIC_STATES + IMU_STATES,STATES,CV_64FC1),
Hb_T(STATES,VIC_STATES + IMU_STATES,CV_64FC1)
{
	// Sets up the pointers to point to the right information arrays 
	Xk = Xk_in; 
	Xkp1 = Xkp1_in; 
	Uk = Uk_in;
	Xkp1_v = Xkp1_v_in;
	Xk_v = Xk_v_in;
	Xkp1_i_w = Xkp1_i_w_in;

	// Sets up the system mass and inertia
	Mq = Mq_meas[quad_num];
	Jqx = Jqx_meas[quad_num];
	Jqy = Jqy_meas[quad_num];
	Jqz = Jqz_meas[quad_num];

	// Initializes the identity matrix
	I = Mat::eye(STATES,STATES,CV_64FC1);

	// Initializes the inertia matrix and its inverse
	J = Mat::zeros(3,3,CV_64FC1);
	J.at<double>(0,0) = Jqx; J.at<double>(1,1) = Jqy; J.at<double>(2,2) = Jqz;
	invert(J,J_inv);

	// Initializes the P matrix
	//P = 10*Mat::eye(STATES,STATES,CV_64FC1);
	resetPmatrix();

	// Initializes the H matrices;
	Hv = Mat::zeros(VIC_STATES,STATES,CV_64FC1);
	Hv.at<double>( Xv, X) = 1; Hv.at<double>( Yv, Y) = 1; Hv.at<double>( Zv, Z) = 1;
	Hv.at<double>(Q0v,Q0) = 1; Hv.at<double>(QIv,QI) = 1; Hv.at<double>(QJv,QJ) = 1;
	Hv.at<double>(QKv,QK) = 1;
	transpose(Hv,Hv_T);

	Hi = Mat::zeros(IMU_STATES,STATES,CV_64FC1);
	Hi.at<double>(WXi,WX) = 1;
	Hi.at<double>(WYi,WY) = 1;
	Hi.at<double>(WZi,WZ) = 1;
	transpose(Hi,Hi_T);
	
	Hb = Mat::zeros(VIC_STATES + IMU_STATES,VIC_STATES + IMU_STATES,CV_64FC1);
	Hb.at<double>( Xv, X) = 1; Hb.at<double>( Yv, Y) = 1; Hb.at<double>( Zv, Z) = 1;
	Hb.at<double>(Q0v,Q0) = 1; Hb.at<double>(QIv,QI) = 1; Hb.at<double>(QJv,QJ) = 1;
	Hb.at<double>(QKv,QK) = 1;
	Hb.at<double>(WXi + VIC_STATES,WX) = 1;
	Hb.at<double>(WYi + VIC_STATES,WY) = 1;
	Hb.at<double>(WZi + VIC_STATES,WZ) = 1;
	transpose(Hb,Hb_T);

	// Initializes the Q and R matrices
	Q = Mat::zeros(STATES,STATES,CV_64FC1);
	Rv = Mat::zeros(VIC_STATES,VIC_STATES,CV_64FC1);
	Ri = Mat::zeros(IMU_STATES,IMU_STATES,CV_64FC1);
	Rb = Mat::zeros(VIC_STATES + IMU_STATES, VIC_STATES + IMU_STATES,CV_64FC1);

	for(int i = 0; i < STATES; i++)
		Q.at<double>(i,i) = Q_diag[i];

	for(int i = 0; i < VIC_STATES; i++) {
		Rv.at<double>(i,i) = Rv_diag[i];
		Rb.at<double>(i,i) = Rv_diag[i];
	}

	for(int i = 0; i < IMU_STATES; i++) {
		Ri.at<double>(i,i) = Ri_diag[i]; 
		Rb.at<double>(i + VIC_STATES,i + VIC_STATES) = Ri_diag[i];
	}

	// Initializes Ad
	this->calcAd(Xk,Uk);
}


// Destructor
Filter3::~Filter3() { }


// Filtering Technique Setting
void Filter3::useKalmanFilter() 		{filterTechnique = FILT_KALMAN;}
void Filter3::useExtendedKalmanFilter()	{filterTechnique = FILT_EXT_KALMAN;}
void Filter3::useQuickFilter()			{filterTechnique = FILT_QUICK;}


// Resets the P matrix to its default value
void Filter3::resetPmatrix()
{
	P = 10*Mat::eye(STATES,STATES,CV_64FC1);
}


// Re-normalizes the quaternion to have a unit magnitude
void Filter3::normalizeQuat(double quat[])
{
	// ensures q0 > 0
	if(quat[0] < 0) {
		for( int i = 0; i <= 3; i++)
			quat[i] = -quat[i];
	}
	// re-normalizes the quaternion
	double quat_norm_inv = 1/(quat[0]*quat[0] 
		+ quat[1]*quat[1] + quat[2]*quat[2] + quat[3]*quat[3]);
	for(int i = 0; i <= 3; i++)
		quat[i] = quat[i]*quat_norm_inv;
}


// Matrix exponential function used in the Ac -> Ad conversion
Mat Filter3::matrixExponential(Mat in)
{
	/*
	  This code attempts to approximate the matrix exponential defined
	  as follows for a matrix x:

		inf  1   k
		sum --- X
		k=0  k!

	  Rather than performing an infinite number of iterations, this process
	  performs 25 iterations.
	*/

	Mat accumulator = Mat::eye(in.rows, in.cols,CV_64FC1);
	Mat out = Mat::eye(in.rows, in.cols,CV_64FC1);

	for(int k = 1; k < 30; k++) {
		accumulator = (1.0/k)*accumulator*in;
		out = out + accumulator;
	}

	return out;
}


// Creates a linear, discrete A matrix for the dynamics of the quadrotor
// for quaternion-based mechanics
void Filter3::calcAd(double Xk[], double Uk[])
{
	// Creates simplified state variables
	double q0 = Xk[Q0];
	double qi = Xk[QI];
	double qj = Xk[QJ];
	double qk = Xk[QK];
	double wx = Xk[WX];
	double wy = Xk[WY];
	double wz = Xk[WZ];	

	// Precalculates input-based values
	double T_2_div_M = 2*Uk[Th]/Mq;
	
	// Starts calculating Ac
	Ac = Mat::zeros(STATES,STATES,CV_64FC1);
	
	// Position-related values
	Ac.at<double>(X,DX) = 1; Ac.at<double>(Y,DY) = 1; Ac.at<double>(Z,DZ) = 1; 

	// Velocity-related values
	Ac.at<double>(DX,Q0) = T_2_div_M *  qj;
	Ac.at<double>(DX,QI) = T_2_div_M *  qk;
	Ac.at<double>(DX,QJ) = T_2_div_M *  q0;
	Ac.at<double>(DX,QK) = T_2_div_M *  qi;

	Ac.at<double>(DY,Q0) = T_2_div_M * -qi;
	Ac.at<double>(DY,QI) = T_2_div_M * -q0;
	Ac.at<double>(DY,QJ) = T_2_div_M *  qk;
	Ac.at<double>(DY,QK) = T_2_div_M *  qj;

	Ac.at<double>(DZ,Q0) = T_2_div_M *  q0;
	Ac.at<double>(DZ,QI) = T_2_div_M * -qi;
	Ac.at<double>(DZ,QJ) = T_2_div_M * -qj;
	Ac.at<double>(DZ,QK) = T_2_div_M *  qk;
	
	// Quaternion-related values
	Ac.at<double>(Q0,QI) = -0.5 * wx;
	Ac.at<double>(Q0,QJ) = -0.5 * wy;
	Ac.at<double>(Q0,QK) = -0.5 * wz;
	Ac.at<double>(Q0,WX) = -0.5 * qi;
	Ac.at<double>(Q0,WY) = -0.5 * qj;
	Ac.at<double>(Q0,WZ) = -0.5 * qk;

	Ac.at<double>(QI,Q0) =  0.5 * wx;
	Ac.at<double>(QI,QJ) =  0.5 * wz;
	Ac.at<double>(QI,QK) = -0.5 * wy;
	Ac.at<double>(QI,WX) =  0.5 * q0;
	Ac.at<double>(QI,WY) = -0.5 * qk;
	Ac.at<double>(QI,WZ) =  0.5 * qj;

	Ac.at<double>(QJ,Q0) =  0.5 * wy;
	Ac.at<double>(QJ,QI) = -0.5 * wz;
	Ac.at<double>(QJ,QK) =  0.5 * wx;
	Ac.at<double>(QJ,WX) =  0.5 * qk;
	Ac.at<double>(QJ,WY) =  0.5 * q0;
	Ac.at<double>(QJ,WZ) = -0.5 * qi;

	Ac.at<double>(QK,Q0) =  0.5 * wz;
	Ac.at<double>(QK,QI) =  0.5 * wy;
	Ac.at<double>(QK,QJ) = -0.5 * wx;
	Ac.at<double>(QK,WX) = -0.5 * qj;
	Ac.at<double>(QK,WY) =  0.5 * qi;
	Ac.at<double>(QK,WZ) = -0.5 * q0;

	// Angular velocity-related values
	Ac.at<double>(WX,WY) = (wz*(Jqy - Jqz))/Jqx;
	Ac.at<double>(WX,WZ) = (wy*(Jqy - Jqz))/Jqx;

	Ac.at<double>(WY,WX) = -(wz*(Jqx - Jqz))/Jqy;
	Ac.at<double>(WY,WZ) = -(wx*(Jqx - Jqz))/Jqy;

	Ac.at<double>(WZ,WX) = (wy*(Jqx - Jqy))/Jqz;
	Ac.at<double>(WZ,WY) = (wx*(Jqx - Jqy))/Jqz;


	// There! Ac has been calculated!
	// Now, time to get Ad and Ad_T...
	Mat temp = Ts*Ac;
	Ad = matrixExponential(temp);
	transpose(Ad,Ad_T);
}


// Uses Euler approximation to predict the state at X(k+1) based on X(k),
// U(k), and the time difference (t) between the two.
void Filter3::predictState(FlightMode current, double Xk[], double Xkp1[], double Uk[], double t)
{
	// If in a mode where the quadrotor can be expected to be sitting on the ground,
	// the prediction algorithm will assume the quadrotor won't move, so Xkp1 = Xk.
	switch(current) {
		case QUAD_OFF:
		case QUAD_IDLE:
		case QUAD_RAMPUP:
		case QUAD_EXITING:
		case QUAD_CYCLE:
		case QUAD_LATENCY_TEST:
		case QUAD_DMC:
		case QUAD_UNDEFINED:
			for(int i = 0; i < STATES; i++)
				Xkp1[i] = Xk[i];
			return;
		default:
			break;
	}

	// Holds the derivatives of each of the states
	double dXk[STATES];

	// Creates simplified state variables
	double q0 = Xk[Q0];
	double qi = Xk[QI];
	double qj = Xk[QJ];
	double qk = Xk[QK];
	double wx = Xk[WX];
	double wy = Xk[WY];
	double wz = Xk[WZ];	
	
	// Pre-computes input values that will get reused throughout
	double T_div_M = Uk[Th]/Mq;	// calculating this once saves a few division
					// operations, which are computationaly expensive

	// Calculates the derivatives of each of the states
	dXk[ X] = Xk[DX]; 
	dXk[ Y] = Xk[DY];
	dXk[ Z] = Xk[DZ];
	dXk[DX] = 2*T_div_M*( q0*qj + qi*qk );
	dXk[DY] = 2*T_div_M*( qj*qk - qi*q0 );
	dXk[DZ] =   T_div_M*( q0*q0 - qi*qi - qj*qj + qk*qk ) - G;
	dXk[Q0] = -0.5*( qi*wx + qj*wy + qk*wz );
	dXk[QI] =  0.5*( q0*wx + qj*wz - qk*wy );
	dXk[QJ] =  0.5*( q0*wy - qi*wz + qk*wx );
	dXk[QK] =  0.5*( q0*wz + qi*wy + qj*wx );

	// The elegant way to get the angular accelerations
	/*
	The operation to be performed: 
	  omega_dot = inv(J)*(Gamma - omega x J*omega), where
		- Theta_dot: angular acceleration values of the quadrotor in Body frame
			(the last three states in dXk[])
		- J: rotational inertia matrix for the quadrotor
		- Gamma: Input torques vector [Tx Ty Tz]'
		- omega: quadrotor angular velocities in Body frame; 
			need to be translated from World frame
		
		J and inv(J) are pre-calculated in the constructor, and omega
		and omega_array are already declared. 
	*/
	Mat omega_dot(3,1,CV_64FC1,&dXk[WX]); // Matrix wrapper for performing matrix/vector
					// operations with the array
	Mat Gamma(3,1,CV_64FC1,&Uk[Tx]); // another wrapper

	omega.at<double>(0,0) = Xk[WX];
	omega.at<double>(1,0) = Xk[WY];
	omega.at<double>(2,0) = Xk[WZ];

	Mat temp = J*omega;
	omega_dot = J_inv*(Gamma - omega.cross(temp));	

	// Calculates X(k+1) via Euler approximation and ensures
	// accidental Not a Number values don't propagate through
	// the system
	for(int i = 0; i < STATES; i++) {
		Xkp1[i] = Xk[i] + t*dXk[i];

		if( std::isnan(Xkp1[i]) )
			Xkp1[i] = 0;
	}

	// Resets the quaternion to unit magnitude
	normalizeQuat(&Xkp1[Q0]);
}


// Performs the prediction phase of the Kalman/Extended Kalman filter
void Filter3::kalmanPredict(FlightMode current)
{
	// Performs the state evolution prediction
	// Uses the nonlinear dynamics of the system and Euler approximation
	// X~(k+1) = f(X(k), U(k))
	this->predictState(current, Xk, Xkp1_est, Uk, Ts); // temporarily set to Ts, found in "../defs.cpp"

	// If doing Extended Kalman Filtering, obtain a new Ad matrix
	if(filterTechnique == FILT_EXT_KALMAN)
		calcAd(Xk, Uk);

	// Performs the estimate covariance prediction
	// Ppred = Ad*P*Ad' + Q
	Ppred = Ad*P*Ad_T + Q;
}


// Performs the update phase of the Kalman/Extended Kalman filter for Vicon measurements
void Filter3::kalmanUpdateVicon()
{
	// Finds the residual covariance
	// S = H*Ppred*H' + R
	Mat S(VIC_STATES,VIC_STATES,CV_64FC1); // residual covariance matrix
	S = Hv*Ppred*Hv_T + Rv;

	// Finds the inverse of the residual covariance
	// a.k.a. S^-1
	Mat S_inv(VIC_STATES,VIC_STATES,CV_64FC1); // inverse of S
	invert(S,S_inv);

	// Calculates the Kalman filter gain matrix
	// K = Ppred*H'*inv(S)
	Mat K(STATES, VIC_STATES,CV_64FC1); // Kalman filter gain matrix
	K = Ppred*Hv_T*S_inv;

	// Calculates the residual
	// V = X_meas(k+1) - X~(k+1)
	Mat V(VIC_STATES,1,CV_64FC1);	// residual vector
	Mat Xkp1_v_mat(VIC_STATES,1,CV_64FC1,Xkp1_v);
	V = Xkp1_v_mat - (Hv*Xkp1_est_mat);

	// Update the estimated state
	// X(k+1) = X~(k+1) + K*V
	Mat Xkp1_mat(STATES,1,CV_64FC1,Xkp1); // estimated state vector wrapper
	Xkp1_mat = Xkp1_est_mat + K*V;

	// Update the estimate covariance
	// P = (I - K*H)*Ppred
	P = (I - K*Hv)*Ppred;

	// Resets quaternion to a unit magnitude
	normalizeQuat(&Xkp1[Q0]);
}


// Performs the update phase of the Kalman/Extended Kalman filter for IMU measurements
void Filter3::kalmanUpdateImu()
{
	// Finds the residual covariance
	// S = H*Ppred*H' + R
	Mat S(IMU_STATES,IMU_STATES,CV_64FC1); // residual covariance matrix
	S = Hi*Ppred*Hi_T + Ri;

	// Finds the inverse of the residual covariance
	// a.k.a. S^-1
	Mat S_inv(IMU_STATES,IMU_STATES,CV_64FC1); // inverse of S
	invert(S,S_inv);

	// Calculates the Kalman filter gain matrix
	// K = Ppred*H'*inv(S)
	Mat K(STATES, IMU_STATES,CV_64FC1); // Kalman filter gain matrix
	K = Ppred*Hi_T*S_inv;

	// Calculates the residual
	// V = X_meas(k+1) - X~(k+1)
	Mat V(IMU_STATES,1,CV_64FC1);	// residual vector
	Mat Xkp1_i_mat(IMU_STATES,1,CV_64FC1,Xkp1_i_w);
	V = Xkp1_i_mat - (Hi*Xkp1_est_mat);

	// Update the estimated state
	// X(k+1) = X~(k+1) + K*V
	Mat Xkp1_mat(STATES,1,CV_64FC1,Xkp1); // estimated state vector wrapper
	Xkp1_mat = Xkp1_est_mat + K*V;

	// Update the estimate covariance
	// P = (I - K*H)*Ppred
	P = (I - K*Hi)*Ppred;

	// Resets quaternion to a unit magnitude
	normalizeQuat(&Xkp1[Q0]);
}


// Performs the update phase of the Kalman/Extended Kalman filter for IMU measurements
void Filter3::kalmanUpdateBoth()
{
	// Finds the residual covariance
	// S = H*Ppred*H' + R
	Mat S(VIC_STATES + IMU_STATES,VIC_STATES + IMU_STATES,CV_64FC1); // residual covariance matrix
	S = Hb*Ppred*Hb_T + Rb;

	// Finds the inverse of the residual covariance
	// a.k.a. S^-1
	Mat S_inv(VIC_STATES + IMU_STATES,VIC_STATES + IMU_STATES,CV_64FC1); // inverse of S
	invert(S,S_inv);

	// Calculates the Kalman filter gain matrix
	// K = Ppred*H'*inv(S)
	Mat K(STATES, VIC_STATES + IMU_STATES,CV_64FC1); // Kalman filter gain matrix
	K = Ppred*Hb_T*S_inv;

	// Calculates the residual
	// V = X_meas(k+1) - X~(k+1)
	Mat V(VIC_STATES + IMU_STATES,1,CV_64FC1);	// residual vector
	Mat Xkp1_b_mat(VIC_STATES + IMU_STATES,1,CV_64FC1);
	Xkp1_b_mat.at<double>( Xv, 1) = Xkp1_v[ Xv];
	Xkp1_b_mat.at<double>( Yv, 1) = Xkp1_v[ Yv];
	Xkp1_b_mat.at<double>( Zv, 1) = Xkp1_v[ Zv];
	Xkp1_b_mat.at<double>(Q0v, 1) = Xkp1_v[Q0v];
	Xkp1_b_mat.at<double>(QIv, 1) = Xkp1_v[QIv];
	Xkp1_b_mat.at<double>(QJv, 1) = Xkp1_v[QJv];
	Xkp1_b_mat.at<double>(QKv, 1) = Xkp1_v[QKv];
	Xkp1_b_mat.at<double>(VIC_STATES + WXi,1) = Xkp1_i_w[WXi];
	Xkp1_b_mat.at<double>(VIC_STATES + WYi,1) = Xkp1_i_w[WYi];
	Xkp1_b_mat.at<double>(VIC_STATES + WZi,1) = Xkp1_i_w[WZi];
	Xkp1_b_mat.at<double>(VIC_STATES + AXi,1) = Xkp1_i_w[AXi];
	Xkp1_b_mat.at<double>(VIC_STATES + AYi,1) = Xkp1_i_w[AYi];
	Xkp1_b_mat.at<double>(VIC_STATES + AZi,1) = Xkp1_i_w[AZi];
	
	V = Xkp1_b_mat - (Hb*Xkp1_est_mat);

	// Update the estimated state
	// X(k+1) = X~(k+1) + K*V
	Mat Xkp1_mat(STATES,1,CV_64FC1,Xkp1); // estimated state vector wrapper
	Xkp1_mat = Xkp1_est_mat + K*V;

	// Update the estimate covariance
	// P = (I - K*H)*Ppred
	P = (I - K*Hb)*Ppred;

	// Resets quaternion to a unit magnitude
	normalizeQuat(&Xkp1[Q0]);
}


// Quick filter prediction phase
void Filter3::quickPredict(FlightMode current)
{
	// Performs the state evolution prediction
	// Uses the nonlinear dynamics of the system and Euler approximation
	// X~(k+1) = f(X(k), U(k))
	this->predictState(current, Xk, Xkp1_est, Uk, Ts); // temporarily set to Ts, found in "../defs.cpp"
}


// Quick filter update phase
void Filter3::quickUpdate(bool vicPosUpdate, bool vicVelUpdate)
{
	double Xkp1_meas[STATES];
	
	// Gets position values directly from Vicon
	Xkp1_meas[ X] = Xkp1_v[ Xv];
	Xkp1_meas[ Y] = Xkp1_v[ Yv];
	Xkp1_meas[ Z] = Xkp1_v[ Zv];
	// Gets Quaternion values directly from Vicon
	Xkp1_meas[Q0] = Xkp1_v[Q0v];
	Xkp1_meas[QI] = Xkp1_v[QIv];
	Xkp1_meas[QJ] = Xkp1_v[QJv];
	Xkp1_meas[QK] = Xkp1_v[QKv];
	// Gets angular velocities directly from the IMU
	Xkp1_meas[WX] = Xkp1_i_w[WXi];
	Xkp1_meas[WY] = Xkp1_i_w[WYi];
	Xkp1_meas[WZ] = Xkp1_i_w[WZi];


	// Interpolates velocity values from a Vicon derivative and an IMU acceleration integration
	double imu_vel_x = Xk[DX] + Ts*Xkp1_i_w[AXi];	// IMU integrations
	double imu_vel_y = Xk[DY] + Ts*Xkp1_i_w[AYi];
	double imu_vel_z = Xk[DZ] + Ts*Xkp1_i_w[AZi];

	double vic_vel_x = (Xkp1_v[Xv] - Xk_v[Xv])/Ts;	// Vicon derivatives
	double vic_vel_y = (Xkp1_v[Yv] - Xk_v[Yv])/Ts;
	double vic_vel_z = (Xkp1_v[Zv] - Xk_v[Zv])/Ts;	

	Xkp1_meas[DX] = alpha*imu_vel_x + (1 - alpha)*vic_vel_x;
	Xkp1_meas[DY] = alpha*imu_vel_y + (1 - alpha)*vic_vel_y;
	Xkp1_meas[DZ] = alpha*imu_vel_z + (1 - alpha)*vic_vel_z;
	
	
	// If Vicon's valid, active, and new, update position and attitude
	if(vicPosUpdate) {
		for(int i =  X; i <=  Z; i++)
			Xkp1[i] = C_diag[i]*Xkp1_est[i] + (1 - C_diag[i])*Xkp1_meas[i];
		for(int i = Q0; i <= QK; i++)
			Xkp1[i] = C_diag[i]*Xkp1_est[i] + (1 - C_diag[i])*Xkp1_meas[i];		

		// if the last Vicon value was also valid, update the velocities as well.
		if(vicVelUpdate) {
			for(int i = DX; i <= DZ; i++)
				Xkp1[i] = C_diag[i]*Xkp1_est[i] + (1 - C_diag[i])*Xkp1_meas[i];
		}
		// Otherwise, simply use the IMU values.
		else {
			Xkp1[DX] = C_diag[DX]*Xkp1_est[DX] + (1 - C_diag[DX])*imu_vel_x;
			Xkp1[DY] = C_diag[DY]*Xkp1_est[DY] + (1 - C_diag[DY])*imu_vel_y;
			Xkp1[DZ] = C_diag[DZ]*Xkp1_est[DZ] + (1 - C_diag[DZ])*imu_vel_z;
		}
	}
	// Otherwise, simply set all Vicon-updated variables to their estimates
	else {
		for(int i = X; i <= QK; i++)
			Xkp1[i] = Xkp1_est[i];
	}

	// Update angular velocities regardless of Vicon status
	for(int i = WX; i <= WZ; i++)
		Xkp1[i] = C_diag[i]*Xkp1_est[i] + (1 - C_diag[i])*Xkp1_meas[i];	
	
	// Resets quaternion to a unit magnitude
	normalizeQuat(&Xkp1[Q0]);
}


void Filter3::printAd()
{
	cout << Ad << endl;
}
