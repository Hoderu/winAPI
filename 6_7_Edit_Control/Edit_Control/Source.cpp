#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "NotepadWindow.h"
#include "resource2.h"

int WINAPI wWinMain(
	HINSTANCE instance,
	HINSTANCE prevInstance,
	LPWSTR commandLine,
	int cmdShow)
{

	//const ATOM atom = ::RegisterClassEx(&windowClass);

	if (!CNotepadWindow::RegisterClass()) {
		return -1;
	}
	CNotepadWindow window;
	if (!window.Create()) {
		return -1;
	}

	//HWND hwnd;
	window.Show(cmdShow);
	HACCEL accelerator = LoadAccelerators(instance, MAKEINTRESOURCE(IDR_ACCELERATOR2));
	MSG message;
	BOOL getMessageResult = 0;
	while ((getMessageResult = ::GetMessage(&message, 0, 0, 0)) != 0) {
		if (getMessageResult == -1) {
			return -1;
		}
		if (!TranslateAccelerator(window.getHandle(), accelerator, &message) &&
			!IsDialogMessage(window.getDialogHandle(), &message)) {
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
}