#include "Level.h"

#include "QBertParams.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"

Level::Level(SLDWorldEntity& world)
{
	InitializeGameObjects(world);
	ConstructPlatform();
}

void Level::SetTexture(sf::Texture& texture)
{
	for (auto& platform : m_Platforms)
	{
		platform.sprite->GetPtr()->setTexture(texture);
		//(*platform.sprite)->setTexture(texture);
	}
}

void Level::ChangePlatformTextureRect(const sf::IntRect& textureRect, uint8_t id)
{
	m_Platforms[id].sprite->GetPtr()->setTextureRect(textureRect);
	//(*m_Platforms[id].sprite)->setTextureRect(textureRect);
}

void Level::ChangeAllPlatformTextureRect(const sf::IntRect& textureRect)
{
	for (auto& platfornm : m_Platforms)
	{
		platfornm.sprite->GetPtr()->setTextureRect(textureRect);
		platfornm.sprite->GetPtr()->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
		//(*platfornm.sprite)->setTextureRect(textureRect);
		//(*platfornm.sprite)->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
	}
}

void Level::ConstructPlatform()
{
	RefPtr<SLD::ObservePtr<SLD::TransformComponent>> platformTransform{};

	//uint8_t colCnt{1};
	const float scaleX{ QBert::GlobalScaleX };
	const float scaleY{ QBert::GlobalScaleY };
	const float platformWidth{ float(PlatformDimension[0]) * scaleX };
	const float platformHeight{ float(PlatformDimension[1]) * scaleY };
	const float platformOffSet{ 1.5f * platformHeight * 0.5f };

	float startPlatformHeight{ (platformHeight * 3.0f) - platformHeight };

	//float debugY{float(PlatformDimension[1])};
	//float debugPlatformOffset{ 1.5f * debugY * 0.5f };
	//float debugX{ float(PlatformDimension[0]) };
	//debugPlatformOffset;
	//debugX;
	//startPlatformHeight;

	//	//							0
	//	//						1		2
	//	//					3		4		5
	//	//				6		7		8		9
	//	//			10		11		12		13		14
	//	//		15		16		17		18		19		20
	//	//	21		22		23		24		25		26		27
	
	uint8_t num{ 0 };
	for (uint8_t i = 0; i < PlatformMaxRow; ++i)
	{
		const int colCnt{ int(i) };
		float offset{ -(0.5f) * float(colCnt) };
		for (int j = 0; j <= colCnt; ++j)
		{
			platformTransform = m_Platforms[num++].gameObject->GetTransform(); // NOTE: DANGER EXCEPTION
			platformTransform->GetPtr()->Translate(platformWidth * offset, startPlatformHeight, float(QBert::Layer::Map));
			platformTransform->GetPtr()->SetScale(scaleX, scaleY, 1.0f);
			offset += 1.0f;
		}

		startPlatformHeight -= platformOffSet;
	}

}

void Level::InitializeGameObjects(SLDWorldEntity& worldEntt)
{
	const size_t renderSize{sizeof(sf::Sprite)};
	const uint32_t renderElemCnt{ 1 };

	for (auto& item : m_Platforms)
	{
		item.gameObject = worldEntt.CreateGameObject();
		auto renderComponent{ item.gameObject->CreateRenderingComponent(renderSize,renderElemCnt) };
		//item.sprite = render->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));
		item.sprite = renderComponent->GetPtr()->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));
	}
}
