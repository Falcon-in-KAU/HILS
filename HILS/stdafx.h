#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/*****************************************************************
	@Brief : Header file for Hardware-In-the-Loop Simulation
	@Note  : Only for the fixed wing aircraft
*****************************************************************/
#include "HILS_Forces.h"
#include "HILS_System.h"
#include "HILS_Actuator.h"
#include "HILS_Parameter.h"

/*****************************************************************
	@Brief : Header file declaration for serial communication system
*****************************************************************/
#include "HILS_Serial.h"

/************************************************************************************
	@Brief : Header files and library filesdeclaration for other functionalities
*************************************************************************************/
#include <atlstr.h>		//for Using CString
#include <mmsystem.h>  // for Using mciSendString() 
#include <conio.h> 
#include <math.h>
#include <time.h>
#pragma comment (lib, "winmm.lib") //MCI 
#include <atlstr.h>						//for using CString
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

/*****************************************************************
	@Brief : library file declaration for console windows
*****************************************************************/

/*****************************************************************
	@Brief : Header files for OpenGL
*****************************************************************/
#include "OpenGL\GL.H"
#include "OpenGL\GLU.H"
#include "OpenGL\glut.h"
/*****************************************************************
	@Brief : Header files for Man-Machine Interface
*****************************************************************/
#include "MMI_Opengl_Window.h"
#include "MMI_Opengl_HUD.h"
#include "MMI_Google_Earth_Window.h"
#include "MMI_Google_Earth_Control.h"
#include "MMI_Google_Earth.h"
#include "guicon.h"

/****************************************************************
*****************************************************************/
#define PI 3.141592654
#define WINDOW_SIZE_X	600
#define WINDOW_SIZE_Y	700

#define R2D		57.2957795
#define D2R		0.0174532925
