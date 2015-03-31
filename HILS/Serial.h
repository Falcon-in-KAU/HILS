/***********************************************************************************************
					6 Degree of Aircraft's Equation of motion simulation
								Programmed by Yongrae Kim
										
										  belong to
					Flight Control Laboratory, LAB building room 327, Korea Aerospace Univ.

									E-mail : s_vettel@naver.com
											 kimyr911125@gmail.com

************************************************************************************************/

#include <windows.h>
#include <windowsX.h>/*HANDLE_MSG*/
#include <stdio.h>
#include <conio.h>

class SERIAL
{
	// This Serial To Recieve and Send Packet by Serial Port

	public:
		HANDLE OpenSerial(char *ComPort, char *Setting);
		
		DWORD WriteComm(unsigned char *pBuff, DWORD nToWrite, HANDLE PORT);
		DWORD ReadComm(unsigned char *pBuff, DWORD nToRead, HANDLE PORT);
		void Close();
		
		SERIAL()
		{
			OsWrite.hEvent = NULL;
			OsRead.hEvent = NULL;
			m_hComm = NULL;
		}


	private:
			OVERLAPPED OsWrite,OsRead;
			HANDLE m_hComm;
};
