#include "EllipseWindow.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

CEllipseWindow::CEllipseWindow()
{
	deltaX = 1;
	deltaY = 1;
	r = 50;
}

CEllipseWindow::~CEllipseWindow()
{
}

bool CEllipseWindow::RegisterClass()
{
	WNDCLASSEX windowClass = {};

	windowClass.cbSize = sizeof( WNDCLASSEX );
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = CEllipseWindow::windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hIcon = LoadIcon( NULL, MAKEINTRESOURCE(IDI_APPLICATION) );
	windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	windowClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"EllipseWindow";
	windowClass.hIconSm = LoadIcon( windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION) );

	if ( !RegisterClassEx( &windowClass ) ) {
		MessageBox( NULL,
			L"Call to RegisterClassEx failed!",
			NULL,
			NULL );
		return 0;
	}
	return 1;
}

bool CEllipseWindow::Create(HWND hWndParent)
{
	static TCHAR szWindowClass[] =  L"win32app";
	static TCHAR szTitle[] = L"Win32 Applacation";

	handle = CreateWindowEx( 
		WS_EX_TOPMOST,
		L"EllipseWindow",
		szTitle,
		WS_CHILD | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		hWndParent,
		NULL,
		GetModuleHandle( 0 ),
		this
		);
	return ( handle != 0 );
}

void CEllipseWindow::Show(int cmdShow)
{
	ShowWindow( handle, cmdShow );
}

HWND CEllipseWindow::getHandle() 
{
	return handle;
}

void CEllipseWindow::onNCCreate(HWND _handle)
{
	handle = _handle;
}

void CEllipseWindow::drawEllipse(HDC hdc, int x, int y)
{
	HBRUSH innBrush = CreateSolidBrush(RGB(0, 0, 255));
	if (::GetFocus() == handle) {
		innBrush = CreateSolidBrush(RGB(255, 0, 0));
	}
	else {
		innBrush = CreateSolidBrush(RGB(0, 255, 0));
	}
	HGDIOBJ object = SelectObject(hdc, innBrush);
	Ellipse(hdc, x - r, y + r, x + r, y - r);
	SelectObject(hdc, object);
	DeleteObject(innBrush);
}

void CEllipseWindow::onPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	GetClientRect(handle, &rect);
	
	HDC displayBufferDC = CreateCompatibleDC(hdc);
	HBITMAP displayBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	HGDIOBJ object = SelectObject(displayBufferDC, displayBuffer);
	DeleteObject(object);
	
	int x = coordinateX;
	int y = coordinateY;
	if (x + r >= rect.right) {
		deltaX = (-1)*deltaX;
		coordinateX = rect.right - r;
	}
	if (x - r <= rect.left) {
		deltaX = (-1)*deltaX;
		coordinateX = rect.left + r;
	}
	if (y + r >= rect.bottom) {
		deltaY = (-1)*deltaY;
		coordinateY = rect.bottom - r;
	}
	if (y - r <= rect.top) {
		deltaY = (-1)*deltaY;
		coordinateY = rect.top + r;
	}

	FillRect(displayBufferDC, &rect, reinterpret_cast<HBRUSH> (COLOR_WINDOW + 1));
	drawEllipse(displayBufferDC, x, y);
	
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, displayBufferDC, 0, 0, SRCCOPY);
	DeleteObject(displayBuffer);
	DeleteObject(displayBufferDC);
	
	EndPaint(handle, &ps);
}

void CEllipseWindow::onDestroy()
{
	KillTimer(handle, timer);
	PostQuitMessage(0);
}

void CEllipseWindow::onTimer()
{
	GetClientRect(handle, &rect);
	coordinateX += deltaX;
	coordinateY += deltaY;
	InvalidateRect(handle, &rect, FALSE);
}

void CEllipseWindow::onLButtonDown()
{
	SetFocus(handle);
}

void CEllipseWindow::onCreate()
{
	timer = SetTimer(handle, 0, 5, 0);
}

LRESULT CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCCREATE == message) {
		CEllipseWindow* window = reinterpret_cast< CEllipseWindow* > (((CREATESTRUCT*)lParam)->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		window->onNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	
	CEllipseWindow* window = reinterpret_cast< CEllipseWindow* > (GetWindowLongPtr(handle, GWLP_USERDATA));
	switch (message) {
	case WM_CREATE: {
		window->coordinateX = window->r;
		window->coordinateY = window->r;
		window->onCreate();
		return 0;
	}
	case WM_PAINT: {
		window->onPaint();
		return 0;
	}
	case WM_TIMER: {
		window->onTimer();
		return 0;
	}
	case WM_SIZE:
		return 0;
	case WM_DESTROY: {
		window->onDestroy();
		return 0;
	}
	case WM_LBUTTONDOWN: {
		window->onLButtonDown();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}