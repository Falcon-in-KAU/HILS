// HILS.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HILS.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


/****************************************************************
	@Brief : Variables for Serial Connection
*****************************************************************/
#define CONNECT_BUTTON		1000
#define DISCONNECT_BUTTON	1001
#define COM_SELECTOR		1002


static HINSTANCE CONNECT_hInstance;
static HINSTANCE DISCONNECT_hInstance;
static HINSTANCE COM_SELECTOR_hInstance;

static HWND CONNECT_hWnd;
static HWND DISCONNECT_hWnd;
static HWND COM_SELECTOR_hWnd;

SERIAL Serial;
HANDLE Serial_Port;

DWORD SERIAL_THREAD;
HANDLE SERIAL_THREAD_HANDLE;

static char COM_List[10][10] = {"COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9","COM10"};
static char COM_str[128];

//for console
HWND Console_Hwnd;

/******************************************************************************
	@Brief : Variables for Simulation Thread Functions 
*******************************************************************************/


#define START_SIM_BUTTON	3000
#define END_SIM_BUTTON		3001

static HINSTANCE START_SIM_BUTTON_hInstance;
static HINSTANCE END_SIM_BUTTON_hInstance;

static HWND START_SIM_BUTTON_hWnd;
static HWND END_SIM_BUTTON_hWnd;

DWORD HILS_AIRCRAFT_SIMULATION_THREAD;
HANDLE HILS_AIRCRAFT_SIMULATION_HANDLE;

#define QUEUESIZE 500
float Queue_Lon[QUEUESIZE]={0,};
int Head_Lon = 0;
int tail_Lon = 0;
float Queue_Lat[QUEUESIZE]={0,};
int Head_Lat = 0;
int tail_Lat = 0;
float Queue_Alt[QUEUESIZE] = {0,};
int Head_Alt = 0;
int tail_Alt = 0;

double Control[4] = {0,};		//Control -> Control Surface Value. Ail, Elev, Rud, Pow
	PARAMETER HILS_Param;
	FORCE HILS_Force;
	SYSTEM HILS_System;

HWND hWnd;
MMI_OPENGL_WINDOW MMI_OpenGL_Window;
MMI_GE_WINDOW MMI_GE_Window;


unsigned long _stdcall HILS_Aircraft_Simulation(void *);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;
	
	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HILS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HILS));

	#ifdef _DEBUG
		RedirectIOToConsole();
		Console_Hwnd = GetConsoleHwnd();
		SetParent((HWND)Console_Hwnd,hWnd);
		SetWindowPos((HWND)Console_Hwnd, HWND_TOP, 600, 775,700, 275, SWP_FRAMECHANGED);
		LONG lStyle = GetWindowLong(Console_Hwnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE |WS_EX_DLGMODALFRAME);
		SetWindowLong(Console_Hwnd, GWL_STYLE, lStyle);
	#endif



	
	/*********************************************************************
		@brief : Windows for Man-Machine Interface
	**********************************************************************/
	MMI_OpenGL_Window.CreateGLWinodow("MMI_Lib",WINDOW_SIZE_X,WINDOW_SIZE_Y,hWnd);
	

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HILS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HILS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1300, 1000, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


