#ifndef FANCY_FILTER_STUFF_OPEN_CV_AGAIN_H
#define FANCY_FILTER_STUFF_OPEN_CV_AGAIN_H

#include <opencv2/core/core.hpp>
#include "../defs.h"

using namespace cv;

class Filter3 {
  protected:
	// Settings variables
	enum FilterType filterTechnique;

	// Quadrotor parameters
	double Mq;
	double Jqx, Jqy, Jqz; 

	// States and Inputs
	double* Xk;	// points to the "current" state vector used for filtering
	double* Xkp1;	// points to the next state vector used for filtering
	double  Xkp1_est[STATES]; // holds the estimate for the next state
	double* Uk;	// points to the "current" input vector used for filtering
	double* Xkp1_v;	// points to the "next" set of Vicon measurements
	double* Xk_v;	// points to the "current" set of Vicon measurements
	double* Xkp1_i_w;	// points to the "next" set of IMU measurements
	                    // ACCELERATIONS ARE IN WORLD FRAME

	Mat Xkp1_est_mat; // Matrix2 wrapper for Xkp1_est (12x1)



	// State prediction matrices
	Mat J, J_inv;	// rotational inertia matrix and its inverse (3x3)
	Mat omega;		// angular velocity matrix

	// Kalman Filter Matrices
	Mat Ac; // continuous, linear A matrix for the system dynamics (STATES x STATES)
	Mat Ad; // discrete, linear A matrix for the system dynamics (STATES x STATES)
	Mat Ad_T; // Ad transpose (STATES x STATES)
	Mat Q; // prediction covariance matrix (STATES x STATES)
	Mat I; // Identity matrix used in the Update phase (STATES x STATES)
	
	Mat P; // estimate covariance matrix (STATES x STATES)
	Mat Ppred; // predicted estimate covariance; used in updating P (STATES x STATES)

	// Vicon Update-related matrices
	Mat Rv; // Vicon measurement covariance matrix (VIC_STATES x VIC_STATES)
	Mat Hv; // maps Vicon measurements to the appropriate states (VIC_STATES x STATES)
	Mat Hv_T; // Hv transpose (STATES x VIC_STATES)

	// IMU Update-related matrices
	Mat Ri; // IMU measurement covariance matrix (IMU_STATES x IMU_STATES)
	Mat Hi; // maps IMU measurements to the appropriate states (IMU_STATES x STATES)
	Mat Hi_T; // Hi transpose (STATES x IMU_STATES)
	
	// Vicon and IMU Update-related matrices
	Mat Rb;
	Mat Hb;
	Mat Hb_T;


  public:
	// Constructor and Destructor
	Filter3(double* Xk_in, double* Xkp1_in, double* Uk_in, 
		double* Xkp1_v_in, double* Xk_v_in, double* Xkp1_i_w_in, int quad_num);
	~Filter3();

	// Kalman Filter algorithm selector
	void useKalmanFilter();
	void useExtendedKalmanFilter();
	void useQuickFilter();

	// Utility Functions
	void resetPmatrix();
	void normalizeQuat(double quat[]);
	Mat matrixExponential(Mat in);
	void calcAd(double Xk[], double Uk[]);
	void predictState(FlightMode current, double Xk[], double Xkp1[], double Uk[], double t);

	// Kalman Filter Functions
	void kalmanPredict(FlightMode current);
	void kalmanUpdateVicon();
	void kalmanUpdateImu();
	void kalmanUpdateBoth();
	
	// Quick Filter Functions
	void quickPredict(FlightMode current);
	void quickUpdate(bool vicPosUpdate, bool vicVelUpdate);
	
	// Troubleshooting Functions
	void printAd();
};

#endif
