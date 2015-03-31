/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

************************************************************************************************/


#include "stdafx.h"
//#include "Matrix.h"

//MATRIX Matrix;

void FORCE::Para(double *Parameter)
{
	this -> mass		= Parameter[0];
	this -> Ixx			= Parameter[1];
	this -> Iyy			= Parameter[2];
	this -> Izz			= Parameter[3];
	this -> Ixz			= Parameter[4];

	this -> Area		= Parameter[5];
	this -> Chord		= Parameter[6];
	this -> Wingspan	= Parameter[7];
	this -> Xcg			= Parameter[8];
	this -> X			= Parameter[9];

	this -> CLo			= Parameter[10];
	this -> CLa			= Parameter[11];
	this -> CLadot		= Parameter[12];
	this -> CLq			= Parameter[13];

	this -> CLde		= Parameter[14];
	this -> CLdf		= Parameter[15];

	this -> CYb			= Parameter[16];
	this -> CYp			= Parameter[17];
	this -> CYr			= Parameter[18];
	this -> CYdr		= Parameter[19];

	this -> Clb			= Parameter[20];
	this -> Clp			= Parameter[21];
	this -> Clr			= Parameter[22];
	this -> Clda		= Parameter[23];
	this -> Cldr		= Parameter[24];

	this -> CMo			= Parameter[25];
	this -> CMa			= Parameter[26];
	this -> CMadot		= Parameter[27];
	this -> CMq			= Parameter[28];
	this -> CMde		= Parameter[29];
	this -> CMdf		= Parameter[30];

	this -> CNb			= Parameter[31];
	this -> CNp			= Parameter[32];
	this -> CNr			= Parameter[33];
	this -> CNdr		= Parameter[34];
	this -> CNda		= Parameter[35];

	this -> CDo			= Parameter[36];
}

void FORCE::Wind(double N, double E, double D)
{
	this->wNED[0] = N;
	this->wNED[1] = E;
	this->wNED[2] = D;
}


