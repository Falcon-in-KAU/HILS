/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

************************************************************************************************/

#include "stdafx.h"

void PARAMETER::Para(double *Parameter)
{


	int swit=0;
	//scanf("%d",&swit);
	
	//pulsar
	if(swit == 0)
	{
		printf("\tpulsar selected\n");
			Parameter[0]			=	2.765;		//mass
			Parameter[1]			=	0.4604;		//Ixx
			Parameter[2]			=	0.23283;	//Iyy
			Parameter[3]			=	0.6781;		//Izz
			Parameter[4]			=	-0.0094;	//Ixz
		
			Parameter[5]			=	0.6977;		//Area
			Parameter[6]			=	0.25;		//Chord
			Parameter[7]			=	3.23;		//Wingspan
			Parameter[8]			=	0.09;		//Xcg
			Parameter[9]			=	0.09;		//Xnp *********
	
			Parameter[10]			=	0.38015;		//CLo
			Parameter[11]			=	5.706;		//CLa
			Parameter[12]			=	0;			//CLadot
			Parameter[13]			=	6.752;		//CLq
	
			Parameter[14]			=	0.291;		//CLde
			Parameter[15]			=	0.0;		//CLdf
		
			Parameter[16]			=	-0.2466;	//CYb
			Parameter[17]			=	-0.2722;	//CYp
			Parameter[18]			=	0.1748;		//CYr
			Parameter[19]			=	0.1417;		//CYdr
		
			Parameter[20]			=	-0.1701;	//Clb
			Parameter[21]			=	-0.6474;	//Clp
			Parameter[22]			=	0.1386;		//Clr
			Parameter[23]			=	-0.423653;	//Clda
			Parameter[24]			=	0.003049;	//Cldr
	
			Parameter[25]			=	0.01591;		//CMo
			Parameter[26]			=	-0.6516;	//CMa
			Parameter[27]			=	0;			//CMadot
			Parameter[28]			=	-13.0916;	//CMq
			Parameter[29]			=	-1.0896;	//CMde
			Parameter[30]			=	0;			//CMdf
	
			Parameter[31]			=	0.04127;		//CNb
			Parameter[32]			=	-0.0527;	//CNp
			Parameter[33]			=	-0.0358;	//CNr
			Parameter[34]			=	-0.0451;	//CNdr
			Parameter[35]			=	0.0048;		//CNda
		
			Parameter[36]			=	0.03;		//CDo
	}
	

	//X-6 3.3m
	if(swit==2)
	{
		printf("\t\tHuman Powered Aircraft!!!\n");
			Parameter[0]			=	110;		//mass
			Parameter[1]			=	969.979;		//Ixx
			Parameter[2]			=	115.63;		//Iyy
			Parameter[3]			=	1041.746;		//Izz
			Parameter[4]			=	-12.331;		//Ixz

			Parameter[5]			=	25.082;		//Area
			Parameter[6]			=	1.0123;		//Chord
			Parameter[7]			=	26;		//Wingspan
			Parameter[8]			=	0.1;		//Xcg
			Parameter[9]			=	0.122;		//X

			Parameter[10]			=	0.936;		//CLo
			Parameter[11]			=	5.854322;		//CLa
			Parameter[12]			=	0;			//CLadot
			Parameter[13]			=	0.032409;		//CLq

			Parameter[14]			=	0.196191;		//CLde
			Parameter[15]			=	0.0;		//CLdf

			Parameter[16]			=	-0.288217;	//CYb
			Parameter[17]			=	-0.158775;	//CYp
			Parameter[18]			=	0.207499;		//CYr
			Parameter[19]			=	0.263440;		//CYdr

			Parameter[20]			=	-0.123215;	//Clb
			Parameter[21]			=	-0.74689;	//Clp
			Parameter[22]			=	0.413121;		//Clr
			Parameter[23]			=	0.0;	//Clda
			Parameter[24]			=	-0.005338;	//Cldr
		
			Parameter[25]			=	-0.01069;		//CMo
			Parameter[26]			=	-0.600407;	//CMa
			Parameter[27]			=	0;			//CMadot
			Parameter[28]			=	-5.198454;	//CMq
			Parameter[29]			=	-0.636787;	//CMde
			Parameter[30]			=	0;			//CMdf

			Parameter[31]			=	0.022810;		//CNb
			Parameter[32]			=	-0.158405;	//CNp
			Parameter[33]			=	-0.031031;	//CNr
			Parameter[34]			=	-0.045372;	//CNdr
			Parameter[35]			=	0.0;		//CNda

			Parameter[36]			=	0.015;		//CDo
	}
  

//X-6 3.6
	if(swit == 1)
	{
		printf("\t\t\tX-6 3.6m selected!!!\n");
		Parameter[0]			=	3.3;		//mass
		Parameter[1]			=	0.9398;		//Ixx
		Parameter[2]			=	0.572;		//Iyy
		Parameter[3]			=	0.1614;		//Izz
		Parameter[4]			=	-0.0069;	//Ixz

		Parameter[5]			=	0.9405;		//Area
		Parameter[6]			=	0.275;		//Chord
		Parameter[7]			=	3.6;		//Wingspan
		Parameter[8]			=	0.11;		//Xcg
		Parameter[9]			=	0.129;		//X

		Parameter[10]			=	0.68;		//CLo
		Parameter[11]			=	5.896812;		//CLa
		Parameter[12]			=	0;			//CLadot
		Parameter[13]			=	7.851893;		//CLq

		Parameter[14]			=	0.239158;		//CLde
		Parameter[15]			=	0.0;		//CLdf
	
		Parameter[16]			=	-0.123352;	//CYb
		Parameter[17]			=	-0.037054;	//CYp
		Parameter[18]			=	0.091589;		//CYr
		Parameter[19]			=	0.085064;		//CYdr
	
		Parameter[20]			=	-0.0895;	//Clb
		Parameter[21]			=	-0.746808;	//Clp
		Parameter[22]			=	0.204009;		//Clr
		Parameter[23]			=	-0.620583;	//Clda
		Parameter[24]			=	0.000753;	//Cldr

		Parameter[25]			=	0.05389;		//CMo
		Parameter[26]			=	-0.626405;	//CMa
		Parameter[27]			=	0;			//CMadot
		Parameter[28]			=	-16.834267;	//CMq
		Parameter[29]			=	-1.092748;	//CMde
		Parameter[30]			=	0;			//CMdf

		Parameter[31]			=	0.0222645;		//CNb
		Parameter[32]			=	-0.065456;	//CNp
		Parameter[33]			=	-0.030009;	//CNr
		Parameter[34]			=	-0.030491;	//CNdr
		Parameter[35]			=	0.029591;		//CNda

		Parameter[36]			=	0.025;		//CDo
	}

//X-6 : 3.6m
	if(swit == 3)
	{
		printf("\t\t\tX-6 3.6m selected!!!\n");
			Parameter[0]			=	3.2;		//mass
			Parameter[1]			=	1.56;		//Ixx
			Parameter[2]			=	0.113;		//Iyy
			Parameter[3]			=	1.651;		//Izz
			Parameter[4]			=	-0.016;		//Ixz

			Parameter[5]			=	1.045;		//Area
			Parameter[6]			=	0.26556;		//Chord
			Parameter[7]			=	3.92;		//Wingspan
			Parameter[8]			=	0.095;		//Xcg
			Parameter[9]			=	0.101;		//X

			Parameter[10]			=	0.70608;		//CLo
			Parameter[11]			=	5.785075;		//CLa
			Parameter[12]			=	0;			//CLadot
			Parameter[13]			=	6.484882;		//CLq

			Parameter[14]			=	0.517030;		//CLde
			Parameter[15]			=	0.0;		//CLdf

			Parameter[16]			=	-0.473758;	//CYb
			Parameter[17]			=	-0.246627;	//CYp
			Parameter[18]			=	0.229797;		//CYr
			Parameter[19]			=	0.293745;		//CYdr

			Parameter[20]			=	-0.299220;	//Clb
			Parameter[21]			=	-0.663684;	//Clp
			Parameter[22]			=	0.201161;		//Clr
			Parameter[23]			=	-0.479013 ;	//Clda
			Parameter[24]			=	0.02963;	//Cldr
		
			Parameter[25]			=	0.00714;		//CMo
			Parameter[26]			=	-0.141843;	//CMa
			Parameter[27]			=	0;			//CMadot
			Parameter[28]			=	-4.032738;	//CMq
			Parameter[29]			=	-0.909703;	//CMde
			Parameter[30]			=	0;			//CMdf

			Parameter[31]			=	0.019455;		//CNb
			Parameter[32]			=	-0.084888;	//CNp
			Parameter[33]			=	-0.021893;	//CNr
			Parameter[34]			=	-0.046106;	//CNdr
			Parameter[35]			=	0.022229
;		//CNda

			Parameter[36]			=	0.015;		//CDo
	}		

}