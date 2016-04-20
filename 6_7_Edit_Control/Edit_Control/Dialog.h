#pragma once
#include <windows.h>
#include "resource2.h"
#include <Commctrl.h>

struct Dialog_Data {
	LOGFONT font;
	BYTE transparency;
	DWORD textColor, bgColor;
};

class Dialog {
public:
	Dialog();
	~Dialog();
	bool Create(HWND _parentHandle);
	void OnInit(HWND _handle);
	INT_PTR OnCommand(WPARAM wParam);
	void OnDestroy();
	HWND GetHandle();
	void OnScroll(WPARAM wParam, LPARAM lParam);

private:
	HWND handle, parentHandle, editControlHandle;
	bool preview;
	Dialog_Data oldSettings, newSettings;
	HFONT actualFont;
	COLORREF chosenPalitre[16];

	void updateSettings(const Dialog_Data& settings);
	void selectColor(DWORD& targetColor);

	static INT_PTR CALLBACK dialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};