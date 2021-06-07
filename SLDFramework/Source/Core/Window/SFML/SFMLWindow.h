
#ifndef SLDFRAMEWORK_SFMLSUBSYSTEM_H
#define SLDFRAMEWORK_SFMLSUBSYSTEM_H

#include "../../Core/Base.h"
#include "../../Inputs/InputParams.h"

namespace sf
{
	class RenderWindow;
}

class SFMLWindow
{
public:
	
	class ImplSFMLWindow;
	
	SFMLWindow(const std::any& windowHandle,uint32_t width,uint32_t height);
	~SFMLWindow();
	SFMLWindow(const SFMLWindow&) = delete;
	SFMLWindow& operator=(const SFMLWindow&) = delete;
	SFMLWindow(SFMLWindow&& other) noexcept;
	SFMLWindow& operator=(SFMLWindow&& other) noexcept;

	SLD::InputParams::ReadOut ReadUserInputs();
	sf::RenderWindow& GetSFMLWindow();
	void Resize(uint32_t width,uint32_t height);
	void ClearColor(const float(&clearColor)[4]);
	void Present(bool shouldVSync);

private:

	OwnedPtr<ImplSFMLWindow> m_pMainWindow{};
};



#endif