namespace PK
{
	void KillProcess(const char *EXEName)
	{
		HANDLE snapshot_handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE != snapshot_handle)
		{
			PROCESSENTRY32 pe;
			if(Process32First(snapshot_handle, &pe))
			{
				do
				{
					//TRACE("KillProcess() PID = %04u, FileName = %s\n", pe.th32ProcessID, pe.szExeFile);
					if (!_tcscmp(pe.szExeFile, (LPCTSTR)EXEName))
					{
						HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
						if (INVALID_HANDLE_VALUE != process_handle)
						{
							//TRACE("프로세스 중지!\n");
							TerminateProcess(process_handle, 0);
							CloseHandle(process_handle);
						}
					}
				}
				while(Process32Next(snapshot_handle, &pe));
			}
			CloseHandle(snapshot_handle);
			snapshot_handle = INVALID_HANDLE_VALUE;
		} 
	}
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	switch (message)
	{
	case WM_CREATE:
		LOAD_GE_hWnd = CreateWindow("button","Load GE",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600,750,80,25,hWnd,(HMENU)LOAD_GE, LOAD_GE_Instance,NULL);
		
		CONNECT_hWnd=CreateWindow("button","Link",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0,750,50,25,hWnd,(HMENU)CONNECT_BUTTON, CONNECT_hInstance,NULL);
		
		//Disconnect button
		DISCONNECT_hWnd=CreateWindow("button","Delink",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		130,750,50,25,hWnd,(HMENU)DISCONNECT_BUTTON, DISCONNECT_hInstance,NULL);
		EnableWindow(DISCONNECT_hWnd, FALSE );

		//COM port combo box
		COM_SELECTOR_hWnd = CreateWindow("combobox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			CBS_DROPDOWNLIST,50,750,80,25,hWnd,(HMENU)COM_SELECTOR,COM_SELECTOR_hInstance,NULL);
		for (int i=0;i<10;i++)
		{
			SendMessage(COM_SELECTOR_hWnd,CB_ADDSTRING,0,(LPARAM)COM_List[i]);
		}

		//Executing Aircraft Simulation
		START_SIM_BUTTON_hWnd = CreateWindow("button","START SIM",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0,800,100,25,hWnd,(HMENU)START_SIM_BUTTON, START_SIM_BUTTON_hInstance,NULL);
		END_SIM_BUTTON_hWnd = CreateWindow("button","END SIM",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100,800,80,25,hWnd,(HMENU)END_SIM_BUTTON, END_SIM_BUTTON_hInstance,NULL);
		EnableWindow(END_SIM_BUTTON_hWnd, FALSE );
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case LOAD_GE:
			MMI_GE_Window.CreateMapWindow("MMI_GE",WINDOW_SIZE_X+83,WINDOW_SIZE_Y,hWnd);
			
			break;
		
		case CONNECT_BUTTON:
			printf("Attempt to link with serial communication system...\r\n");
			EnableWindow(CONNECT_hWnd, FALSE );
			EnableWindow(COM_SELECTOR_hWnd, FALSE);
			printf("\tLink to : %s...\r\n",COM_str);
			if(Serial_Port = Serial.OpenSerial(COM_str,"115200,n,8,1"))
			{
				printf("\tSerial communication has been linked\r\n");
				printf("\t\tCom Port : %s, 115200,n,8,1\r\n",COM_str);
				//Serial.Serial_Link_Flag = 1;
				EnableWindow(DISCONNECT_hWnd, TRUE );
				//Serial.SERIAL_THREAD_HANDLE = CreateThread(NULL,0,COMM_Serial_Window.Serial_Thread,NULL,0,&COMM_Serial_Window.SERIAL_THREAD);

			}
			else
			{
				EnableWindow(CONNECT_hWnd, TRUE );
				EnableWindow(COM_SELECTOR_hWnd, TRUE);
				printf("\tFailed to link with serial communication system\r\n");
			}
			break;
		case COM_SELECTOR:
			switch(HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				i=SendMessage(COM_SELECTOR_hWnd, CB_GETCURSEL,0,0);
				SendMessage(COM_SELECTOR_hWnd, CB_GETLBTEXT, i, (LPARAM)COM_str);
				SetWindowText(hWnd, COM_str);
				break;
			}
			break;
		case DISCONNECT_BUTTON:
			printf("Disconnect serial communication\r\n");
			Serial.Close();
			EnableWindow(CONNECT_hWnd, TRUE );
			EnableWindow(DISCONNECT_hWnd, FALSE );
			EnableWindow(COM_SELECTOR_hWnd, TRUE);
			//printf("\tSerial thread has been terminated\r\n");
			
			break;
		case START_SIM_BUTTON:
			EnableWindow(START_SIM_BUTTON_hWnd, FALSE );
			EnableWindow(END_SIM_BUTTON_hWnd, TRUE );
			printf("Start Simulation\r\n");
			/*********************************************************************
				@brief : Thread for Aircraft Simulation 
			**********************************************************************/
			HILS_AIRCRAFT_SIMULATION_HANDLE = CreateThread(NULL,0,HILS_Aircraft_Simulation,NULL,0,&HILS_AIRCRAFT_SIMULATION_THREAD);
			break;

		case END_SIM_BUTTON:
			EnableWindow(START_SIM_BUTTON_hWnd, TRUE );
			EnableWindow(END_SIM_BUTTON_hWnd, FALSE );
			memset(&Queue_Lon[0],0,QUEUESIZE);
			memset(&Queue_Lat[0],0,QUEUESIZE);
			memset(&Queue_Alt[0],0,QUEUESIZE);
			Head_Lon = 0;
			tail_Lon = 0;
			Head_Lat = 0;
			tail_Lat = 0;
			Head_Alt = 0;
			tail_Alt = 0;
			TerminateThread(HILS_AIRCRAFT_SIMULATION_HANDLE,0);
			printf("End Simulation\r\n");
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PK::KillProcess("googleearth.exe");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


/****************************************************************
	@Brief : Thread for Aircraft Simulation
*****************************************************************/


void GOOGLEEARTH_TrajUpdate(float Lat, float Long, float pD)
{
	Queue_Lon[0] = (Long);
	Queue_Lat[0] = (Lat);
	Queue_Alt[0] = -(pD);
	for(int i=QUEUESIZE-1;i>-1;i--)
	{
		Queue_Lon[i+1]=Queue_Lon[i];
		Queue_Lat[i+1]=Queue_Lat[i];
		Queue_Alt[i+1]=Queue_Alt[i];
		//printf("%d\r\n",i);
	}
}

unsigned long _stdcall HILS_Aircraft_Simulation(void *)
{
	//PARAMETER HILS_Param;
	//FORCE HILS_Force;
	//SYSTEM HILS_System;

	double Parameter[40] = {0,};
	HILS_Param.Para(&Parameter[0]);
	HILS_Force.Para(&Parameter[0]);
	HILS_System.Parameter(&Parameter[0]);
	HILS_System.Init(-10,0,0,11,0,0,0,0,-200);
	Control[0] = 0;
	Control[1] = 0;
	Control[2] = 0;
	Control[3] = 0;
	
	DWORD Tick_cnt = 0;
	DWORD Delay_Time = 0;
	int count = 0;



	while(1)
	{
		Tick_cnt = clock();
		HILS_Force.Wind(0,0,0);
		HILS_Force.Compute_forces(HILS_System.q,HILS_System.Vrel,HILS_System.wbe,&Control[0]);
		HILS_System.System(HILS_Force.out);
		MMI_OpenGL_Window.HUD_Data.TAS = HILS_Force.Vt;
		MMI_OpenGL_Window.HUD_Data.GS = sqrt(HILS_System.vNED[0]*HILS_System.vNED[0]+HILS_System.vNED[1]*HILS_System.vNED[1]);
		MMI_OpenGL_Window.HUD_Data.Altitude = -HILS_System.pNED[2];
		MMI_OpenGL_Window.HUD_Data.theta = HILS_System.Euler[1]*R2D;
		MMI_OpenGL_Window.HUD_Data.phi = -HILS_System.Euler[0]*R2D;
		MMI_OpenGL_Window.HUD_Data.Heading = HILS_System.Euler[2]*R2D;
		if(MMI_OpenGL_Window.HUD_Data.Heading<0)
			MMI_OpenGL_Window.HUD_Data.Heading = MMI_OpenGL_Window.HUD_Data.Heading+360;
		//MMI_GE_Window.
		MMI_OpenGL_Window.HUD_Data.Latitude = 57.2958/6370217.6*HILS_System.pNED[0]+MMI_GE_Window.Lat_Base;
		MMI_OpenGL_Window.HUD_Data.Longitude = 57.2958/6370217.6/cos(MMI_OpenGL_Window.HUD_Data.Latitude*0.0174532925)*HILS_System.pNED[1]+MMI_GE_Window.Long_Base;
		if(count==10)
		{
			GOOGLEEARTH_TrajUpdate(MMI_OpenGL_Window.HUD_Data.Latitude,MMI_OpenGL_Window.HUD_Data.Longitude,-HILS_System.pNED[2]);
			MMI_KML kml;
			CString KML_Data;
			KML_Data = kml.Generate_KML_Database(&Queue_Lat[0],&Queue_Lon[0],&Queue_Alt[0],QUEUESIZE);
			FILE *fp;
			if(NULL == (fp=fopen("C:\\Program Files (x86)\\Google\\Google Earth Pro\\GCS1.kml","wt")))
			{
				CreateDirectory("C:\\Program Files (x86)\\Google\\Google Earth Pro",NULL);
				fp = fopen("C:\\Program Files (x86)\\Google\\Google Earth Pro\\GCS1.kml","wt");
			}
			fprintf(fp,"%s",KML_Data);
			fclose(fp);
			KML_Data = kml.Generate_Aircraft(-(-MMI_OpenGL_Window.HUD_Data.phi),-MMI_OpenGL_Window.HUD_Data.theta,MMI_OpenGL_Window.HUD_Data.Heading,Queue_Lat[0],Queue_Lon[0],-Queue_Alt[0]);
			//FILE *fp;
			if(NULL == (fp=fopen("C:\\Program Files (x86)\\Google\\Google Earth Pro\\Bombadier.kml","wt")))
			{
				CreateDirectory("C:\\Program Files (x86)\\Google\\Google Earth Pro",NULL);
				fp = fopen("C:\\Program Files (x86)\\Google\\Google Earth Pro\\Bombadier.kml","wt");
			}
			fprintf(fp,"%s",KML_Data);
			fclose(fp);
			count = 0;

		}

		if(Delay_Time = clock()-Tick_cnt<LOOPTIME)
		{
			Sleep(LOOPTIME-Delay_Time);
		}
		else
		{
			Sleep(0);
		}
		count++;
	}

	return 0;
}