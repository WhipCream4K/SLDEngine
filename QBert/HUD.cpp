#include "HUD.h"
#include "QBertParams.h"
#include "Components/TransformComponent.h"

HUD::HUD(SLDWorldEntity& worldEntity)
	: m_HUDSprites()
	, m_HUDTexts()
	, m_PlayerLivesIndicatorSprite()
	, m_PlayerLivesIndicatorObject()
	, m_CurrentScore()
	, m_AnimTimeCount()
	, m_AnimId()
{
	SetUpSprite(worldEntity);
	m_HUDTexts.reserve(3);
	SetUpTexts(worldEntity);
}

void HUD::SetTexture(const sf::Texture& texture)
{
	for (auto& sprite : m_HUDSprites)
	{
		sprite.sprite->setTexture(texture);
		CopyTextureRegion(sprite.sprite.get(), sprite.spirteHandle);
	}

	m_PlayerLivesIndicatorSprite->setTexture(texture);

	for (auto& obj : m_PlayerLivesIndicatorObject)
	{
		CopyTextureRegion(m_PlayerLivesIndicatorSprite.get(), obj.second);
	}
}

void HUD::SetFont(const sf::Font& font)
{
	for (auto& text : m_HUDTexts)
	{
		text.text->setFont(font);
		const auto& bounds = text.text->getGlobalBounds();
		text.text->setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
	}
}

void HUD::ChangePlatFormIndicatorTextureRect(const sf::IntRect& textureRect)
{
	auto& element{ m_HUDSprites[int(SpriteId::ChangeToPlatform)] };
	element.sprite->setTextureRect(textureRect);
	CopyTextureRegion(element.sprite.get(), element.spirteHandle);
}

void HUD::OnLevelChange(Level::LevelState state)
{
	sf::IntRect targetRect{};
	int levelNum{};
	switch (state)
	{
	case Level::LevelState::Level1:	targetRect = QBert::Level1::PlatfromChangeTo;
		levelNum = 1;
		break;
	case Level::LevelState::Level2: targetRect = QBert::Level2::PlatfromChangeTo;
		levelNum = 2;
		break;
	case Level::LevelState::Level3:	targetRect = QBert::Level3::PlatfromChangeTo;
		levelNum = 3;
		break;
	case Level::LevelState::Count: break;
	default: break;
	}

	m_HUDTexts[int(TextId::Level)].text->setString(std::to_string(levelNum));
	ChangePlatFormIndicatorTextureRect(targetRect);
}

void HUD::ShouldAddScore(int score)
{
	m_CurrentScore += score;
	m_HUDTexts[int(TextId::Score)].text->setString(std::to_string(m_CurrentScore));
}

void HUD::Update(float deltaTime)
{
	deltaTime;
	PlayNaiveAnimation(deltaTime);
}

void HUD::PlayNaiveAnimation(float deltaTime)
{
	m_AnimTimeCount += deltaTime;
	if (m_AnimTimeCount >= m_AnimInterval)
	{
		m_AnimTimeCount = 0;
		
		const int totalSpriteInAnim{ 6 };
		m_AnimId = (m_AnimId + 1) % totalSpriteInAnim;
		auto& element{ m_HUDSprites[int(SpriteId::Player1Sprite)] };
		auto textureRect = element.sprite->getTextureRect();
		textureRect.top = 112 + int(m_AnimId * textureRect.height);
		element.sprite->setTextureRect(textureRect);
		CopyTextureRegion(element.sprite.get(), element.spirteHandle);

		auto& arrowLeftElement{ m_HUDSprites[int(SpriteId::ArrowsLeft)] };
		textureRect = arrowLeftElement.sprite->getTextureRect();
		const int fadeAmount{1};
		textureRect.left = 264 - (int(m_AnimId) * fadeAmount);
		arrowLeftElement.sprite->setTextureRect(textureRect);

		CopyTextureRegion(arrowLeftElement.sprite.get(), arrowLeftElement.spirteHandle);

		auto& arrowRightElement{ m_HUDSprites[int(SpriteId::ArrowsRight)] };
		textureRect = arrowRightElement.sprite->getTextureRect();
		textureRect.left = 272 + (int(m_AnimId) * fadeAmount);
		arrowRightElement.sprite->setTextureRect(textureRect);

		CopyTextureRegion(arrowRightElement.sprite.get(), arrowRightElement.spirteHandle);

	}
}

