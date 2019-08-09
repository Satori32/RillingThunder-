#include <windows.h>

#include "WindowClass.h"
#include "WndProc.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window W;
	Window::SharedEventHandler WE = std::make_shared<RollingThunderProc>();
	W.SetEventHandler(WE);

	const TCHAR Name[] = _T("RollingThander!!");
	const TCHAR Title[] = _T("RollingThander!!");

	W.Create(hInstance, Name, Title, 800, 600);

	W.ShowWindow(nCmdShow);
	W.UpdateWindow();

	return W.PeekMessageLoop(0,0,0);

}