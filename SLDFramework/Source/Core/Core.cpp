#include "Core.h"
#include "Window/Window.h"

#include "../Miscellaneous/DX12Resource.h"

bool SLD::Core::Init()
{
	SLD_LOG_INIT();

#ifdef HAS_DX12
	const bool shouldUseWARP{};
	Instance<DX12Resource>().Init(shouldUseWARP);
#endif

	return true;
}


SLD::Core::Core()
{
	SLD_LOG_INIT();
}

bool SLD::Core::CreateViewPortFromHWND(const windowHandle& anyWindowHandle)
{
#ifdef _WIN32
	if (m_AlreadyHaveViewPort)
		return false;

	m_AlreadyHaveViewPort = true;
	HWND windowHandle{ std::any_cast<HWND>(anyWindowHandle) };

	// Get window dimension
	RECT windowRect{};

	if (::GetWindowRect(windowHandle, &windowRect) == FALSE)
		return false;

	const uint32_t windowWidth{ uint32_t(windowRect.right - windowRect.left) };
	const uint32_t windowHeight{ uint32_t(windowRect.bottom - windowRect.top) };

	// Get window title
	const int windowTitleLength{ ::GetWindowTextLengthW(windowHandle) };
	const int maxAllocatedSize{ 256 };
	wchar_t windowTitle[maxAllocatedSize]{};
	::GetWindowTextW(windowHandle, windowTitle, windowTitleLength + 1);

	// convert wide char to char from MSDN
	//	https://docs.microsoft.com/en-us/cpp/text/how-to-convert-between-various-string-types?view=msvc-160

	// Allocate two bytes in the multibyte output string for every wide
	// character in the input string (including a wide character
	// null). Because a multibyte character can be one or two bytes,
	// you should allot two bytes for each character. Having extra
	// space for the new string is not an error, but having
	// insufficient space is a potential security problem.

	const size_t newStrSize{ size_t((windowTitleLength + 1)) * 2 };
	char* strWinTitle{ new char[newStrSize] };

	wcstombs_s(nullptr, strWinTitle, newStrSize, &windowTitle[0], _TRUNCATE);

	std::string strWindowTitle{ strWinTitle,newStrSize };

	delete[] strWinTitle;

	
	LLWindow windowSubSystem{
#ifdef HAS_SFML
		SFMLWindow{anyWindowHandle,windowWidth,windowHeight}
#endif
	};


	LLInputs inputSubSystem{
#ifdef HAS_SFML
		SFMLInputs{windowHandle}
		//SFMLInputs{RefPtr<SFMLWindow>{std::get_if<SFMLWindow>(&windowSubSystem),[](SFMLWindow*){}}}
		//SFMLInputs{std::get_if<SFMLWindow>(&windowSubSystem)}
#endif
	};


	m_MainViewPort = std::make_shared<Window>(windowWidth, windowHeight,
		std::move(windowSubSystem),
		std::move(inputSubSystem),
		anyWindowHandle,
		strWindowTitle);

	m_MainRenderer.SetRenderWindow(m_MainViewPort);

	if (!m_GuiDebugger.AttachDrawWindow(m_MainViewPort))
		return false;
#endif

	return true;
}

