#pragma once

#include <SLDFramework.h>
#include "Core/DynamicMulticastDelegate.h"

namespace SLD
{
	class RenderComponent;
}

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
		RefPtr<SLD::Tracker> spriteHandle{};
		std::vector<Node> edges{};
		uint32_t stepCount{};
		int currentSpriteId{};
	};

	using HexGrid = std::map<uint32_t, std::vector<Platform>>;

	enum class LevelState
	{
		Level1,
		Level2,
		Level3,

		Count
	};


public:

	// Gonna do static const for now
	static const int MaxSpriteCnt;

	Level(SLDWorldEntity& world);

	void Update(float deltaTime);
	void SetTexture(const sf::Texture& texture);
	void SetSpriteOrigin(const sf::IntRect& textureRect);
	void SetSpriteTextureRectFromId(int id, const sf::IntRect& textureRect);
	void ChangeAllPlatformSprite(int id);
	void ChangeAllPlatformSprite(const RefPtr<sf::Sprite>& sprite);
	void ChangePlatformSprite(const RefPtr<sf::Sprite>& sprite, uint32_t row, uint32_t col);
	void ChangePlatformTextureRect(const sf::IntRect& textureRect, uint32_t row, uint32_t col);
	void ChangeAllPlatformTextureRect(const sf::IntRect& textureRect);
	const HexGrid& GetGrid() const noexcept;

	void OnPlayerFinishedJump(const Node& to);
	void OnPlayerDied(int currentLives);
	void OnGameWon();

	using FLevelChange = SLD::DynamicMulticastDelegate<void(LevelState)>;
	static FLevelChange OnLevelChange;
	void ChangeLevelSprite(LevelState state);

private:

	bool CheckWinCondition(int winSpriteId) const;
	void SetUpSprites(int amountToCreate);
	void SetupThreadWorker();
	void HandlePlatformSpriteSwitch(const Node& node, LevelState state);

	static constexpr uint8_t PlatformCnt{ 28 };
	static constexpr uint8_t PlatformDimension[2]{ 32,32 };
	static constexpr uint8_t PlatformMaxRow{ 7 };
	static constexpr uint8_t PlatformMaxCol{ 7 };

	void ConstructPlatform(SLDWorldEntity& worldEntt);
	void ResetAllPlatformToBase();

	//std::array<Platform, PlatformCnt> m_Platforms;
	// Row and Column
	HexGrid m_HexPlatform;
	std::vector<RefPtr<sf::Sprite>> m_SwapSprites;
	std::string m_MapLayOutFile;
	int m_WinSpriteId{};
	LevelState m_CurrentState;
	int m_SpriteFlashCount{};
	float m_SpriteFlashTimeCount{};
	float m_SpriteFlashInterval{ 0.25f };
	float m_FlashTimeCount{};
	float m_FlashTime{ 5.0f };
	bool m_WinFlag{};
	//SLD::PersistentThreadWorker m_CheckWinConditionThread;
	//std::atomic_bool m_WinFlag{};
};
