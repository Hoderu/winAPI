#pragma once
#include <windows.h>

class CEllipseWindow {
public:
    CEllipseWindow();
	~CEllipseWindow();

	//Register class of window
	static bool RegisterClass();

	//Create item of window
	bool Create(HWND hWndParent);
	//Show window
	void Show(int cmdShow);

	HWND getHandle();

protected:
	void onDestroy();
	void onPaint();
	void onTimer();
	void onNCCreate(HWND handle);
	void onCreate();
	void onLButtonDown();

private:
	//handle of window
	HWND handle;
	RECT rect;
	UINT_PTR timer;
	double coordinateX;
	double coordinateY;
	double deltaX;
	double deltaY;
	int r;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void drawEllipse(HDC hdc, int x, int y);
};