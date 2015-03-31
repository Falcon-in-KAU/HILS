/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

************************************************************************************************/


#include "System.h"
#include <string.h>
//#include "Matrix.h"

//MATRIX matrix;

void SYSTEM::Parameter(double *parameter)
{
	mass = parameter[0];
	Ixx  = parameter[1];
	Iyy  = parameter[2];
	Izz  = parameter[3];
	Ixz  = parameter[4];
}

void SYSTEM::Init(double phi, double theta, double psi, double U, double V, double W, double N, double E, double D)
{
	phi = phi*3.14159226/180;
	theta = theta*3.14159226/180;
	psi = psi*3.14159226/180;

	q[0] = cos(phi/2)*cos(theta/2)*cos(psi/2)+sin(phi/2)*sin(theta/2)*sin(psi/2);
	q[1] = sin(phi/2)*cos(theta/2)*cos(psi/2)-cos(phi/2)*sin(theta/2)*sin(psi/2);
	q[2] = cos(phi/2)*sin(theta/2)*cos(psi/2)+sin(phi/2)*cos(theta/2)*sin(psi/2);
	q[3] = cos(phi/2)*cos(theta/2)*sin(psi/2)-sin(phi/2)*sin(theta/2)*cos(psi/2);

	this -> Vrel[0] = U;
	this -> Vrel[1] = V;
	this -> Vrel[2] = W;

	for(int i=0;i<13;i++)
	{
		this->x[i] = 0;
	}
	for(int i=0;i<13;i++)
		this->x_dt[i] = 0;

	pNED[0] = N;
	pNED[1] = E;
	pNED[2] = D;
	
 
}


void SYSTEM::System(double in[])
{
	F_at[0] = in[0];	F_at[1] = in[1];	F_at[2] = in[2];
	M_at[0] = in[3];	M_at[1] = in[4];	M_at[2] = in[5];

	norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	if(fabs(norm-1.0) > 0.0000001)
	{
		q[0] = q[0]/norm;
		q[1] = q[1]/norm;
		q[2] = q[2]/norm;
		q[3] = q[3]/norm;
	}
	g[0] = 0;
	g[1] = 0;
	g[2] = 9.80665;

	//input state variables
	x[0] = pNED[0];	x[1] = pNED[1];	x[2] = pNED[2]; 
	x[3] = q[0];	x[4] = q[1];	x[5] = q[2];	x[6] = q[3];
	x[7] = Vrel[0];	x[8] = Vrel[1];	x[9] = Vrel[2];
	x[10] = wbe[0];	x[11] = wbe[1];	x[12] = wbe[2];

	gamma = Ixx*Izz - Ixz*Ixz;

	J[0][0] = Ixx;	J[0][1] = 0;	J[0][2] = -Ixz;
	J[1][0] = 0;	J[1][1] = Iyy;	J[1][2] = 0;
	J[2][0] = -Ixz;	J[2][1] = 0;	J[2][2] = Izz;

	J_inv[0][0] =Izz/gamma;	J_inv[0][1] =0;		J_inv[0][2] =Ixz/gamma;
	J_inv[1][0] =0;			J_inv[1][1] =1/Iyy;	J_inv[1][2] =0;
	J_inv[2][0] =Ixz/gamma;	J_inv[2][1] =0;		J_inv[2][2] =Ixx/gamma;

	//Rotational transformation matrix from earth to body
	C_e2b[0][0] = q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
	C_e2b[0][1] = 2*(q[1]*q[2] + q[0]*q[3]);
	C_e2b[0][2] = 2*(q[1]*q[3] - q[0]*q[2]);
	C_e2b[1][0] = 2*(q[1]*q[2] - q[0]*q[3]);
	C_e2b[1][1] = q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
	C_e2b[1][2] = 2*(q[2]*q[3] + q[0]*q[1]);
	C_e2b[2][0] = 2*(q[1]*q[3] + q[0]*q[2]);
	C_e2b[2][1] = 2*(q[2]*q[3] - q[0]*q[1]);
	C_e2b[2][2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];


	//Rotational transformation matrix from body to earth
	//	matrix.transform((double *)C_e2b, (double *)C_b2e, 3,3);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			C_b2e[i][j] = C_e2b[j][i];
		}
	}

	//define Rate matrix
	P = wbe[0];
	Q = wbe[1];
	R = wbe[2];

	Omega[0][0] =0;		Omega[0][1] =-R;	Omega[0][2] =Q;	
	Omega[1][0] =R;		Omega[1][1] =0;		Omega[1][2] =-P;	
	Omega[2][0] =-Q;	Omega[2][1] =P;		Omega[2][2] =0;

	qOmega[0][0] =0;	qOmega[0][1] =P;	qOmega[0][2] =Q;	qOmega[0][3] =R;
	qOmega[1][0] =-P;	qOmega[1][1] =0;	qOmega[1][2] =-R;	qOmega[1][3] =Q;
	qOmega[2][0] =-Q;	qOmega[2][1] =R;	qOmega[2][2] =0;	qOmega[2][3] =-P;
	qOmega[3][0] =-R;	qOmega[3][1] =-Q;	qOmega[3][2] =P;	qOmega[3][3] =0;

