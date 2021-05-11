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
	
}

void QBertGame::Run()
{
	
}
