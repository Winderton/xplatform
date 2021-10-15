#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <dwrite.h>
#include <d2d1.h>


#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")


static ID2D1Factory* d2dFactory;
static ID2D1HwndRenderTarget* renderTarget;
static IDWriteFactory* writeFactory;
static IDWriteTextFormat* textFormat;
static ID2D1SolidColorBrush* textBrush;

static const WCHAR code[] = L"#include <iostream>\n\n int main(int argc, char** argv)\n {\n \treturn 0;\n }\n\ ";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hresult;
	
	switch (msg)
	{
	case WM_SIZE:
		if (renderTarget) renderTarget->Release();
		RECT canvas;
		GetClientRect(hwnd, &canvas);
		//text dimensions
		D2D1_SIZE_U textSize = D2D1::SizeU(
			textSize.width = canvas.right - canvas.left,
			textSize.height = canvas.bottom - canvas.top
			);
		hresult = d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, textSize), &renderTarget);
		if (textBrush) { textBrush->Release();}
		hresult = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &textBrush);
	case WM_MOUSEMOVE:
		POINT p;
		GetCursorPos(&p);
		std::cout << "Mouse position: x: " << p.x << ", y: " << p.y << std::endl;
		break;
	case WM_PAINT: {
		//gpu
		renderTarget->BeginDraw();
		D2D1::ColorF clr(0.3f, 0.3f, 0.9f, 0);
		renderTarget->Clear(clr);
		D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();
		renderTarget->DrawText(code, ARRAYSIZE(code) - 1, textFormat, D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), textBrush);

		hresult = renderTarget->EndDraw();
		ValidateRect(hwnd, 0);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
#if 0
		//gdi
		RECT canvas;
		GetClientRect(hwnd, &canvas);
		HBRUSH brush = CreateSolidBrush(RGB(40, 40, 40));
		HDC windowDc = GetDC(hwnd);
		FillRect(windowDc, &canvas, brush);
		ReleaseDC(hwnd, windowDc);
		ValidateRect(hwnd, 0);
		break;
	}
#endif
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
return 0;
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");
	freopen_s(&fp, "CONOUT$", "w", stdout);

	HRESULT hresult;
	hresult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	hresult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	hresult = writeFactory->CreateTextFormat(L"Consolas", 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.0F, L"en-us", &textFormat);

	WNDCLASSEX wc = {};
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