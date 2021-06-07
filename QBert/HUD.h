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
		RefPtr<sf::Sprite> sprite{};
		RefPtr<SLD::Tracker> spirteHandle{};
		RefPtr<SLD::GameObject> gameObject{};
	};

	struct TextHandle
	{
		RefPtr<sf::Text> text{};
		RefPtr<SLD::Tracker> textHandle{};
		RefPtr<SLD::GameObject> gameObject{};
	};

public:

	HUD(SLDWorldEntity& worldEntity);
	void SetTexture(const sf::Texture& texture);
	void SetFont(const sf::Font& font);

	void ChangePlatFormIndicatorTextureRect(const sf::IntRect& textureRect);
	void OnLevelChange(Level::LevelState state);
	void ShouldAddScore(int score);
	void Update(float deltaTime);
	void PlayNaiveAnimation(float deltaTime);

private:

	void SetUpSprite(SLDWorldEntity& worldEntity);
	void SetUpTexts(SLDWorldEntity& worldEntity);

	std::vector<SpriteHandle> m_HUDSprites;
	std::vector<TextHandle> m_HUDTexts;

	using PlayerLivesIndicator = std::pair<RefPtr<SLD::GameObject>, RefPtr<SLD::Tracker>>;

	RefPtr<sf::Sprite> m_PlayerLivesIndicatorSprite;
	std::vector<PlayerLivesIndicator> m_PlayerLivesIndicatorObject;

	//RefPtr<sf::RectangleShape>
	
	int m_CurrentScore{};
	float m_AnimTimeCount{};
	int m_AnimId{};
	float m_AnimInterval{ 0.1f };
	//SpriteHandle m_PlayerLivesIndicator;
	//RefPtr<sf::Sprite> m_Player1Sprite;
	//RefPtr<sf::Sprite> m_PlayerNumberSprite;
	//RefPtr<sf::Sprite> m_ChangeToSprite;
	//RefPtr<sf::Sprite> m_LevelIdSprite;
	//RefPtr<sf::Sprite> m_ChangeToPlatformSprite;

	//RefPtr<SLD::Tracker> m_Player1SpriteHandle;
	//RefPtr<SLD::Tracker> m_PlayerNumberSpriteHandle;
	//RefPtr<SLD::Tracker> m_ChangeToSpriteHandle;
	//RefPtr<SLD::Tracker> m_LevelIdSpriteHandle;
	//RefPtr<SLD::Tracker> m_ChangeToPlatformSpriteHandle;
};

