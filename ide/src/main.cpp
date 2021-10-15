#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_MOUSEMOVE:
		POINT p;
		GetCursorPos(&p);
		std::cout << "Mouse position: x: " << p.x << ", y: " << p.y << std::endl;
		break;
	case WM_PAINT: {
		RECT canvas;
		GetClientRect(hwnd, &canvas);
		HBRUSH brush = CreateSolidBrush(RGB(40, 40, 40));
		HDC windowDc = GetDC(hwnd);
		FillRect(windowDc, &canvas, brush);
		ReleaseDC(hwnd, windowDc);
		ValidateRect(hwnd, 0);
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
	return 0;
}



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");
	freopen_s(&fp, "CONOUT$", "w", stdout);



	WNDCLASSEX wc = {0};
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "Window";

	RegisterClassEx(&wc);


	hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		"WinAPI window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);


	if (hwnd == NULL) { __asm {int 3} }


	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, hwnd, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}