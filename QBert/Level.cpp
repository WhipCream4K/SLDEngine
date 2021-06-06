#include "Level.h"

#include "QBertParams.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "JSON/nlohmann/json.hpp"
//#include "Sound/SoundManager.h"
#include <fstream>

Level::Level(SLDWorldEntity& world)
	: m_MapLayOutFile("./Resources/Map/LevelLayout.json")
{
	//InitializeGameObjects(world);
	ConstructPlatform(world);
}

void Level::SetTexture(sf::Texture& texture)
{
	texture;
	for (auto& platform : m_HexPlatform)
	{
		for (auto& obj : platform.second)
		{
			obj;
			//obj.sprite->GetPtr()->setTexture(texture);
		//platform.sprite->GetPtr()->setTexture(texture);
		}
		//(*platform.sprite)->setTexture(texture);
	}
}

void Level::ChangeAllPlatformSprite(const RefPtr<sf::Sprite>& sprite)
{
	sprite;
	const auto* target{ sprite.get() };
	for (auto& row : m_HexPlatform)
	{
		for (auto& platform : row.second)
		{
			CopyTextureRegion(target, platform.spriteHandle);
		}
	}
}

void Level::ChangePlatformSprite(const RefPtr<sf::Sprite>& sprite, uint32_t row, uint32_t col)
{
	sprite;
	row;
	col;
	//m_HexPlatform.at(row)[col].showSprite
}

void Level::ChangePlatformTextureRect(const sf::IntRect& textureRect, uint32_t row, uint32_t col)
{
	textureRect;
	row;
	col;
	//m_HexPlatform.at(row)[col].sprite->GetPtr()->setTextureRect(textureRect);
}

//void Level::ChangePlatformTextureRect(const sf::IntRect& textureRect, uint8_t id)
//{
//	m_HexPlatform[id].sprite->GetPtr()->setTextureRect(textureRect);
//	//(*m_Platforms[id].sprite)->setTextureRect(textureRect);
//}

void Level::ChangeAllPlatformTextureRect(const sf::IntRect& textureRect)
{
	textureRect;
	for (auto& platform : m_HexPlatform)
	{
		for (auto& obj : platform.second)
		{
			obj;
			//auto sfSprite{ obj.sprite->GetPtr() };
			//sfSprite->setTextureRect(textureRect);
			//sfSprite->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
		}
		//platfornm.sprite->GetPtr()->setTextureRect(textureRect);
		//platfornm.sprite->GetPtr()->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
		//(*platfornm.sprite)->setTextureRect(textureRect);
		//(*platfornm.sprite)->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
	}
}

void Level::SetStepNeeded(uint32_t count)
{
	m_StepNeeded = count;
}

void Level::OnPlayerJump(const Node& to)
{
	to;
	std::cout << "yay" << std::endl;
}

const Level::HexGrid& Level::GetGrid() const noexcept
{
	return m_HexPlatform;
}

void Level::ConstructPlatform(SLDWorldEntity& world)
{
	using namespace nlohmann;


	const float scaleX{ QBert::GlobalScaleX };
	const float scaleY{ QBert::GlobalScaleY };

	//	//							0
	//	//						1		2
	//	//					3		4		5
	//	//				6		7		8		9
	//	//			10		11		12		13		14
	//	//		15		16		17		18		19		20
	//	//	21		22		23		24		25		26		27

	std::ifstream mapLayOutFile{ m_MapLayOutFile };
	json jsonObj{};
	mapLayOutFile >> jsonObj;

	const size_t renderSize{ sizeof(sf::Sprite) };
	const uint32_t renderElemCnt{ 1 };

	uint32_t col{};
	for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
	{
		std::vector<Platform> platforms{};
		for (size_t i = 0; i < it.value().size(); ++i)
		{			
			const auto gameObject{ world.CreateGameObject() };
			//const auto renderComponent{ gameObject->CreateRenderingComponent(renderSize,renderElemCnt) };
			//world.GetRenderBuffer().PushRenderElement()
			//const auto sfSprite{ renderComponent->GetPtr()->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite)) };
			std::vector<Node> connectedGrid{};

			auto temp{ gameObject->GetTransform() };
			auto handle = world.GetRenderBuffer().AllocRenderElement<sf::Sprite>(temp, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));
			
			auto& link = it.value()[i]["link"];
			for (auto& item : link.items())
			{
				const uint32_t cRow{ uint32_t(item.value()["row"])};
				const uint32_t cCol{ uint32_t{item.value()["col"]} };
				Node node{ cRow,cCol };
				connectedGrid.push_back(node);
			}

			platforms.emplace_back(Platform{ gameObject,handle,std::move(connectedGrid),0 });

			float posX{ (it.value()[i])["x"] };
			float posY{ (it.value()[i])["y"] };

			auto transform{ platforms[i].gameObject->GetTransform()->GetPtr() };
			transform->Translate(posX * scaleX, posY * scaleY, float(QBert::Layer::Map));
			transform->SetScale(scaleX, scaleY, 1.0f);
		}



		m_HexPlatform.try_emplace(col, platforms);
		col++;
	}

#pragma region OLD
	//const float platformWidth{ float(PlatformDimension[0]) * scaleX };
//const float platformHeight{ float(PlatformDimension[1]) * scaleY };
//const float platformOffSet{ 1.5f * platformHeight * 0.5f };

//float startPlatformHeight{ (platformHeight * 3.0f) - platformHeight };

//float debugY{float(PlatformDimension[1])};
//float debugPlatformOffset{ 1.5f * debugY * 0.5f };
//float debugX{ float(PlatformDimension[0]) };
//debugPlatformOffset;
//debugX;
//startPlatformHeight;



//uint8_t num{ 0 };
//for (uint8_t i = 0; i < PlatformMaxRow; ++i)
//{
//	const int colCnt{ int(i) };
//	float offset{ -(0.5f) * float(colCnt) };
//	for (int j = 0; j <= colCnt; ++j)
//	{
//		platformTransform = m_Platforms[num++].gameObject->GetTransform(); // NOTE: DANGER EXCEPTION
//		platformTransform->GetPtr()->Translate(platformWidth * offset, startPlatformHeight, float(QBert::Layer::Map));
//		platformTransform->GetPtr()->SetScale(scaleX, scaleY, 1.0f);
//		offset += 1.0f;
//	}

//	startPlatformHeight -= platformOffSet;
//}
#pragma endregion 
}