void FORCE::Compute_forces(double *quat, double *UVW, double *PQR, double *Control)
{
	q0 = quat[0]; q1 = quat[1]; q2 = quat[2]; q3 = quat[3];

	//Each Control variables has a different unit : degree, radian, radian
	//		when Simulation calculate the control surface input using the Nonlinear 
	//		Trajectory Tracking logic and the Specific Force Acceleration Matching guidance.


	Ail = Control[0];				
	Elev = Control[1];								
	Rud = Control[2];								
	Pow = Control[3];//;


	//temporary
	Pow = 0.0;


	To = 2.0*0.8*(280.0/800.0)*Pow*9.81;

	//Complete transformation matrix
	ctm[0][0] = q0*q0 + q1*q1 - q2*q2 - q3*q3;
	ctm[0][1] = 2*(q1*q2 + q0*q3);
	ctm[0][2] = 2*(q1*q3 - q0*q2);
	ctm[1][0] = 2*(q1*q2 - q0*q3);
	ctm[1][1] = q0*q0 - q1*q1 + q2*q2 - q3*q3;
	ctm[1][2] = 2*(q2*q3 + q0*q1);
	ctm[2][0] = 2*(q1*q3 + q0*q2);
	ctm[2][1] = 2*(q2*q3 - q0*q1);
	ctm[2][2] = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	//compute external effect on the aircraft
	//wUVW = ctm * wNED
	//dUVW = UVW - wUVW
	//Matrix.mult((double *)ctm, (double *)wNED, (double *)wUVW, 3, 3, 3, 1);
	//Matrix.sub((double *)UVW,(double *)wUVW,(double *)dUVW,3,1);

	double Temp = 0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp = Temp + ctm[i][j]*wNED[j];
		}
		wUVW[i] = Temp;
		Temp = 0;
	}
	for(int i=0;i<3;i++)
	{
		dUVW[i] = UVW[i] - wUVW[i];
	}
	
	//calculate airspeed
	Vt = sqrt(dUVW[0]*dUVW[0] + dUVW[1]*dUVW[1] + dUVW[2]*dUVW[2]);
	alpha = atan2(dUVW[2], dUVW[0]);
	beta = asin(dUVW[1]/Vt);
	AoA = alpha;
	AoB = beta;
	//Rotate rates from body to stability axes
	C_s2b[0][0] = cos(alpha);	C_s2b[0][1] = 0;	C_s2b[0][2] = sin(alpha);
	C_s2b[1][0] = 0;			C_s2b[1][1] = 1;	C_s2b[1][2] = 0;
	C_s2b[2][0] = -sin(alpha);	C_s2b[2][1] = 0;	C_s2b[2][2] = cos(alpha);

	
	//Rotate rates from stability to body axes
	//Matrix.transform((double *)C_s2b, (double *)C_b2s, 3, 3);
	//Matrix.mult((double *)C_s2b, (double *)PQR, (double *)PQRs, 3,3,3,1);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			C_b2s[i][j] = C_s2b[j][i];
		}
	}

	//body to stability
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp = Temp + C_b2s[i][j]*PQR[j];
		}
		PQRs[i] = Temp;
		Temp = 0;
	}

	Ps = PQRs[0];
	Qs = PQRs[1];
	Rs = PQRs[2];

	//Calculate aerodynamics coefficient
	CL = CLo + CLa*alpha + CLq*Wingspan*Qs/(2*Vt) + CLde*Elev;
	if(CL>1.2) CL = 1.2;
	if(CL<-1.2) CL = -1.2;

	CD = CDo + CL*CL/(3.141592654*12*0.85);
	if(CD>2.0) CD = 2.0;
	if(CD<-2.0) CD = -2.0;
	CX = -CD;
	CY = CYb*beta + CYdr*Rud + CYp*Wingspan*Ps/(2*Vt) + CYr*Wingspan*Rs/(2*Vt);
	CZ = -CL;
	Cl = Clb*beta + Clp*Ps*Wingspan/(2*Vt) + Clr*Rs*Wingspan/(2*Vt) + Clda*Ail;
	Cm = CMo + CMa*alpha + CMq*Qs*Chord/(2*Vt) + CMde*Elev;
	Cn = CNb*beta + CNp*Ps*Wingspan/(2*Vt) + CNr*Rs*Wingspan/(2*Vt) + CNdr*Rud + CNda*Ail;

	pres = 0.5*Vt*Vt*rho;

	//compute wind forces and moments
	wForce[0] = CX*pres*Area;
	wForce[1] = CY*pres*Area;
	wForce[2] = CZ*pres*Area;
	wMoment[0] = Cl*pres*Area*Wingspan;
	wMoment[1] = Cm*pres*Area*Chord;
	wMoment[2] = Cn*pres*Area*Wingspan;

	//Rotate wind forces and moments to stability axis
	//Matrix.mult((double *)C_b2s, (double *)wForce, (double *)Force,3,3,3,1);
	//Matrix.mult((double *)C_b2s, (double *)wMoment, (double*)Moment,3,3,3,1);
	Force[0] = wForce[0]*cos(beta)-wForce[1]*sin(beta);
	Force[1] = wForce[0]*sin(beta)+wForce[1]*cos(beta);
	Force[2] = wForce[2];
	Moment[0]  = wMoment[0]*cos(beta)-wMoment[1]*sin(beta);
	Moment[1]  = wMoment[0]*sin(beta)+wMoment[1]*cos(beta);
	Moment[2]  = wMoment[2];

	// Conversion from stability to body axis
	Force[0] = Force[0]*cos(alpha)-Force[2]*sin(alpha) + To;
	Force[1] = Force[1];
	Force[2] = Force[0]*sin(alpha)+Force[2]*cos(alpha);
	Moment[0] = Moment[0]*cos(alpha)-Moment[2]*sin(alpha);
	Moment[1] = Moment[1];
	Moment[2] = Moment[0]*sin(alpha)+Moment[2]*cos(alpha);

	if(Vt<0)
	{
		Force[0] = 0;
		Force[1] = 0;
		Force[2] = 0;
		Moment[0] = 0;
		Moment[1] = 0;
		Moment[2] = 0;
	}

	//Transfer forces and moments to the system.cpp
	//class : forces 
	//public : out
	for(int i=0;i<3;i++)
	{
		out[i] = Force[i];
		SpecA[i] = Force[i]/mass;
	}
	for(int i=0;i<3;i++)
	{
		out[3+i] = Moment[i];
	}

}