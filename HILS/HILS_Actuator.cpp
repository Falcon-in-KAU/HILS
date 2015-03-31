#include "stdafx.h"

/************************************************
@Brief : Discrete Transfer Function of Servo
**************************************************/
void ACTUATOR::Servo(float wn, float zeta, double u, double *y)
{
		yn[2] = (yn[1]*(2/SAMPLING/SAMPLING + zeta*wn/SAMPLING)-yn[0]/SAMPLING/SAMPLING+wn*wn*u)/(1/SAMPLING/SAMPLING+zeta*wn/SAMPLING+wn*wn);
		yn[0] = yn[1];
		yn[1] = yn[2];
		*y = yn[2];
}