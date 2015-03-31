/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

					class Force : computing forces and moments effects to the aircraft
************************************************************************************************/

#include <math.h>

#define rho		1.225		//Air density

class FORCE
{
	private:
		//Control
		double Ail, Elev, Rud, Pow;

		//Thrust
		double To;

		//variables on the calculation of wind effect
		double alpha, beta;
		double wNED[3], wUVW[3], dUVW[3];
		double q0, q1, q2, q3;
		double Ps, Qs, Rs, PQRs[3];

		//aircraft parameters
		double mass, Ixx, Iyy, Izz, Ixz;
		double Area, Chord, Wingspan, Xcg, X;

		//variables using at calculating forces and moments
		double pres;					//air pressure
		double CLo, CLa, CLadot, CLq, CLde, CLdf;
		double CYb, CYp, CYr, CYdr;
		double Clb, Clp, Clr, Clda, Cldr;
		double CMo, CMa, CMadot, CMq, CMde, CMdf;
		double CNb, CNp, CNr, CNdr, CNda;
		double CDo;
		double CL, CD, CX, CY, CZ, Cl, Cm, Cn;
		
		//Complete transformation matrix
		double ctm[3][3];

		//Rotational transformation matrix between earth frame and body frame
		double C_e2b[3][3], C_b2e[3][3];
		
		//Rotational rates in stability axis
		double C_s2b[3][3], C_b2s[3][3];

		double wForce[3], wMoment[3];
		double Force[3], Moment[3];
		

	public:
		double Vt;
		double out[6];
		double SpecA[3];
		double AoA;
		double AoB;

		void Wind(double N, double E, double D);
		void Compute_forces(double *quat, double *UVW, double *PQR, double *Control);
		void Para(double *Parameter);

};