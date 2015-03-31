#include "Actuator.h"
#define dt 0.01

/************************************************
@Brief : Discrete Transfer Function of Servo
**************************************************/
void ACTUATOR::Servo(float wn, float zeta, double u, double *y)
{
		yn[2] = (yn[1]*(2/dt/dt + zeta*wn/dt)-yn[0]/dt/dt+wn*wn*u)/(1/dt/dt+zeta*wn/dt+wn*wn);
		yn[0] = yn[1];
		yn[1] = yn[2];
		*y = yn[2];
}