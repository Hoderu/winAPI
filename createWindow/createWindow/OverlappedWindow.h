#pragma once
#include <windows.h>

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
	void onDestroy();

private:
	HWND handle; //handle of window

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};