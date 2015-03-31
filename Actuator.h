

class ACTUATOR
{
	private:
		float yn[3];

	public:
		void Servo(float wn, float zeta, double u, double *y);

};