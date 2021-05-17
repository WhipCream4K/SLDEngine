#include "Level.h"

#include "QBertParams.h"
#include "GameObject/GameObject.h"

Level::Level(SLDWorldEntity& world)
{
	InitializeGameObjects(world);
	//ConstructPlatform();
}

void Level::SetTexture(sf::Texture& texture)
{
	for (auto& platform : m_Platforms)
	{
		platform.sprite->setTexture(texture);
	}
}

void Level::ChangePlatformTextureRect(const sf::IntRect& textureRect, uint8_t id)
{
	m_Platforms[id].sprite->setTextureRect(textureRect);
}

void Level::ChangeAllPlatformTextureRect(const sf::IntRect& textureRect)
{
	for (auto& platfornm : m_Platforms)
	{
		platfornm.sprite->setTextureRect(textureRect);
		platfornm.sprite->setOrigin(float(textureRect.width), float(textureRect.height));
	}
}

void Level::ConstructPlatform()
{
	RefPtr<SLD::ObservePtr<SLD::TransformComponent>> platformTransform{};

	//uint8_t colCnt{1};
	const float platformWidth{ float(PlatformDimension[0]) };
	const float platformHeight{ float(PlatformDimension[1])};

	float startPlatformHeight{ platformHeight * 3.0f };
	
	uint8_t num{0};
	for (uint8_t i = 0; i < PlatformMaxRow; ++i)
	{
		const int colCnt{ int(i) };
		float offset{ -(0.5f) * float(colCnt) };
		for (int j = 0; j <= colCnt; ++j)
		{
			platformTransform = m_Platforms[num++].gameObject->GetTransform();
			platformTransform->GetPtr()->Translate(platformWidth * offset, startPlatformHeight, 1.0f);
			platformTransform->GetPtr()->SetScale(QBert::GlobalScaleX, QBert::GlobalScaleY, 1.0f);
			offset += 1.0f;
		}

		startPlatformHeight -= platformHeight;
	}

	//	//TODO: Set up platform position

	//	//							0
	//	//						1		2
	//	//					3		4		5
	//	//				6		7		8		9
	//	//			10		11		12		13		14
	//	//		15		16		17		18		19		20
	//	//	21		22		23		24		25		26		27


	//	
	//}
}

void Level::InitializeGameObjects(SLDWorldEntity& worldEntt)
{
	const size_t renderSize{
		sizeof(void*) + sizeof(sf::Sprite)
	};
	const size_t renderElemCnt{ 2 };
	
	for (auto& item : m_Platforms)
	{
		item.gameObject = worldEntt.CreateGameObject();
		RefPtr<SLD::RenderingComponent> render{ item.gameObject->CreateRenderingComponent(renderSize,renderElemCnt) };
		render->PushElement(SLD::RenderIdentifier(SFMLRenderElement::WorldMatrix), item.gameObject->GetTransform().get());
		item.sprite = render->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));
	}
}
