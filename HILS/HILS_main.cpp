// HILS.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "HILS.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HILS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
							//TRACE("���μ��� ����!\n");
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		// �޴� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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