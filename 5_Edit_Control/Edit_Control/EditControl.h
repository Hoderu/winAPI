#pragma once
#include <windows.h>
#include <array>

class CEditControl {
public:
	CEditControl();
	~CEditControl();

	//Create item of window
	bool Create(HWND hWndParent);
	//Show window
	void Show(int cmdShow);
	HWND GetHandle();

private:
	//handle of window
	HWND handle;
};