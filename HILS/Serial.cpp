#include "Serial.h"

DWORD SERIAL::ReadComm(unsigned char *pBuff, DWORD nToRead, HANDLE PORT)
{
	ReadFile(PORT,pBuff,nToRead,&nToRead,NULL);
	return nToRead ;
}

DWORD SERIAL::WriteComm(unsigned char *pBuff, DWORD nToWrite, HANDLE PORT)
{
	DWORD dwWritten, dwError, dwErrorFlags;
	COMSTAT comstat;
	int n;
	
	n = WriteFile(PORT, pBuff, nToWrite, &dwWritten,&OsWrite);
	
	if(!n)
	{
		if(GetLastError() == ERROR_IO_PENDING )
		{
			while( !GetOverlappedResult(PORT,&OsWrite,&dwWritten,TRUE) )
			{
				dwError = GetLastError();   
				if(dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(PORT,&dwErrorFlags,&comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(PORT,&dwErrorFlags,&comstat);
		}
				
	}

	return dwWritten;

}

HANDLE SERIAL::OpenSerial(char *ComPort, char *Setting)
{
	
		m_hComm = CreateFile(ComPort,
							 GENERIC_READ | GENERIC_WRITE,
							 0,
							 NULL,
							 OPEN_EXISTING,
							 NULL,
							 NULL);
				
		if( m_hComm == (HANDLE)-1 ) 
			   return FALSE;

		SetCommMask(m_hComm,EV_RXCHAR);
		SetupComm(m_hComm,4096,4096);

		PurgeComm(m_hComm,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		COMMTIMEOUTS cto;

		cto.ReadIntervalTimeout = 1;
		cto.ReadTotalTimeoutConstant = 1;
		cto.ReadTotalTimeoutMultiplier = 1;
				
		if (!SetCommTimeouts(m_hComm, &cto))
			return 0;

		DCB dcb;

	FillMemory(&dcb, sizeof(dcb), 0); 
	   dcb.DCBlength = sizeof(dcb); 
	   if (!BuildCommDCB(Setting, &dcb)) // "19200,n,8,1"
   		  return FALSE; 
	   else
	   {
	   		dcb.fOutxCtsFlow = false;					// Disable CTS monitoring
			dcb.fOutxDsrFlow = false;					// Disable DSR monitoring
			dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR monitoring
			dcb.fOutX = false;							// Disable XON/XOFF for transmission
			dcb.fInX = false;							// Disable XON/XOFF for receiving
			dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)

		  SetCommState(m_hComm, &dcb);
		}

    
	return m_hComm;
}

void SERIAL::Close()
{
	if(OsWrite.hEvent)
		CloseHandle(OsWrite.hEvent);

	if(OsRead.hEvent)
		CloseHandle(OsRead.hEvent);
		
	if(m_hComm)
		CloseHandle(m_hComm);
}
	