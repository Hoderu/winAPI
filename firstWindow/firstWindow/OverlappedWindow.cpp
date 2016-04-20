#include "OverlappedWindow.h"
#include "EllipseWindow.h"
#include <windows.h>
#include <ctime>

COverlappedWindow::COverlappedWindow()
{
	handle = 0;
	for (int i = 0; i < childWindows.size(); i++) {
		childWindows[i] = CEllipseWindow();
	}
}

COverlappedWindow::~COverlappedWindow()
{
}

bool COverlappedWindow::RegisterClass()
{
	WNDCLASSEX windowClass = {};

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
	windowClass.lpszClassName = L"COverlappedWindow";
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&windowClass)) {
		MessageBox(NULL,
			L"Call to RegisterClassEx failed!",
			NULL,
			NULL);
		return 0;
	}
	return 1;
}

bool COverlappedWindow::Create()
{
	static TCHAR szWindowClass[] = L"win32app";
	static TCHAR szTitle[] = L"Win32 Applacation";

	handle = CreateWindowEx(
		WS_EX_TOPMOST,
		L"COverlappedWindow",
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		GetModuleHandle(0),
		this
		);
	return (handle != 0);
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	for (int i = 0; i < childWindows.size(); i++) {
		childWindows[i].Show(cmdShow);
	}
}

void COverlappedWindow::onNCCreate(HWND _handle)
{
	handle = _handle;
}

void COverlappedWindow::onCreate()
{
	CEllipseWindow::RegisterClass();
	for (int i = 0; i < childWindows.size(); i++) {
		childWindows[i].Create(handle);
	}
}

void COverlappedWindow::onSize()
{
	RECT rect;
	int middleX, middleY, nWidth, nHeight;
	GetClientRect(handle, &rect);
	middleX = (rect.left + rect.right) / 2;
	middleY = (rect.top + rect.bottom) / 2;
	nWidth = (rect.right - rect.left) / 2;
	nHeight = (rect.bottom - rect.top) / 2;
	SetWindowPos(childWindows[0].getHandle(), HWND_TOP, rect.left, rect.top, nWidth, nHeight, 0);
	SetWindowPos(childWindows[1].getHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0);
	SetWindowPos(childWindows[2].getHandle(), HWND_TOP, rect.left, middleY, nWidth, nHeight, 0);
	SetWindowPos(childWindows[3].getHandle(), HWND_TOP, middleX, middleY, nWidth, nHeight, 0);
}

void COverlappedWindow::onDestroy() 
{
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCCREATE == message) {
		COverlappedWindow* window = reinterpret_cast< COverlappedWindow* > (((CREATESTRUCT*)lParam)->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		window->onNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}

	COverlappedWindow* window = reinterpret_cast< COverlappedWindow* > (GetWindowLongPtr(handle, GWLP_USERDATA));
	switch (message) {
	case WM_CREATE: {
		window->onCreate();
		return 0;
	}
	case WM_SIZE: {
		window->onSize();
		return 0;
	}
	case WM_DESTROY: {
		window->onDestroy();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}