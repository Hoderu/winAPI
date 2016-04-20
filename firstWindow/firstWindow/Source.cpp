#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "OverlappedWindow.h"
/*
struct WNDCLASSEX {
UINT cbSize;
UINT style;
WNDPROC lpfnWndProc;
int cbClsExtra;
int cbWndExtra;
HINSTANCE hInstance;
HICON hIcon;
HCURSOR hCursor;
HBRUSH hbrBackground;
LPCWSTR lpszMenuName;
LPCWSTR lpszClassName;
HICON hIconSm;
};

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
case WM_CREATE:
return 0;
case WM_PAINT:
return 0;
case WM_SIZE:
return 0;
case WM_DESTROY:
return 0;
default:
return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
return 0;
}

HWND WINAPI CreateWindow(
//DWORD dwExStyle,
LPCTSTR lpClassName,
LPCTSTR lpWindowName,
DWORD dwStyle,
int x,
int y,
int nWidth,
int nHeight,
HWND hWndParent,
HMENU hMenum,
HINSTANCE hInstance,
LPVOID lpParam
);
*/
/*
BOOL WINAPI ShowWindow(
HWND hWnd,
int nCmdShow);
*/
int WINAPI wWinMain(
	HINSTANCE instance,
	HINSTANCE prevInstance,
	LPWSTR commandLine,
	int cmdShow)
{

	//const ATOM atom = ::RegisterClassEx(&windowClass);

	if ( !COverlappedWindow::RegisterClass() ) {
		return -1;
	}
	COverlappedWindow window;
	if( !window.Create() ) {
		return -1;
	}

	//HWND hwnd;
	window.Show( cmdShow );

	MSG message;
	BOOL getMessageResult = 0;
	while ( ( getMessageResult = ::GetMessage( &message, 0, 0, 0 ) ) != 0 ) {
		if ( getMessageResult == -1 ) {
			return -1;
		}
		::TranslateMessage( &message );
		::DispatchMessage( &message );
	}
}