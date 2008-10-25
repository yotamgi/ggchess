#ifndef __BASIC_WND_PROG__
#define __BASIC_WND_PROG__

#include <windows.h>
#include <ATLBase.h>
#include <math.h>

namespace  Win32Tools {

	LRESULT CALLBACK BasicWindowProc(HWND hWnd,
					UINT msg,
					WPARAM wParam,
					LPARAM lParam);

	HWND InitWindow(HINSTANCE hinstance, 
				WNDPROC WindowProc = BasicWindowProc,
				int width = 800, int height = 600, HBRUSH bgColor = (HBRUSH)GetStockObject(WHITE_BRUSH),
				char Capitian[] = "Basic Window");
	
	BOOL RegisterThisClass(HINSTANCE hinstance, WNDPROC CurrWindowProc, HBRUSH bgColor);

	HWND CreateWnd(HINSTANCE hinstance, char captain[]);

	void MainLoop();
};


#endif __BASIC_WND_PROG__