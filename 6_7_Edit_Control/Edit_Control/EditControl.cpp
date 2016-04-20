#include "EditControl.h"
#include "resource2.h"
CEditControl::CEditControl()
{
	handle = 0;
}

CEditControl::~CEditControl()
{
}

bool CEditControl::Create(HWND hWndParent)
{
	static TCHAR szWindowClass[] = L"win32app";
	static TCHAR szTitle[] = L"Win32 Applacation";

	handle = CreateWindowEx(
		0,
		L"Edit",
		NULL,
		WS_CHILD | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		hWndParent,
		NULL,
		GetModuleHandle(0),
		0
		);
	HRSRC resourceHandle = FindResource(GetModuleHandle(0), MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");
	HGLOBAL handleData = LoadResource(GetModuleHandle(0), resourceHandle);
	LPVOID data = LockResource(handleData);
	SetWindowText(handle, reinterpret_cast<LPCWSTR>(data));

	SetFocus(handle);
	return (handle != 0);
}

void CEditControl::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND CEditControl::GetHandle()
{
	return handle;
}
