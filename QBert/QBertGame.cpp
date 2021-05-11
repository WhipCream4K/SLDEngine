#include "QBertGame.h"

QBertGame::QBertGame(const std::any& windowHandle)
{
	windowHandle;
}

QBertGame::QBertGame(HWND windowHandle)
{
	m_Framework.CreateViewPortFromHWND(windowHandle);

}

void QBertGame::Start()
{
	// Map generation

	// HUD generation

}

void QBertGame::Run()
{
	// input layer
	bool isQuit{};
	while (!((isQuit = m_Framework.TranslateUserInputs())))
	{
		

		// World Updates


		// Async update and render
		m_Framework.Step();
	}


}
