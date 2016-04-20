#include "NotepadWindow.h"
#include <windows.h>
#include <ctime>
#include "resource2.h"

CNotepadWindow::CNotepadWindow()
{
	editControl = CEditControl();
}

CNotepadWindow::~CNotepadWindow()
{
}

bool CNotepadWindow::RegisterClass()
{
	WNDCLASSEX windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = CNotepadWindow::windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	windowClass.lpszClassName = L"NotepadWindow";
	windowClass.hIconSm = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0));
	
	if (!RegisterClassEx(&windowClass)) {
		MessageBox(NULL,
			L"Call to RegisterClassEx failed!",
			NULL,
			NULL);
		return 0;
	}

	/*LPTSTR lpszFilePath = (LPTSTR)"C:\\fff.exe";
	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSize(lpszFilePath, &dwDummy);
	LPBYTE lpVersionInfo = new BYTE[dwFVISize];
	GetFileVersionInfoEx(NULL, lpszFilePath, 0, dwFVISize, lpVersionInfo);
	*/
	//DestroyIcon(windowClass.hIcon);
	return 1;
}

bool CNotepadWindow::Create()
{
	static TCHAR szWindowClass[] = L"win32app";
	static TCHAR szTitle[] = L"Win32 Application";
	LPTSTR lpBuffer = new wchar_t[nBufferMax];
	LoadString(GetModuleHandle(0), IDS_STRING102, lpBuffer, nBufferMax);
	//hIcon = LoadIcon(NULL, MAKEINTRESOURCE(201));

	handle = CreateWindowEx(
		0,
		L"NotepadWindow",
		lpBuffer,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1000, 500,
		NULL,
		NULL,
		GetModuleHandle(0),
		this
		);
	return (handle != 0);
}

void CNotepadWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	editControl.Show(cmdShow);
}

void CNotepadWindow::onNCCreate(HWND _handle)
{
	handle = _handle;
}

void CNotepadWindow::onCreate()
{
	editControl.Create(handle);
	isWritten = false;
}

void CNotepadWindow::onSize()
{
	RECT rect;
	GetClientRect(handle, &rect);
	SetWindowPos(editControl.GetHandle(), HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, 0);
}

void CNotepadWindow::saveText()
{
	int length = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t *text = new wchar_t[length + 1];
	SendMessage(editControl.GetHandle(), WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text));
	wchar_t fileName[maxFileNameSize];
	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = fileName;
	ofn.hwndOwner = handle;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLEHOOK;
	if (GetSaveFileName(&ofn)) {
		HANDLE file = CreateFile(fileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		DWORD a;
		WriteFile(file, text, length * sizeof(wchar_t), &a, 0);
		CloseHandle(file);
	}
	delete[] text;
}

HWND CNotepadWindow::getHandle() {
	return handle;
}

bool CNotepadWindow::onClose()
{
	if ( isWritten ) {
		int msgBoxID = MessageBox(handle, L"Save text?", L"Window", MB_YESNOCANCEL);
		switch ( msgBoxID ) {
			case IDYES: {
				saveText();
				return true;
			}
			case IDNO: {
				return true;
			}
			case IDCANCEL: {
				return false;
			}
		}
	}
}

void CNotepadWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	switch HIWORD(wParam) {
		case EN_CHANGE: {
			isWritten = true;
		}
		case 0: {
			onCommandMenu(wParam, lParam);
			break;
		}
		case 1: {
			switch LOWORD(wParam) {
				case ID_ACCELERATOR40003: {
					SendMessage(handle, WM_DESTROY, 0, 0);
					break;
				}
			}
		}
	}
}

void CNotepadWindow::onCommandMenu(WPARAM wParam, LPARAM lParam)
{
	switch LOWORD(wParam) {
	case ID_FILE_SAVE40005: {
						   saveText();
						   break;
	}
	case ID_FILE_EXIT40006: {
			SendMessage(handle, WM_CLOSE, 0, 0);
			break;
		}
		

	}
}

void CNotepadWindow::onDestroy()
{
	PostQuitMessage(0);
}

LRESULT CNotepadWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCCREATE == message) {
		CNotepadWindow* window = reinterpret_cast< CNotepadWindow* > (((CREATESTRUCT*)lParam)->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		window->onNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}

	CNotepadWindow* window = reinterpret_cast< CNotepadWindow* > (GetWindowLongPtr(handle, GWLP_USERDATA));
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
	case WM_CLOSE: {
		if (window->onClose()) {
			return DefWindowProc(handle, message, wParam, lParam);
		}
		return 0;
	}
	case WM_COMMAND: {
		window->onCommand(wParam, lParam);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}