#pragma once

#include <SLDFramework.h>

class Level
{
public:

	struct Node
	{
		uint32_t row{};
		uint32_t col{};
	};
	
	struct Platform
	{
		RefPtr<SLD::GameObject> gameObject{};
		RefPtr<SLD::ObservePtr<sf::Sprite>> sprite{};
		std::vector<Node> edges{};
		uint32_t stepCount{};
		//RefPtr<sf::Sprite*> sprite{};
	};

	using HexGrid = std::map<uint32_t, std::vector<Platform>>;

public:

	Level(SLDWorldEntity& world);

	void SetTexture(sf::Texture& texture);
	void ChangePlatformTextureRect(const sf::IntRect& textureRect,uint32_t row,uint32_t col);
	void ChangeAllPlatformTextureRect(const sf::IntRect& textureRect);
	void SetStepNeeded(uint32_t count);
	const HexGrid& GetGrid() const noexcept;

private:

	static constexpr uint8_t PlatformCnt{ 28 };
	static constexpr uint8_t PlatformDimension[2]{ 32,32 };
	static constexpr uint8_t PlatformMaxRow{ 7 };
	static constexpr uint8_t PlatformMaxCol{ 7 };

	void ConstructPlatform(SLDWorldEntity& worldEntt);

	//std::array<Platform, PlatformCnt> m_Platforms;
	// Row and Column
	HexGrid m_HexPlatform;
	std::string m_MapLayOutFile;
	uint32_t m_StepNeeded;
};