#ifdef CAN_CREATE_WINDOW_HANDLE
bool SLD::Core::CreateNewViewPort(uint32_t width, uint32_t height, const std::string& winName)
{
	if (m_AlreadyHaveViewPort)
		return false;

#ifdef _WIN32

	::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Create win class
	WNDCLASSEX wc{};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = nullptr;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)::GetModuleHandleW(NULL);
	wc.hIcon = ::LoadIconW(wc.hInstance, nullptr);
	wc.hCursor = ::LoadCursorW(wc.hInstance, nullptr);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;

	const wchar_t* className{ L"SLD_FRAMEWORK" };
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(wc.hInstance, nullptr);

	::RegisterClassExW(&wc);


	// Create "Real" window
	// convert char to wchar_t from MSDN
	//	https://docs.microsoft.com/en-us/cpp/text/how-to-convert-between-various-string-types?view=msvc-160

	const size_t wideCharSize{ winName.size() + 1 };
	wchar_t* windowTitleName{ new wchar_t[wideCharSize] };
	mbstowcs_s(nullptr, windowTitleName, wideCharSize, winName.c_str(), _TRUNCATE);

	const int userScreenWidth{ ::GetSystemMetrics(SM_CXSCREEN) };
	const int userScreenHeight{ ::GetSystemMetrics(SM_CYSCREEN) };

	const int windowX{ std::max(0,(userScreenWidth - int(width)) / 2) };
	const int windowY{ std::max(0,(userScreenHeight - int(height)) / 2) };

	HWND hWnd{
		::CreateWindowExW(
			0,
			m_WindowClass.lpszClassName,
			windowTitleName,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			int(width),
			int(height),
			NULL,
			NULL,
			wc.hInstance,
			nullptr
			)
	};

	delete[] windowTitleName;

	// assign
	m_WindowHandle = hWnd;
	m_WindowClass = wc;

	::ShowWindow(hWnd, 0);
	::UpdateWindow(hWnd);

#endif

	//	LLInputs inputSubSystem{
	//#ifdef HAS_SFML
	//		SFMLInputs{hWnd}
	//#endif
	//	};

//	auto windowSubSystem{
//#ifdef HAS_SFML
//		SFMLWindow{hWnd,width,height}
//#endif
//	};

	LLWindow windowSubSystem{
#ifdef HAS_SFML
		SFMLWindow{hWnd,width,height}
#endif
	};

	
	LLInputs inputSubSystem{
#ifdef HAS_SFML
		//SFMLInputs{RefPtr<SFMLWindow>{std::get_if<SFMLWindow>(&windowSubSystem),[](SFMLWindow*){}}}
		//SFMLInputs{std::get_if<SFMLWindow>(&windowSubSystem)}
		SFMLInputs{hWnd}
#endif
	};

	m_MainViewPort = std::make_shared<Window>(width, height,
		std::move(windowSubSystem),
		std::move(inputSubSystem),
		hWnd,
		winName);

	m_MainRenderer.SetRenderWindow(m_MainViewPort);

	if (!m_GuiDebugger.AttachDrawWindow(m_MainViewPort))
		return false;

	return true;
}

#endif

bool SLD::Core::TranslateUserInputs()
{
	m_WorldEntity.StartWorldTime();

	const bool isQuit{ m_MainViewPort->PollUserInputs()};
	if (!isQuit)
	{
		const auto userEvData{ m_MainViewPort->GetInputData() };
		ShouldResizeWindow(m_MainViewPort, userEvData);

		// World Input interaction
		for (int i = 0; i < userEvData.eventCntThisFrame; ++i)
		{
			m_WorldEntity.GetWorldInputSetting().ParseMessage(userEvData.events[i]);
		}
	}

	return isQuit;
}

void SLD::Core::Step()
{
	// *** UPDATE WORLD ***


	// JOIN THREADS

	// *** Render ***
	m_MainViewPort->ClearBackBuffer();

	auto& allRenderComponent{ m_WorldEntity.GetAllRenderingComponentsEditable() };
	m_MainRenderer.Render(allRenderComponent);

	m_MainViewPort->Present();

	m_WorldEntity.EndWorldTime();
}

SLD::WorldEntity& SLD::Core::GetDefaultWorldEntity()
{
	return m_WorldEntity;
}

SLD::Core::~Core()
{
#ifdef CAN_CREATE_WINDOW_HANDLE
	if (m_DoesFrameworkOwnWindowHandle)
	{
		::DestroyWindow(m_WindowHandle);
		::UnregisterClassW(m_WindowClass.lpszClassName, m_WindowClass.hInstance);
	}
#endif
}

void SLD::Core::ShouldResizeWindow(const RefPtr<Window>& window, const EventQueueHandle& userEv)
{
	for (int i = 0; i < userEv.eventCntThisFrame; ++i)
	{
		auto& evData{ userEv.events[i].data };
		switch (userEv.events[i].eventId)
		{
		case uint8_t(InputParams::WindowEventType::WE_Resized):
		{
			window->Resize(evData.windowSize.width, evData.windowSize.height);
			break;
		}
		}
	}
}

