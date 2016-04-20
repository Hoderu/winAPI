#pragma once
#include <windows.h>
#include <array>
#include "EditControl.h"

class CNotepadWindow {
public:
	CNotepadWindow();
	~CNotepadWindow();

	//Register class of window
	static bool RegisterClass();

	//Create item of window
	bool Create();
	//Show window
	void Show(int cmdShow);
	HWND getHandle();

protected:
	void onNCCreate(HWND handle);
	void onDestroy();
	void onCreate();
	bool onClose();
	void onSize();
	void onCommand(WPARAM wParam, LPARAM lParam);
	void onCommandMenu(WPARAM wParam, LPARAM lParam);

private:
	//handle of window
	HWND handle;
	CEditControl editControl;
	bool isWritten;
	HICON hIcon;

	void saveText();

	static const int maxFileNameSize = 1024;
	static const int nBufferMax = 1024;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

};