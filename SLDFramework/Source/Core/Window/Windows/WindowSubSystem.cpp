#include "WindowSubSystem.h"

#include "../../Miscellaneous/WinPrerequisite.h"

class SLD::WindowSubSystem::ImpleWindowSubSystem
{
public:

	void Present();
	void ClearBackBuffer(const float(&clearColor)[4]);
	
private:


	
};

void SLD::WindowSubSystem::ImpleWindowSubSystem::ClearBackBuffer(const float(& clearColor)[4])
{
	
}

void SLD::WindowSubSystem::Present()
{
	
}

void SLD::WindowSubSystem::ClearBackBuffer(const float(&clearColor)[4])
{
	m_pImpleWindow->ClearBackBuffer(clearColor);
}
