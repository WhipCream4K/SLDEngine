#pragma once

#include <SLDFramework.h>
#include "Level.h"

class HUD
{
public:

	enum class SpriteId
	{
		Player1Sprite,
		Player1Number,
		ChangeToText,
		ChangeToPlatform,
		LevelIndicator,
		ArrowsLeft,
		ArrowsRight,
		
		Count
	};

	enum class TextId
	{
		Score,
		Level,
		Count,
	};

	struct SpriteHandle
	{
		SharedPtr<sf::Sprite> sprite{};
		SharedPtr<SLD::Tracker> spirteHandle{};
		SharedPtr<SLD::GameObject> gameObject{};
	};

	struct TextHandle
	{
		SharedPtr<sf::Text> text{};
		SharedPtr<SLD::Tracker> textHandle{};
		SharedPtr<SLD::GameObject> gameObject{};
	};

public:

	HUD(SLDWorldEntity& worldEntity);
	void SetTexture(const sf::Texture& texture);
	void SetFont(const sf::Font& font);

	void ChangePlatFormIndicatorTextureRect(const sf::IntRect& textureRect);
	void OnLevelChange(Level::LevelState state);
	void ShouldAddScore(int score);
	void OnPlayerDied(int currentLives);
	void Update(float deltaTime);
	void PlayNaiveAnimation(float deltaTime);
	void OnLevelRestart(Level::LevelState state);

private:

	void SetUpSprite(SLDWorldEntity& worldEntity);
	void SetUpTexts(SLDWorldEntity& worldEntity);

	std::vector<SpriteHandle> m_HUDSprites;
	std::vector<TextHandle> m_HUDTexts;

	using PlayerLivesIndicator = std::pair<SharedPtr<SLD::GameObject>, SharedPtr<SLD::Tracker>>;

	SharedPtr<sf::Sprite> m_PlayerLivesIndicatorSprite;
	std::vector<PlayerLivesIndicator> m_PlayerLivesIndicatorObject;

	//SharedPtr<sf::RectangleShape>
	
	int m_CurrentScore{};
	float m_AnimTimeCount{};
	int m_AnimId{};
	float m_AnimInterval{ 0.1f };
	//SpriteHandle m_PlayerLivesIndicator;
	//SharedPtr<sf::Sprite> m_Player1Sprite;
	//SharedPtr<sf::Sprite> m_PlayerNumberSprite;
	//SharedPtr<sf::Sprite> m_ChangeToSprite;
	//SharedPtr<sf::Sprite> m_LevelIdSprite;
	//SharedPtr<sf::Sprite> m_ChangeToPlatformSprite;

	//SharedPtr<SLD::Tracker> m_Player1SpriteHandle;
	//SharedPtr<SLD::Tracker> m_PlayerNumberSpriteHandle;
	//SharedPtr<SLD::Tracker> m_ChangeToSpriteHandle;
	//SharedPtr<SLD::Tracker> m_LevelIdSpriteHandle;
	//SharedPtr<SLD::Tracker> m_ChangeToPlatformSpriteHandle;
};

