/***********************************************************************************************
							Discrete Actuator Model
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com
************************************************************************************************/

class ACTUATOR
{
	private:
		float yn[3];

	public:
		void Servo(float wn, float zeta, double u, double *y);

};