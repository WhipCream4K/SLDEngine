#include "Level.h"

#include "QBertParams.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "JSON/nlohmann/json.hpp"
#include "Sound/SoundManager.h"
#include "QBertGame.h"
#include "FlyingDisc.h"
#include <fstream>

#include "Player.h"

Level::FLevelChange Level::OnLevelChange{};
Level::FLevelRestart Level::OnLevelRestart{};
const int Level::MaxSpriteCnt{ 3 };

Level::Level(SLDWorldEntity& world)
	: m_MapLayOutFile("./Resources/Map/LevelLayout.json")
	, m_CurrentState(LevelState::Level1)
{
	ConstructPlatform(world);
	const int spriteCntForNow{ 3 };
	SetUpSprites(spriteCntForNow);
	const int reserveFlyingDisc{ 4 };
	PreInitializeFlyingDisc(world, reserveFlyingDisc);
}

void Level::Update(float deltaTime)
{
	if (m_WinFlag)
	{
		// Display Flashing Sprite
		m_FlashTimeCount += deltaTime;
		m_SpriteFlashTimeCount += deltaTime;

		if (m_FlashTimeCount >= m_FlashTime)
		{
			// Stop and Broadcast that level should change
			m_WinFlag = false;
			m_FlashTimeCount = 0;

			// NOTE: Look at this level change logic
			m_CurrentState = LevelState((uint32_t(m_CurrentState) + 1) % uint32_t(LevelState::Count));

			// Change Level Sprite
			QBertGame::OnAddScore.BroadCast(QBert::ScoreType::DiscBonus * m_AmountOfFlyingDisc);
			ChangeLevelSprite(m_CurrentState);
			OnLevelChange.BroadCast(m_CurrentState);
			return;
		}

		if (m_SpriteFlashTimeCount >= m_SpriteFlashInterval)
		{
			m_SpriteFlashTimeCount -= m_SpriteFlashInterval;
			m_SpriteFlashCount = (m_SpriteFlashCount + 1) % MaxSpriteCnt;
			ChangeAllPlatformSprite(m_SpriteFlashCount);
		}
	}

	for (auto& disc : m_FlyingDiscs)
	{
		disc->Update(deltaTime);
	}
}

void Level::SetTexture(const sf::Texture& texture)
{
	for (auto& sprite : m_SwapSprites)
	{
		sprite->setTexture(texture);
	}

	for (auto& disc : m_FlyingDiscs)
	{
		disc->SetTexture(texture);
	}
}

void Level::SetSpriteOrigin(const sf::IntRect& textureRect)
{
	for (auto& sprite : m_SwapSprites)
	{
		sprite->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
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

void Level::ChangeAllPlatformSprite(const SharedPtr<sf::Sprite>& sprite)
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

void Level::ChangePlatformSprite(const SharedPtr<sf::Sprite>& sprite, uint32_t row, uint32_t col)
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
}

void Level::ChangeAllPlatformTextureRect(const sf::IntRect& textureRect)
{
	textureRect;
	for (auto& platform : m_HexPlatform)
	{
		for (auto& obj : platform.second)
		{
			obj;
		}
	}
}

rtm::float3f Level::GetHexGridPos(int row, int col)
{
	rtm::float3f targetPos{};
	if (row >= 0 && row < int(m_HexPlatform.size()))
	{
		if (col >= 0 && col < int(m_HexPlatform.at(row).size()))
		{
			targetPos = m_HexPlatform.at(row)[col].gameObject->GetTransform()->GetPtr()->GetWorldPos();
		}
	}

	return targetPos;
}

void Level::OnPlayerFinishedJump(const Node& to)
{
	if (to.row >= 0 && to.row < int(m_HexPlatform.size()))
	{
		if (to.col >= 0 && to.col < int(m_HexPlatform.at(to.row).size()))
			HandlePlatformSpriteSwitch(to, m_CurrentState);
	}

}

void Level::OnPlayerDied(int currentLives)
{
	if (currentLives < 0)
	{
		// TODO: Restart Level
		m_WinFlag = true;
		m_CurrentState = LevelState(int(m_CurrentState) - 1);
		//const int baseSpriteId{ 0 };
		//for (auto& row : m_HexPlatform)
		//{
		//	for (size_t i = 0; i < row.second.size(); ++i)
		//	{
		//		if (row.second[i].currentSpriteId != baseSpriteId)
		//		{
		//			row.second[i].currentSpriteId = baseSpriteId;
		//			ChangePlatformSprite(m_SwapSprites[baseSpriteId], row.first, uint32_t(i));
		//		}
		//	}
		//}
	}
}

void Level::OnGameWon()
{
	m_WinFlag = true;
	SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Win, 0.5f);
}