void HUD::SetUpSprite(SLDWorldEntity& worldEntity)
{
	for (int i = 0; i < int(SpriteId::Count); ++i)
	{
		SpriteHandle handle{};
		handle.gameObject = worldEntity.CreateGameObject();
		handle.sprite = std::make_shared<sf::Sprite>();
		auto& transform{ handle.gameObject->GetTransform() };

		transform->GetPtr()->SetScale(QBert::GlobalScaleX, QBert::GlobalScaleY, 1.0f);

		sf::IntRect textureRect{};

		rtm::float3f objPos{};
		objPos.z = float(QBert::Layer::HUD);

		switch (SpriteId(i))
		{
		case SpriteId::Player1Sprite:

			// Purple P
			textureRect = sf::IntRect{ 184,112,56,8 };
			objPos.x -= 300.0f;
			objPos.y += 240.0f;

			break;
		case SpriteId::Player1Number:

			textureRect = sf::IntRect{ 240,112,8,16 };
			// Same as Player1Sprite
			objPos.x -= 220.0f;
			objPos.y += 240.0f + 8.0f;

			break;
		case SpriteId::ChangeToText:

			textureRect = sf::IntRect{ 240,144,56,8 };
			objPos.x -= 300.0f;
			objPos.y += 180.0f;

			break;
		case SpriteId::ChangeToPlatform:

			textureRect = QBert::Level1::PlatfromChangeTo;
			objPos.x -= 300.0f;
			objPos.y += 150.0f;

			break;
		case SpriteId::LevelIndicator:

			textureRect = sf::IntRect{ 240,128,42,8 };
			objPos.x += 300.0f;
			objPos.y += 180.0f;

			break;
		case SpriteId::ArrowsLeft: 

			textureRect = sf::IntRect{264,118,8,8};
			objPos.x -= 330.0f;
			objPos.y += 140.0f;

			break;
		case SpriteId::ArrowsRight:

			textureRect = sf::IntRect{ 272,118,8,8 };
			
			objPos.x -= 270.0f;
			objPos.y += 140.0f;
			
			break;
		case SpriteId::Count: break;
		default: break;
		}

		transform->GetPtr()->Translate(objPos);
		handle.sprite->setTextureRect(textureRect);
		handle.sprite->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);

		handle.spirteHandle = worldEntity.GetRenderBuffer().PushRenderElement(
			transform,
			SLD::RenderIdentifier(SFMLRenderElement::RenderSprite),
			handle.sprite.get());

		m_HUDSprites.push_back(handle);
	}

	// Player Lives Indicator
	m_PlayerLivesIndicatorSprite = std::make_shared<sf::Sprite>();

	const sf::IntRect staticPlayerSprite{ 128,0,16,16 };
	m_PlayerLivesIndicatorSprite->setTextureRect(staticPlayerSprite);
	m_PlayerLivesIndicatorSprite->setOrigin(float(staticPlayerSprite.width) * 0.5f, float(staticPlayerSprite.height) * 0.5f);

	for (int i = 0; i < QBert::PlayerMaxLives; ++i)
	{
		auto gameObject = worldEntity.CreateGameObject();
		auto transform{ gameObject->GetTransform() };

		auto handle{ worldEntity.GetRenderBuffer().PushRenderElement(transform,
			SLD::RenderIdentifier(SFMLRenderElement::RenderSprite),
			m_PlayerLivesIndicatorSprite.get()) };

		m_PlayerLivesIndicatorObject.emplace_back(gameObject, handle);
	}
}

void HUD::SetUpTexts(SLDWorldEntity& worldEntity)
{
	for (int i = 0; i < int(TextId::Count); ++i)
	{
		TextHandle handle{};
		
		handle.gameObject = worldEntity.CreateGameObject();
		auto& transform{ handle.gameObject->GetTransform() };
		
		handle.text = std::make_shared<sf::Text>();
		handle.text->setString(std::to_string(m_CurrentScore));
		handle.text->setFillColor(sf::Color{255,127,80,255});
		
		rtm::float3f objPos{};
		objPos.z = float(QBert::Layer::HUD);
		
		switch (TextId(i))
		{
		case TextId::Score: 

			objPos.x -= 300.f;
			objPos.y += 220.0f;

			break;
		case TextId::Level: 

			objPos.x += 330.0f;
			objPos.y += 183.0f;
			handle.text->setString("1");
			handle.text->setCharacterSize(20);

			break;
		case TextId::Count: break;
		default: break;
		}

		transform->GetPtr()->Translate(objPos);

		//handle.textHandle = worldEntity.GetRenderBuffer().AllocRenderElement<sf::Text>(transform, SLD::RenderIdentifier(SFMLRenderElement::RenderText
		
		//handle.textHandle = worldEntity.GetRenderBuffer().PushRenderElement(
		//	transform,
		//	SLD::RenderIdentifier(SFMLRenderElement::RenderText),
		//	&handle.text.get()
		//);

		m_HUDTexts.push_back(handle);

		auto ptrAddress{ m_HUDTexts[i].text.get() };

		//TODO: Damn never use SFML Text again
		m_HUDTexts[i].textHandle = worldEntity.GetRenderBuffer().PushRenderElement(
			transform,
			SLD::RenderIdentifier(SFMLRenderElement::RenderText),
			&ptrAddress
		);
	}
}
