// QBert.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "QBertGame.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef _DEBUG
#include <vld.h>
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define WINMAIN

#ifdef WINMAIN

BOOL InitWindowInstance(HINSTANCE hInstance, HWND& windowHandle);
ATOM MyRegisterClass(HINSTANCE hInstance, const std::wstring& className);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(hInstance);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	const std::wstring className{ L"QBert_SLDFRAMEWORK" };
	
	MyRegisterClass(hInstance,className);

	//SLD_LOG_TRACE_HEAP_ALLOC(10);
	//SLD_LOG_TRACE_HEAP_ALLOC(10);
	//
	HWND windowHandle {};
	if (!InitWindowInstance(hInstance, windowHandle))
		return FALSE;

	::ShowWindow(windowHandle, nCmdShow);
	::UpdateWindow(windowHandle);

	QBertGame game{ windowHandle };

	game.Start();
	//game.Run();
	
	MSG msg{};
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		game.Run();
	}
	
	::DestroyWindow(windowHandle);
	UnregisterClass(className.c_str(), hInstance);
	
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance,const std::wstring& className)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, nullptr);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = className.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);

	return RegisterClassExW(&wcex);
}

BOOL InitWindowInstance(HINSTANCE hInstance, HWND& windowHandle)
{
	/* Screen dimension of the main display*/
	const int screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
	const int screenHeight{ GetSystemMetrics(SM_CYSCREEN) };

	const auto windowClassName{ L"QBert" };


	RECT windowRect{ 0,0,
		static_cast<LONG>(1280),
	static_cast<LONG>(720) };

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const int windowWidth{ windowRect.right - windowRect.left };
	const int windowHeight{ windowRect.bottom - windowRect.top };

	/* Center the window within the screen.               */
	const int windowX = std::max(0, (screenWidth - windowWidth) / 2);
	const int windowY = std::max(0, (screenHeight - windowHeight) / 2);

	windowHandle =
		CreateWindowEx(
			0,
			L"QBert_SLDFRAMEWORK",
			windowClassName,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			hInstance,
			nullptr
		);
	
	if (!windowHandle)
		return FALSE;

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
