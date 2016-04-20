#pragma once
#include <windows.h>
#include <array>
#include "EditControl.h"
#include "Dialog.h"

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
	HWND getEditControlHandle();
	HWND getDialogHandle();

	COLORREF GetBgColor();
	COLORREF GetTextColor();
	BYTE GetTransparency();
	void SetColors(COLORREF bgColor, COLORREF textColor);
	void SetTransparency(BYTE transparancy);

protected:
	void onNCCreate(HWND handle);
	void onDestroy();
	void onCreate();
	bool onClose();
	void onSize();
	void onCommand(WPARAM wParam, LPARAM lParam);
	void onCommandMenu(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlColorEdit(HDC hdc);

private:
	//handle of window
	HWND handle;
	CEditControl editControl;
	Dialog dialogControl;
	bool isWritten;
	HICON hIcon;
	COLORREF textColor, bgColor;
	BYTE transparency;
	HBRUSH activeBrush;

	void saveText();

	static const int maxFileNameSize = 1024;
	static const int nBufferMax = 1024;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

};