#pragma once

#include <SLDFramework.h>

#include <any>
class QBertGame
{
public:
	
	QBertGame(const std::any& windowHandle);
	QBertGame(HWND windowHandle);

	void Start();
	void Run();

private:
	
	SLDFramework m_Framework;
};

