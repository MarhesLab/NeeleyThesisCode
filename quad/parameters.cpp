#include "parameters.h"

// Name Parameters
std::string vicon_names[] = {"quadrotor_1","quadrotor_2","quadrotor_3"};

// Inertia Parameters
// Source of supplied inertia parameters (second set): 
// http://asctec-users.986163.n3.nabble.com/Parameters-Asctec-Hummingbird-td4024755.html
// Source of current values: Step torque response testing
const double Mq_meas[]  = { 0.654, 99.9, 99.9};  // net mass (kg)
const double Jqx_meas[] = {0.0072, 0.00365, 99.9}; // inertia around x axis (kg*m^2)
const double Jqy_meas[] = {0.0072, 0.00368, 99.9}; // inertia around y axis (kg*m^2)
const double Jqz_meas[] = {0.016, 0.00703, 99.9}; // inertia around z axis (kg*m^2)
const double CG_off_x[] = {-0.0060, 0, 0}; // Center of Gravity along X axis (m)
const double CG_off_y[] = {-0.0005, 0, 0}; // Center of Gravity along Y axis (m)



/* const double Jqx_meas[] = {0.0117, 99.9, 99.9}; // inertia around x axis (kg*m^2)
const double Jqy_meas[] = {0.0117, 99.9, 99.9}; // inertia around y axis (kg*m^2)
const double Jqz_meas[] = {0.0234, 99.9, 99.9}; // inertia around z axis (kg*m^2) */

// Length Parameters
const double Lq_meas[]  = {0.17, 0.17, 0.17}; // length of quadrotor arms

// Propeller Parameters
const double Rp_meas[]  = {0.1, 0.1, 0.1}; // propeller radius
const double T_factor[] = {4.90*pow(10,-8), 6.11*pow(10,-8), 6.11*pow(10,-8)}; // Propeller thrust factor (N/(rpm)^2)
const double D_factor[] = {8.50*pow(10,-10), 1.50*pow(10,-9), 1.50*pow(10,-9)}; // Propeller drag factor (N*m/(rpm)^2)
const double Bp_calc[]  = {T_factor[0]/pow(rpm2rad,2), T_factor[1]/pow(rpm2rad,2), T_factor[2]/pow(rpm2rad,2)};
const double Kp_calc[]  = {D_factor[0]/pow(rpm2rad,2), D_factor[1]/pow(rpm2rad,2), D_factor[2]/pow(rpm2rad,2)};
