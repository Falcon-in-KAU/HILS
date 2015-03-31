/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

************************************************************************************************/
#include <stdio.h>
#include <math.h>

#define SAMPLING	0.01
#define LOOPTIME	10		//10ms = 0.01seconds

class SYSTEM
{
	private:
		//forces and moment
		double F_at[3], M_at[3];
		//Inertia matrices
		double mass, Ixx, Iyy, Izz, Ixz, gamma;
		double J[3][3], J_inv[3][3];
		//Rotational matrix between earth and body frame
		double C_b2e[3][3], C_e2b[3][3];
		//gravity matrix
		double g[3];
		//Rate matrix
		double Omega[3][3], qOmega[4][4], P, Q, R;

		double pNED_dt[3], q_dt[4], Vrel_dt[3], wbe_dt[3];
		//state derivative :: pN pE pD q0 q1 q2 q3 U V W P Q R
		double x_dt[13];

		//quaternion and norm vector
		double q0, q1, q2, q3, norm;

		

	public:
		//euler angle
		double Euler[3];
		//state variables :: pN pE pD q0 q1 q2 q3 U V W P Q R
		double x[13];
		double pNED[3], q[4], Vrel[3], wbe[3];
		
		//Calculating NED Velocity.
		double vNED[3];

		void Init(double phi, double pitch, double yaw, double U, double V, double W, double N, double E, double D);
		void System(double in[]);
		void Parameter(double *parameter);


};