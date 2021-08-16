#pragma once

// This is for the user to include the necessary components in the framework

#include "Core/Base.h"
#include "Core/Core.h"
#include "GameObject/Prefab.h"
#include "GameObject/GameObject.h"
#include "Inputs/InputParams.h"

using SLDFramework = SLD::Core;
using SLDWorldEntity = SLD::WorldEntity;

//#ifdef SLD_APPLICATION_CLASS
//
//inline SharedPtr<SLD_APPLICATION_CLASS> CreateApplication()
//{
//	return std::make_shared<SLD_APPLICATION_CLASS>();
//}
//
//#endif

#ifdef SLD_PROVIDE_MAIN

BOOL InitWindowInstance(HINSTANCE hInstance, HWND& windowHandle);
ATOM MyRegisterClass(HINSTANCE hInstance);

#ifdef _WIN32

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	
	MyRegisterClass(hInstance);

	HINSTANCE hInst = (HINSTANCE)GetModuleHandleW(NULL)
	
	HWND windowHandle{};
	if (!InitWindowInstance(hInstance, windowHandle))
		return FALSE;

#ifdef SLD_APPLICATION_CLASS

	const auto myApp = CreateApplication();

	myApp->OnAppBegin();

	myApp->InitCore(windowHandle);

	::ShowWindow(windowHandle, nCmdShow);

	myApp->OnGameConstruct();

	MSG msg{};
	bool isQuit{};
	while(!isQuit)
	{
		while(::PeekMessageW(&msg,nullptr,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				isQuit = true;
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}

		myApp->UpdateCore();
	}

#endif
	return TRUE;
}

#endif

inline LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

inline ATOM MyRegisterClass(HINSTANCE hInstance)
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
	wcex.lpszClassName = EngineContext::windowClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);

	return RegisterClassExW(&wcex);
}

inline BOOL InitWindowInstance(HINSTANCE hInstance, HWND& windowHandle)
{
	/* Screen dimension of the main display*/
	const int screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
	const int screenHeight{ GetSystemMetrics(SM_CYSCREEN) };

	const auto windowClassName{ EngineContext::windowClassName };

#ifdef SLD_APPLICATION_CLASS

	char const* applicationName{ typeid(SLD_APPLICATION_CLASS).name() };
	const size_t newSize{ strlen(applicationName) + 1 };
	wchar_t* windowTitleName{ new wchar_t[newSize] };
	mbstowcs_s(nullptr, windowTitleName, newSize, applicationName, _TRUNCATE);

#else

	const wchar_t* windowTitleName = L"SLD_FRAMEWORK_WINDOW";

#endif

	RECT windowRect{ 0,0,
		static_cast<LONG>(EngineContext::WindowWidth),
	static_cast<LONG>(EngineContext::WindowHeight) };

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const int windowWidth{ windowRect.right - windowRect.left };
	const int windowHeight{ windowRect.bottom - windowRect.top };

	/* Center the window within the screen.               */
	const int windowX = std::max(0, (screenWidth - windowWidth) / 2);
	const int windowY = std::max(0, (screenHeight - windowHeight) / 2);

	windowHandle =
		CreateWindowExW(
			0,
			windowClassName,
			windowTitleName,
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

	delete[] windowTitleName;

	if (!windowHandle)
		return FALSE;

	return TRUE;
}


#endif