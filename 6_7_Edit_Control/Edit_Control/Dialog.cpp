#include "Dialog.h"
#include "NotepadWindow.h"

Dialog::Dialog() 
{
	handle = 0;
	preview = 0;
	parentHandle = 0;
	editControlHandle = 0;
}

Dialog::~Dialog()
{
}

bool Dialog::Create(HWND _parentHandle)
{
	parentHandle = _parentHandle;
	CNotepadWindow* parentWindow = reinterpret_cast<CNotepadWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	editControlHandle = parentWindow->getEditControlHandle();
	INT_PTR error_code = ::DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 
		parentHandle, dialogProc, reinterpret_cast<LPARAM>(this));
	ShowWindow(handle, SW_SHOW);
	return (error_code != -1);
}

void Dialog::OnInit(HWND _handle)
{
	LOGFONT logFont;
	handle = _handle;
	preview = 0;
	CNotepadWindow* parentWindow = reinterpret_cast<CNotepadWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETRANGEMIN, true, 8);
	SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETRANGEMAX, true, 72);
	//SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETPOS, true, reinterpret_cast<LPARAM>(newSettings.font));
	SendDlgItemMessage(handle, IDC_SLIDER2, TBM_SETRANGEMIN, true, 0);
	SendDlgItemMessage(handle, IDC_SLIDER2, TBM_SETRANGEMAX, true, 255);
	actualFont = reinterpret_cast<HFONT>(SendMessage(editControlHandle, WM_GETFONT, 0, 0));
	GetObject(actualFont, sizeof(logFont), &logFont);
	newSettings.font = logFont;
	newSettings.transparency = parentWindow->GetTransparency();
	newSettings.textColor = parentWindow->GetTextColor();
	newSettings.bgColor = parentWindow->GetBgColor();
	oldSettings = newSettings;
	SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETPOS, true, (LPARAM)logFont.lfHeight);
	SendDlgItemMessage(handle, IDC_SLIDER2, TBM_SETPOS, true, newSettings.transparency);
}

void Dialog::OnDestroy()
{
}

HWND Dialog::GetHandle()
{
	return handle;
}

void Dialog::updateSettings(const Dialog_Data& settings)
{
	CNotepadWindow* parentWindow = reinterpret_cast<CNotepadWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	DeleteObject(actualFont);
	actualFont = CreateFontIndirect(&settings.font);
	SendMessage(editControlHandle, WM_SETFONT, reinterpret_cast<WPARAM>(actualFont), true);
	SetLayeredWindowAttributes(parentHandle, 0, settings.transparency, LWA_ALPHA);
	parentWindow->SetColors(settings.bgColor, settings.textColor);
	parentWindow->SetTransparency(settings.transparency);
}

void Dialog::selectColor(DWORD& targetColor) {
	CHOOSECOLOR choosecolor = {};
	choosecolor.lStructSize = sizeof(CHOOSECOLOR);
	choosecolor.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	choosecolor.lpCustColors = chosenPalitre;
	choosecolor.rgbResult = targetColor;
	::ChooseColor(&choosecolor);
	targetColor = choosecolor.rgbResult;
}

void Dialog::OnScroll(WPARAM wParam, LPARAM lParam)
{
	HWND commandHandle = reinterpret_cast<HWND>(lParam);
	if (commandHandle == GetDlgItem(handle, IDC_SLIDER1)) {
		newSettings.font.lfHeight = SendDlgItemMessage(handle, IDC_SLIDER1, TBM_GETPOS, 0, 0);
	}
	else if (commandHandle == GetDlgItem(handle, IDC_SLIDER2)) {
		newSettings.transparency = (BYTE)SendDlgItemMessage(handle, IDC_SLIDER2, TBM_GETPOS, 0, 0);
	}
	if (preview) {
		updateSettings(newSettings);
	}
}

INT_PTR Dialog::OnCommand(WPARAM wParam)
{
	switch LOWORD(wParam) {
		case IDC_BUTTON2: 
		{
			selectColor(newSettings.textColor);
			break;
		}
		case IDC_BUTTON1:
		{
			selectColor(newSettings.bgColor);
			break;
		}
		case IDC_CHECK1:
		{
			if (IsDlgButtonChecked(handle, IDC_CHECK1) == BST_CHECKED) {
				preview = 1;
			}
			else {
				preview = 0;
			}
			break;
		}
		case IDOK:
		{
			updateSettings(newSettings);
			EndDialog(handle, wParam);
			OnDestroy();
			return true;
		}
		case IDCANCEL:
		{
			updateSettings(oldSettings);
			EndDialog(handle, wParam);
			OnDestroy();
			return true;
		}
	}

	if (preview) {
		updateSettings(newSettings);
	}
	return false;
}

INT_PTR Dialog::dialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_INITDIALOG == uMsg) {
		Dialog* dialog = reinterpret_cast<Dialog*>(lParam);
		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG>(dialog));
		dialog->OnInit(hwndDlg);
		return true;
	}

	Dialog* dialog = reinterpret_cast<Dialog*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
	switch (uMsg){
		case WM_COMMAND:
		{
			return dialog->OnCommand(wParam);
		}
		case WM_HSCROLL:
		{
			dialog->OnScroll(wParam, lParam);
			return false;
		}
		case WM_CLOSE:
		{			 
			return true;
		}
	}
		return false;
}