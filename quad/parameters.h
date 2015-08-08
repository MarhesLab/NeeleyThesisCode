#ifndef QUAD_MEAS_H
#define QUAD_MEAS_H

#include <math.h>
#include <string>

#include "../defs.h" // rad2rpm

// Name Parameters
extern std::string vicon_names[];

// Inertia Parameters
extern const double Mq_meas[];
extern const double Jqx_meas[];
extern const double Jqy_meas[];
extern const double Jqz_meas[];
extern const double CG_off_x[];
extern const double CG_off_y[];

// Length Parameters
extern const double Lq_meas[];

// Propeller Parameters
extern const double Rp_meas[];
extern const double T_factor[];
extern const double D_factor[];
extern const double Bp_calc[];
extern const double Kp_calc[];


#endif
