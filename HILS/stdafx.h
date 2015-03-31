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

/****************************************************************
*****************************************************************/
#define PI 3.141592654
