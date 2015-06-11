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

/******************************************************************************
	@Brief : Variables for Simulation Thread Functions 
*******************************************************************************/
DWORD HILS_AIRCRAFT_SIMULATION_THREAD;
HANDLE HILS_AIRCRAFT_SIMULATION_HANDLE;

HWND hWnd;
MMI_OPENGL_WINDOW MMI_OpenGL_Window;
MMI_GE_WINDOW MMI_GE_Window;
PARAMETER HILS_Param;
FORCE HILS_Force;
SYSTEM HILS_System;
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

	/*********************************************************************
		@brief : Thread for Aircraft Simulation 
	**********************************************************************/
	HILS_AIRCRAFT_SIMULATION_HANDLE = CreateThread(NULL,0,HILS_Aircraft_Simulation,NULL,0,&HILS_AIRCRAFT_SIMULATION_THREAD);

	/*********************************************************************
		@brief : Windows for Man-Machine Interface
	**********************************************************************/
	MMI_OpenGL_Window.CreateGLWinodow("MMI_Lib",WINDOW_SIZE_X,WINDOW_SIZE_Y,hWnd);
	MMI_GE_Window.CreateMapWindow("MMI_GE",WINDOW_SIZE_X+83,WINDOW_SIZE_Y,hWnd);

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
      CW_USEDEFAULT, 0, 1300, 800, NULL, NULL, hInstance, NULL);

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

	switch (message)
	{
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
unsigned long _stdcall HILS_Aircraft_Simulation(void *)
{
	

	double Control[4] = {0,};		//Control -> Control Surface Value. This will be replaced.
	double Parameter[40] = {0,};
	HILS_Param.Para(&Parameter[0]);
	HILS_Force.Para(&Parameter[0]);
	HILS_System.Parameter(&Parameter[0]);
	HILS_System.Init(0,0,0,11.1,0,0,0,0,-1000);

	DWORD Tick_cnt = 0;
	DWORD Delay_Time = 0;
	while(1)
	{
		Tick_cnt = clock();
		HILS_Force.Compute_forces(HILS_System.q,HILS_System.Vrel,HILS_System.wbe,&Control[0]);
		HILS_System.System(HILS_Force.out);
		if(Delay_Time = clock()-Tick_cnt<LOOPTIME)
		{
			Sleep(LOOPTIME-Delay_Time);
		}
		else
		{
			Sleep(0);
		}

	}

	return 0;
}