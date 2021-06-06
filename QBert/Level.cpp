#include "Level.h"

#include "QBertParams.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "JSON/nlohmann/json.hpp"
//#include "Sound/SoundManager.h"
#include <fstream>

Level::FLevelRestart Level::OnLevelRestart{};
const int Level::MaxSpriteCnt{ 3 };

Level::Level(SLDWorldEntity& world)
	: m_MapLayOutFile("./Resources/Map/LevelLayout.json")
	, m_CurrentState(LevelState::Level1)
{
	//InitializeGameObjects(world);
	ConstructPlatform(world);
	const int spriteCntForNow{ 3 };
	SetUpSprites(spriteCntForNow);
	//SetupThreadWorker();
}

void Level::Update(float deltaTime)
{
	if(m_WinFlag)
	{
		// Display Flashing Sprite
		m_FlashTimeCount += deltaTime;
		if(m_FlashTimeCount >= m_FlashTime)
		{
			// Stop and Broadcast that level should restart
			m_WinFlag = false;
			m_FlashTimeCount = 0;
			return;
		}

		if(m_FlashTimeCount >= m_SpriteFlashInterval)
		{
			m_SpriteFlashCount = (m_SpriteFlashCount + 1) % MaxSpriteCnt;
		}

		ChangeAllPlatformSprite(m_SpriteFlashCount);
		
	}
	deltaTime;
}

void Level::SetTexture(const sf::Texture& texture)
{
	for (auto& sprite : m_SwapSprites)
	{
		sprite->setTexture(texture);
	}
}

void Level::SetSpriteTextureRectFromId(int id, const sf::IntRect& textureRect)
{
	if (id >= 0 && id < int(m_SwapSprites.size()))
	{
		auto& ref{ m_SwapSprites[id] };
		ref->setTextureRect(textureRect);
		ref->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
	}
}

void Level::ChangeAllPlatformSprite(int id)
{
	const auto* target{ m_SwapSprites[id].get() };
	for (auto& row : m_HexPlatform)
	{
		for (auto& platform : row.second)
		{
			CopyTextureRegion(target, platform.spriteHandle);
		}
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
	const auto* target{ sprite.get() };
	auto handle{ m_HexPlatform.at(row)[col].spriteHandle };

	CopyTextureRegion(target, handle);
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

void Level::OnPlayerFinishedJump(const Node& to)
{
	HandlePlatformSpriteSwitch(to, m_CurrentState);
}

void Level::OnPlayerDied(int currentLives)
{
	if(currentLives < int(QBert::PlayerMaxLives))
	{
		// TODO: Restart Level
	}
}

bool Level::CheckWinCondition(int winSpriteId) const
{
	bool win{ true };
	for (const auto& row : m_HexPlatform)
	{
		for (const auto& platForm : row.second)
		{
			if (platForm.currentSpriteId != winSpriteId)
			{
				win = false;
				break;
			}
		}
	}

	return win;
}

void Level::SetUpSprites(int amountToCreate)
{
	for (int i = 0; i < amountToCreate; ++i)
	{
		auto ref{ m_SwapSprites.emplace_back(std::make_shared<sf::Sprite>()) };
	}
}

void Level::SetupThreadWorker()
{
	//m_CheckWinConditionThread.Start();
	//m_CheckWinConditionThread.AssignTask([this]()
	//	{
	//		if (!m_WinFlag)
	//		{
	//			m_WinFlag = CheckWinCondition(m_WinSpriteId);
	//			
	//		}
	//	});
}

void Level::HandlePlatformSpriteSwitch(const Node& node, LevelState state)
{
	auto& platform = m_HexPlatform.at(node.row)[node.col];

	// There are only 3 sprites in use
	// 0 is default
	// 1 is usually what changed to
	// 2 for now is to signal winning

	const int winSpriteId{ 1 };
	switch (state)
	{
	case LevelState::Level1:
	{
		// Step only once then lock the sprite changes
		platform.stepCount = 1;
			
		if (platform.currentSpriteId != winSpriteId)
		{
			ChangePlatformSprite(m_SwapSprites[winSpriteId], node.row, node.col);
			platform.currentSpriteId = winSpriteId;
		}
	}
	break;
	case LevelState::Level2:
		{
			
		}
		break;
	case LevelState::Level3: break;
	default:;
	}

	m_WinFlag = CheckWinCondition(winSpriteId);
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

	//const size_t renderSize{ sizeof(sf::Sprite) };
	//const uint32_t renderElemCnt{ 1 };

	uint32_t col{};
	for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
	{
		std::vector<Platform> platforms{};
		for (size_t i = 0; i < it.value().size(); ++i)
		{
			const auto gameObject{ world.CreateGameObject() };
			std::vector<Node> connectedGrid{};

			auto temp{ gameObject->GetTransform() };
			auto handle = world.GetRenderBuffer().AllocRenderElement<sf::Sprite>(temp, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

			auto& link = it.value()[i]["link"];
			for (auto& item : link.items())
			{
				const uint32_t cRow{ uint32_t(item.value()["row"]) };
				const uint32_t cCol{ uint32_t{item.value()["col"]} };
				Node node{ cRow,cCol };
				connectedGrid.push_back(node);
			}

			platforms.emplace_back(Platform{ gameObject,handle,std::move(connectedGrid),0,0 });

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
