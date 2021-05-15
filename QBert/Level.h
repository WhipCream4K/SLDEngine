#pragma once

#include <SLDFramework.h>

class Level
{
public:

	Level(SLDWorldEntity& world);

	void SetTexture(sf::Texture& texture);
	void ChangePlatformTextureRect(const sf::IntRect& textureRect,uint8_t id);
	void ChangeAllPlatformTextureRect(const sf::IntRect& textureRect);

private:

	static constexpr uint8_t PlatformCnt{ 28 };
	static constexpr uint8_t PlatformDimension[2]{ 32,32 };
	static constexpr uint8_t PlatformMaxRow{ 7 };
	static constexpr uint8_t PlatformMaxCol{ 7 };

	void ConstructPlatform();
	void InitializeGameObjects(SLDWorldEntity& worldEntt);

	struct Platform
	{
		RefPtr<SLD::GameObject> gameObject{};
		RefPtr<sf::Sprite> sprite{};
	};
	
	std::array<Platform, PlatformCnt> m_Platforms;
};

