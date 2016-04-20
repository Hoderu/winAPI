#pragma once
#include <windows.h>
#include "EllipseWindow.h"
#include <array>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	//Register class of window
	static bool RegisterClass();

	//Create item of window
	bool Create();
	//Show window
	void Show(int cmdShow);

protected:
	void onNCCreate(HWND handle);
	void onDestroy();
	void onCreate();
	void onSize();

private:
	//handle of window
	HWND handle;
	std::array<CEllipseWindow, 4> childWindows;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	
};