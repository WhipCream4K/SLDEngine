
#ifndef SLDFRAMEWORK_SFMLSUBSYSTEM_H
#define SLDFRAMEWORK_SFMLSUBSYSTEM_H

#include "../../Core/Base.h"
#include "../../Inputs/InputParams.h"


class SFMLWindow
{
public:
	
	class ImpleSFMLWindow;
	
	SFMLWindow(const std::any& windowHandle,uint32_t width,uint32_t height);

	SFMLWindow(const SFMLWindow&) = delete;
	SFMLWindow& operator=(const SFMLWindow&) = delete;
	SFMLWindow(SFMLWindow&& other) noexcept;
	SFMLWindow& operator=(SFMLWindow&& other) noexcept;

	SLD::InputParams::ReadOut ReadUserInputs();
	void Resize(uint32_t width,uint32_t height);
	void ClearColor(const float(&clearColor)[4]);
	void Present(bool shouldVSync);
	void Close();

private:

	OwnedPtr<ImpleSFMLWindow> m_pMainWindow{};
};



#endif
