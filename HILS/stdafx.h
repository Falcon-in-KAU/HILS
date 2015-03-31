#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

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
