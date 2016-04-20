#include "OverlappedWindow.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

COverlappedWindow::COverlappedWindow() {
}

COverlappedWindow::~COverlappedWindow() {
}

bool COverlappedWindow::RegisterClass() {
	WNDCLASSEX windowClass;
	
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = COverlappedWindow::windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = (LPCSTR)L"OverlappedWindow";
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&windowClass)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			NULL,
			NULL);
		return 1;
	}
	return 0;
}

bool COverlappedWindow::Create() {
	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Win32 Applacation");

	handle = CreateWindowEx(
		WS_EX_TOPMOST,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL
		);
	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)this);
	return (handle != 0);
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		return 0;
	case WM_SIZE:
		return 0;
	case WM_DESTROY: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->onDestroy();
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}

void COverlappedWindow::onDestroy() {

}