void Level::ChangeLevelSprite(LevelState state)
{
	const int startSpriteId{ 0 };
	const int spareSpriteId{ 1 };
	const int endSpriteId{ 2 };

	const float levelStepX{ QBert::LevelPixelX * QBert::GlobalScaleX };
	const float levelStepY{ QBert::LevelPixelY * QBert::GlobalScaleY };
	switch (state)
	{
	case LevelState::Level1:
	{
		m_AmountOfFlyingDisc = 2;
		m_SwapSprites[startSpriteId]->setTextureRect(QBert::Level1::SpriteStart);
		m_SwapSprites[endSpriteId]->setTextureRect(QBert::Level1::SpriteEnd);
		m_SwapSprites[spareSpriteId]->setTextureRect(QBert::Level1::SpriteSpare);

		auto pos{ GetHexGridPos(3,3) };
		pos.x += levelStepX;
		m_FlyingDiscs[0]->SetPos(pos, 3, 4);

		pos = GetHexGridPos(3, 0);
		pos.x -= levelStepX;
		m_FlyingDiscs[1]->SetPos(pos, 3, -1);

	}
	break;
	case LevelState::Level2:
	{
		m_AmountOfFlyingDisc = 3;

		m_SwapSprites[startSpriteId]->setTextureRect(QBert::Level2::SpriteStart);
		m_SwapSprites[endSpriteId]->setTextureRect(QBert::Level2::SpriteEnd);
		m_SwapSprites[spareSpriteId]->setTextureRect(QBert::Level2::SpriteSpare);

		//NOTE: Reference from youtube
		auto pos{ GetHexGridPos(0,0) };
		pos.x += levelStepX;
		pos.y += levelStepY / 2.0f;
		m_FlyingDiscs[0]->SetPos(pos, -1, -1);

		pos = GetHexGridPos(2, 0);
		pos.x -= levelStepX;

		m_FlyingDiscs[1]->SetPos(pos, 2, -1);

		pos = GetHexGridPos(3, 3);
		pos.x += levelStepX;

		m_FlyingDiscs[2]->SetPos(pos, 3, 4);
	}
	break;
	case LevelState::Level3:
	{
		m_AmountOfFlyingDisc = 4;

		m_SwapSprites[startSpriteId]->setTextureRect(QBert::Level3::SpriteStart);
		m_SwapSprites[endSpriteId]->setTextureRect(QBert::Level3::SpriteEnd);
		m_SwapSprites[spareSpriteId]->setTextureRect(QBert::Level3::SpriteSpare);

		//NOTE: Reference from youtube
		auto pos{ GetHexGridPos(1,0) };
		pos.x -= levelStepX;
		m_FlyingDiscs[0]->SetPos(pos, 1, 2);

		pos = GetHexGridPos(1, 1);
		pos.x += levelStepX;

		m_FlyingDiscs[1]->SetPos(pos, 1, -1);

		pos = GetHexGridPos(2, 2);
		pos.x += levelStepX;

		m_FlyingDiscs[2]->SetPos(pos, 2, 3);

		pos = GetHexGridPos(3, 0);
		pos.x -= levelStepX;

		m_FlyingDiscs[3]->SetPos(pos, 3, -1);

	}
	break;
	default: break;
	}

	ResetAllPlatformToBase();

}

bool Level::IsFlyingDiscExist(const Node& check)
{
	bool isExist{};
	for (auto& disc : m_FlyingDiscs)
	{
		const auto discNode{ disc->GetNode() };
		if (check.row == discNode.row &&
			check.col == discNode.col)
		{
			isExist = true;
			return isExist;
		}
	}

	return isExist;
}

void Level::UseFlyingDisc(const SharedPtr<Player>& player, int row, int col)
{
	if (player)
	{
		for (auto& disc : m_FlyingDiscs)
		{
			auto node{ disc->GetNode() };
			if (node.row == row && node.col == col)
			{
				disc->AttachToPlayer(player);
			}
		}

		m_AmountOfFlyingDisc--;
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
		m_SwapSprites.emplace_back(std::make_shared<sf::Sprite>());
	}
}

void Level::HandlePlatformSpriteSwitch(const Node& node, LevelState state)
{
	auto& platform = m_HexPlatform.at(node.row)[node.col];

	// There are only 3 sprites in use
	// 0 is default
	// 1 is for level 2 and above
	// 2 is what changed to

	const int winSpriteId{ 2 };
	switch (state)
	{
	case LevelState::Level1:
	{
		// Step only once then lock the sprite changes
		platform.stepCount = 1;

		if (platform.currentSpriteId != winSpriteId)
		{
			QBertGame::OnAddScore.BroadCast(QBert::ScoreType::ChangeColor);
			ChangePlatformSprite(m_SwapSprites[winSpriteId], node.row, node.col);
			platform.currentSpriteId = winSpriteId;
		}
	}
	break;
	case LevelState::Level2:
	{
		// Step two times then lock
		if (platform.stepCount != 2)
		{
			QBertGame::OnAddScore.BroadCast(QBert::ScoreType::ChangeColor);
			platform.stepCount++;
			platform.currentSpriteId++;
			ChangePlatformSprite(m_SwapSprites[platform.currentSpriteId], node.row, node.col);
		}
	}
	break;
	case LevelState::Level3:
	{
		QBertGame::OnAddScore.BroadCast(QBert::ScoreType::ChangeColor);
		const uint32_t maxStep{ 2 };
		platform.stepCount = (platform.stepCount + 1) % maxStep;
		platform.currentSpriteId = (platform.stepCount == 0) ? 0 : 2;
		ChangePlatformSprite(m_SwapSprites[platform.currentSpriteId], node.row, node.col);
	}
	break;
	default: break;
	}

	if (const bool isLevelClear{ CheckWinCondition(winSpriteId) }; isLevelClear)
	{
		QBertGame::OnGameWon.BroadCast();
	}
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
				const int cRow{ item.value()["row"] };
				const int cCol{ item.value()["col"] };
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

void Level::ResetAllPlatformToBase()
{
	for (auto& row : m_HexPlatform)
	{
		for (size_t i = 0; i < row.second.size(); ++i)
		{
			row.second[i].currentSpriteId = 0;
			row.second[i].stepCount = 0;
			ChangePlatformSprite(m_SwapSprites[0], row.first, uint32_t(i));
		}
	}
}

void Level::PreInitializeFlyingDisc(SLDWorldEntity& world, int startAmount)
{
	for (int i = 0; i < startAmount; ++i)
	{
		auto& ref{ m_FlyingDiscs.emplace_back(std::make_shared<FlyingDisc>(world, -1, -1)) };
		ref->SetLevelState(&m_CurrentState);
		Player::OnPlayerFinishedJump.AddDynamic(&FlyingDisc::OnPlayerFinishedJump, ref);
	}
}
