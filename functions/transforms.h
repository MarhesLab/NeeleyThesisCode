#ifndef TRANSFORMS_H
#define TRANSFORMS_H

double minAngle(double angle);

// Coordinate Transforms
void quatRot   (double in[], double out[], double quat[]);

void quatRotInv(double in[], double out[], double quat[]);

void quat2EulerZYX(double quat[], double euler[]);

void eulerZYX2Quat(double euler[], double quat[]);

double quat2Yaw(double quat[]);


// Input Transforms
void inputsToMotorSpeeds(double Uq[], double omega[], double Lq, double Bp, double Kp);

void motorSpeedsToInputs(double omega[], double Uq[], double Lq, double Bp, double Kp);

#endif
