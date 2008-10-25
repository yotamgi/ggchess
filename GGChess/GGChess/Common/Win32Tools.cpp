#include "Win32Tools.h"

/**
 * The function that makes the window and the stuff
 */
HWND Win32Tools::InitWindow(HINSTANCE hinstance, WNDPROC WindowProc,
							  int width, int height, HBRUSH bgColor,
							  char Capitian[])
{
	if(!RegisterThisClass(hinstance, WindowProc, bgColor)) {
		MessageBoxA(0, "Register class failed!", "Error", MB_OK);
		exit(0);
	}

	HWND hwnd = CreateWnd(hinstance, Capitian);

	if(!hwnd) {
		MessageBoxA(0, "Create Window failed!", "Error", MB_OK);
		exit(0);
	}

	ShowWindow(hwnd, TRUE);
	UpdateWindow(hwnd);

	return hwnd;
}

BOOL Win32Tools::RegisterThisClass(HINSTANCE hinstance, WNDPROC CurrWindowProc, HBRUSH bgColor)
{
	USES_CONVERSION;

	// fill the wndclass
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);									// size of structure 
	wcx.style = CS_HREDRAW |  CS_VREDRAW;						// redraw if size changes 
	wcx.lpfnWndProc = CurrWindowProc;							// points to window procedure 
    wcx.cbClsExtra = 0;											// no extra class memory 
    wcx.cbWndExtra = 0;											// no extra window memory 
    wcx.hInstance = hinstance;									// handle to instance 
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
    wcx.hbrBackground = bgColor	;						    	// white background brush 
    wcx.lpszMenuName =  NULL;									// name of menu resource 
    wcx.lpszClassName = A2W("MyWindowClass");					// name of window class 
	wcx.hIconSm = (HICON)LoadImage(hinstance,					// small class icon 
							MAKEINTRESOURCE(5),
							IMAGE_ICON, 
							GetSystemMetrics(SM_CXSMICON), 
							GetSystemMetrics(SM_CYSMICON), 
							LR_DEFAULTCOLOR); 
 
    // Register the window class. 
	return (BOOL)RegisterClassEx(&wcx);
}

HWND Win32Tools::CreateWnd(HINSTANCE hinstance, char captain[])
{
	USES_CONVERSION;
	// Create the window
	return CreateWindow(A2W("MyWindowClass"), A2W(captain),
							  WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
							  NULL, NULL, hinstance, NULL);
}

LRESULT CALLBACK Win32Tools::BasicWindowProc(HWND hwnd,
					UINT msg,
					WPARAM wParam,
					LPARAM lParam)
{	
	// This is the main evant handeler supplied by me!
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_CLOSE:
		DestroyWindow(hwnd);
	break;	

	default: return (DefWindowProc(hwnd, msg, wParam, lParam));

	}
	return 0;

}

void Win32Tools::MainLoop()
{
	MSG m;
	while (GetMessage(&m, NULL, 0, 0))
	{
		TranslateMessage(&m);
		DispatchMessage(&m);
	}
}