#ifndef __GUICON_H__

#define __GUICON_H__

#ifdef _DEBUG

void RedirectIOToConsole();
HWND GetConsoleHwnd(void);
void ClearConsoleToColors(int ForgC, int BackC);
#endif

#endif

/* End of File */