//	Euler[0] = atan2(C_e2b[2][3],C_e2b[3][3]);
//	Euler[1] = -asin(C_e2b[1][3]);
//	Euler[2] = atan2(C_e2b[1][2], C_e2b[1][1]);


	//--------------------
	//Navigation Equation
	//pNED_dt = C_b2e*Vrel
	//--------------------
//	matrix.mult((double *)C_b2e, (double *)Vrel, (double *)pNED_dt, 3,3,3,1);
	double Temp=0;

	//C_b2e*Vrel
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp = Temp + C_b2e[i][j]*Vrel[j];
		}
		pNED_dt[i] = Temp;
		Temp = 0;
	}


	//------------------------------------------
	//Force Equation
	//Vrel_dt = -Omega*Vrel + C_e2b*g + 1/m*F_at
	//------------------------------------------
	//-Omega*Vrel : temp
	double temp[3] = {0,};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp = Temp - Omega[i][j]*Vrel[j];
		}
		temp[i] = Temp;
		Temp = 0;
	}

	//C_e2b*g : temp2
	double temp2[3] = {0,};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp = Temp + C_e2b[i][j]*g[j];
		}
		temp2[i] = Temp;
		Temp = 0;
	}
	//1/m*F_at : temp3
	double temp3[3] = {0,}; 
	for(int i=0;i<3;i++)
	{
		temp3[i] = F_at[i]/mass;
	}
	//Vrel_dt
	for(int i=0;i<3;i++)
	{
		Vrel_dt[i] = temp[i]+temp2[i]+temp3[i];
	}
	memset(temp,0,sizeof(double));
	memset(temp2,0,sizeof(double));
	memset(temp3,0,sizeof(double));


	//-------------------------------
	//Moment Equation
	//wbe_dt = J_inv*(-Omega*J*wbe + M_at)
	//-------------------------------
	double temp4[3][3] = {0,};
	double temp5[3][3] = {0,};

	//Omega*J = temp4
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Temp=0;
			for(int k=0;k<3;k++)
			{
				Temp = Temp + Omega[i][k]*J[k][j];
			}
			temp4[i][j] = Temp;
		}
	}

	//(Omega*J)*wbe = temp
	for(int i=0;i<3;i++)
	{
		Temp = 0;
		for(int j=0;j<3;j++)
		{
			Temp = Temp + temp4[i][j]*wbe[j];
		}
		temp[i] = Temp;
	}

	//Omega*J*wbe = -temp + M_at = temp2
	for(int i=0;i<3;i++)
	{
		temp2[i] = -temp[i] + M_at[i];
	}

	//J_inv*(-Omega*J*wbe + M_at) = J_inv*temp2
	for(int i=0;i<3;i++)
	{
		Temp = 0;
		for(int j=0;j<3;j++)
		{
			Temp = Temp + J_inv[i][j]*temp2[j];
		}
		wbe_dt[i] = Temp;
	}

	//--------------------------------
	//Quaternion Equation
	//-q_dt = 0.5*qOmega*q
	//--------------------------------
	for(int i=0;i<4;i++)
	{
		Temp = 0;
		for(int j=0;j<4;j++)
		{
			Temp = Temp + qOmega[i][j]*q[j];
		}
		q_dt[i] = -0.5*Temp;
	}

	x_dt[0] = pNED_dt[0];
	x_dt[1] = pNED_dt[1];
	x_dt[2] = pNED_dt[2];
	x_dt[3] = q_dt[0];
	x_dt[4] = q_dt[1];
	x_dt[5] = q_dt[2];
	x_dt[6] = q_dt[3];
	x_dt[7] = Vrel_dt[0];
	x_dt[8] = Vrel_dt[1];
	x_dt[9] = Vrel_dt[2];
	x_dt[10] = wbe_dt[0];
	x_dt[11] = wbe_dt[1];
	x_dt[12] = wbe_dt[2];

	for(int i=0;i<13;i++)
	{
		x[i] = x[i] + x_dt[i] * SAMPLING;
	}

	pNED[0] = pNED[0]+x_dt[0]*SAMPLING;
	pNED[1] = pNED[1]+x_dt[1]*SAMPLING;
	pNED[2] =  pNED[2]+x_dt[2]*SAMPLING;
	q[0] = x[3];
	q[1] = x[4];
	q[2] = x[5];
	q[3] = x[6];
	Vrel[0] = x[7];
	Vrel[1] = x[8];
	Vrel[2] = x[9];
	wbe[0] = x[10];
	wbe[1] = x[11];
	wbe[2] = x[12];

	Euler[0] = atan2(2*(q[2]*q[3] + q[0]*q[1]),  (1-2*(q[1]*q[1] + q[2]*q[2]) ));
	Euler[1] = (-asin(2*(q[1]*q[3] - q[0]*q[2])));
	Euler[2] = atan2((2*(q[0]*q[3] + q[1]*q[2])), (1-2*(q[2]*q[2]+q[3]*q[3])));

	for(int i=0;i<3;i++)
	{
		Temp = 0;
		for(int j=0;j<3;j++)
		{
			Temp = Temp + C_b2e[i][j]*Vrel[j];
		}
		vNED[i]=Temp;
	}

}