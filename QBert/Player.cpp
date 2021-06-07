#include "Player.h"

#include <SLDFramework.h>

#include "FlyingDisc.h"
#include "QBertGame.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "Rendering/RenderParams.h"
#include "Components/InputComponent.h"
#include "Sound/SoundManager.h"

#include "QBertParams.h"

Player::FPlayerFinishedJump Player::OnPlayerFinishedJump{};
Player::FPlayerDied Player::OnPlayerDied{};

Player::Player(SLD::WorldEntity& world)
{
	m_GameObject = world.CreateGameObject();
	m_MoveSpeed = 10.0f;

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->GetPtr()->SetScale(2.5f, 2.5f, 1.0f);
	const auto transform{ m_TransformComponent.lock() };

	// Setup Render Component
	m_CharacterSprite = std::make_shared<sf::Sprite>();
	m_CharacterSpriteHandle = world.GetRenderBuffer().PushRenderElement(transform, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());


	m_InputComponent = m_GameObject->CreateComponent<SLD::InputComponent>();
}

Player::Player(SLD::WorldEntity& world, const std::shared_ptr<Level>& gameLevel)
	: m_World(&world)
	, m_GameLevel(gameLevel)
{
	m_GameObject = world.CreateGameObject();
	m_MoveSpeed = 10.0f;

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->GetPtr()->SetScale(2.5f, 2.5f, 1.0f);
	const auto transform{ m_TransformComponent.lock() };

	// Setup Render Component
	m_CharacterSprite = std::make_shared<sf::Sprite>();
	m_CharacterSpriteHandle = world.GetRenderBuffer().PushRenderElement(transform, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());

	m_InputComponent = m_GameObject->CreateComponent<SLD::InputComponent>();
}

void Player::SetSpriteTexture(const sf::Texture& texture) const
{
	//m_CharacterSprite->GetPtr()->setTexture(texture, true);
	auto ref{ m_CharacterSprite };
	ref->setTexture(texture, true);

	// Set to correct sprite rect
	const sf::IntRect spriteRect{
		0,0,16,16
	};
	//m_CharacterSprite->GetPtr()->setTextureRect(spriteRect);
	//m_CharacterSprite->GetPtr()->setOrigin(16.0f * 0.5f, 16.0f * 0.5f);
	ref->setTextureRect(spriteRect);
	ref->setOrigin(16.0f * 0.5f, 16.0f * 0.5f);

	CopyTextureRegion(m_CharacterSprite.get(), m_CharacterSpriteHandle);
}

void Player::SetUpPlayerInput()
{
	if (const auto inputComponent{ m_InputComponent.lock() }; inputComponent)
	{
		auto ptr{ inputComponent->GetPtr() };

		ptr->BindAction("MoveUpRight", SLD::InputEvent::IE_Released, &Player::MoveUpRight, this);
		ptr->BindAction("MoveUpLeft", SLD::InputEvent::IE_Released, &Player::MoveUpLeft, this);
		ptr->BindAction("MoveDownRight", SLD::InputEvent::IE_Released, &Player::MoveDownRight, this);
		ptr->BindAction("MoveDownLeft", SLD::InputEvent::IE_Released, &Player::MoveDownLeft, this);
	}
}

void Player::SetSpriteColor(const sf::Color& color)
{
	m_CharacterSprite->setColor(color);
	CopyTextureRegion(m_CharacterSprite.get(), m_CharacterSpriteHandle);
}

void Player::SetPos(const rtm::float3f& pos) const
{
	if (m_TransformComponent.lock())
		m_TransformComponent.lock()->GetPtr()->Translate(pos);
}

void Player::MoveUpRight()
{
	if (m_State == State::None)
	{
		m_State = State::Moving;
		m_MoveDirection = MoveDirection::UpRight;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::MoveUpLeft()
{
	if (m_State == State::None)
	{
		m_State = State::Moving;
		m_MoveDirection = MoveDirection::UpLeft;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}

}

void Player::MoveDownRight()
{
	if (m_State == State::None)
	{
		m_State = State::Moving;
		m_MoveDirection = MoveDirection::DownRight;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::MoveDownLeft()
{
	if (m_State == State::None)
	{
		m_State = State::Moving;
		m_MoveDirection = MoveDirection::DownLeft;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::MoveHorizontal(float value)
{
	if (m_State == State::None)
	{
		if (const auto transform{ m_TransformComponent.lock() })
		{
			const rtm::float3f& pos{ transform->GetPtr()->GetWorldPos() };
			const float speed{ 100.0f };
			const float horizon{ pos.x + (speed * value * m_World->GetDeltaTime()) };
			transform->GetPtr()->Translate(horizon, pos.y, pos.z);
		}
	}
}

void Player::Move(const Level::Node& to)
{
	if (m_State == State::None)
	{
		m_State = State::Moving;
		m_CurrentNode = to;

		if (auto level{ m_GameLevel.lock() };
			level)
		{
			auto& grid{ level->GetGrid() };
			if (to.row < int(grid.size()) &&
				to.col < int(grid.at(to.row).size()))
			{
				auto platformTransform{ grid.at(to.row)[to.col].gameObject->GetTransform()->GetPtr() };
				m_CalculatedLocation = platformTransform->GetWorldPos();
				m_CalculatedLocation.z = float(QBert::Layer::Player);

				const float halfStep{ QBert::LevelPixelY / 2.0f };				
				m_CalculatedLocation.y += halfStep * QBert::GlobalScaleY;
			}
		}

		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::Update(float deltaTime)
{
	// NOTE: Try to ignore this spaghetti
	switch (m_State)
	{
	case State::None: break;
	case State::Moving:
	{
		const rtm::vector4f targetPos{ rtm::vector_load3(&m_CalculatedLocation) };
		const rtm::vector4f currentPos{ LerpTo(targetPos, deltaTime) };

		// Check if we are near that Calculated Position
		// imma use intrinsic function here rather than distance squared
		const float threshold{ 1.0f };
		if (rtm::vector_all_near_equal3(currentPos, targetPos, threshold))
		{
			SetPos(m_CalculatedLocation);
			m_MoveDirection = MoveDirection::None;
			m_State = State::None;
			OnPlayerFinishedJump.BroadCast(m_CurrentNode);
		}
	}
	break;
	case State::Fall:
	{
		m_ReSpawnTimeCount += deltaTime;
		m_FlashBlackCount += deltaTime;

		if (m_ReSpawnTimeCount >= m_ReSpawnTime)
		{
			m_State = State::None;
			m_ReSpawnTimeCount -= m_ReSpawnTime;

			// ReSpawn Here
			if (const auto level{ m_GameLevel.lock() };
				level)
			{
				auto& hexGrid{ level->GetGrid() };
				if (m_CurrentNode.row < int(hexGrid.size()))
				{
					if (m_CurrentNode.col < int(hexGrid.at(m_CurrentNode.row).size()))
					{
						auto lastPlatformTransform{ hexGrid.at(m_CurrentNode.row)[m_CurrentNode.col].gameObject->GetTransform()->GetPtr() };
						rtm::float3f resPos{ lastPlatformTransform->GetWorldPos() };
						resPos.y += QBert::LevelPixelY / 2.0f * QBert::GlobalScaleY;
						resPos.z = float(QBert::Layer::Player);
						SetPos(resPos);
						SetSpriteColor({ 255,255,255,255 });
					}
				}
			}
			break;
		}

		if (m_MoveDirection == MoveDirection::None)
		{
			if (m_FlashBlackCount >= m_FlashBlackInterval)
			{
				// Flash white
				m_FlashBlackCount -= m_FlashBlackInterval;
				const auto switchColor{ m_CharacterSprite->getColor().a == 255 ?
				sf::Color{0,0,0,0} :
				sf::Color{255,255,255,255} };

				SetSpriteColor(switchColor);
			}

			break;
		}

		const rtm::vector4f targetPos{ rtm::vector_load3(&m_CalculatedLocation) };
		const rtm::vector4f currentPos{ LerpTo(targetPos, deltaTime) };

		const float threshold{ 1.0f };
		if (rtm::vector_all_near_equal3(currentPos, targetPos, threshold))
		{
			SetPos(m_CalculatedLocation);
			m_MoveDirection = MoveDirection::None;
		}

	}
	break;
	case State::Lock: break;
	default: break;
	}
}


void Player::LerpTo(const rtm::float3f& to, float deltaTime)
{
	if (auto transform{ m_TransformComponent.lock() };
		transform)
	{
		auto ptr{ transform->GetPtr() };

		rtm::vector4f currentPos{ rtm::vector_load3(&ptr->GetWorldPos()) };
		const rtm::vector4f targetPos{ rtm::vector_load3(&to) };

		// Lerp To
		currentPos = rtm::vector_lerp(currentPos, targetPos, m_MoveSpeed * deltaTime);

		rtm::float3f floatCurrentPos{};
		rtm::vector_store3(currentPos, &floatCurrentPos);

		ptr->Translate(floatCurrentPos);
	}
}

rtm::vector4f Player::LerpTo(const rtm::vector4f& to, float deltaTime)
{
	if (auto transform{ m_TransformComponent.lock() };
		transform)
	{
		auto ptr{ transform->GetPtr() };

		rtm::vector4f currentPos{ rtm::vector_load3(&ptr->GetWorldPos()) };

		// Lerp To
		currentPos = rtm::vector_lerp(currentPos, to, m_MoveSpeed * deltaTime);

		rtm::float3f floatCurrentPos{};
		rtm::vector_store3(currentPos, &floatCurrentPos);

		ptr->Translate(floatCurrentPos);
		return currentPos;
	}

	return {};
}

std::shared_ptr<SLD::GameObject> Player::GetGameObject()
{
	return m_GameObject;
}

void Player::SetCurrentNode(uint32_t row, uint32_t col)
{
	m_CurrentNode.row = row;
	m_CurrentNode.col = col;
}

void Player::OnLevelChange(Level::LevelState)
{
	SetPos(QBertGame::Player1DefaultSpawnPoint);
	m_CurrentNode.col = 0;
	m_CurrentNode.row = 0;
	m_State = State::None;
}

void Player::OnLevelRestart(Level::LevelState)
{
	SetPos(QBertGame::Player1DefaultSpawnPoint);
	m_CurrentNode.col = 0;
	m_CurrentNode.row = 0;
	m_State = State::None;
	m_CurrentLives = QBert::PlayerMaxLives;
}

void Player::OnGameClear()
{
	m_State = State::Lock;
}

void Player::SetPlayerState(State state)
{
	m_State = state;
}

rtm::float3f Player::CalculatePath(MoveDirection dir, Level::Node& outNode)
{
	int targetRow{};
	int targetCol{};
	rtm::float3f targetPos{ m_TransformComponent.lock()->GetPtr()->GetWorldPos() };

	const float halfSide{ QBert::LevelPixelX / 2.0f };
	const float halfStep{ QBert::LevelPixelY / 2.0f };
	const float doubleHalfStep{ QBert::LevelPixelY / 4.0f };

	switch (dir)
	{
	case MoveDirection::None: break;
	case MoveDirection::DownRight:

		targetRow = int(outNode.row) + 1;
		targetCol = int(outNode.col) + 1;

		targetPos.x += halfSide * QBert::GlobalScaleX;
		targetPos.y -= (halfStep + doubleHalfStep) * QBert::GlobalScaleY;

		break;
	case MoveDirection::DownLeft:

		targetRow = int(outNode.row) + 1;
		targetCol = int(outNode.col);

		targetPos.x -= halfSide * QBert::GlobalScaleX;
		targetPos.y -= (halfStep + doubleHalfStep) * QBert::GlobalScaleY;

		break;
	case MoveDirection::UpRight:

		targetRow = int(outNode.row) - 1;
		targetCol = int(outNode.col);

		targetPos.x += halfSide * QBert::GlobalScaleX;
		targetPos.y += (halfStep + doubleHalfStep) * QBert::GlobalScaleY;

		break;
	case MoveDirection::UpLeft:

		targetRow = int(outNode.row) - 1;
		targetCol = int(outNode.col) - 1;

		targetPos.x -= halfSide * QBert::GlobalScaleX;
		targetPos.y += (halfStep + doubleHalfStep) * QBert::GlobalScaleY;

		break;
	default: break;
	}

	bool isDead{};
	// Check if this node is valid
	// if not then character is dead
	if (auto level = m_GameLevel.lock();
		level)
	{
		// early check if targetrow is in range of hexgrid
		auto& hexGrid{ level->GetGrid() };
		if (targetRow >= 0 && targetRow < int(hexGrid.size()))
		{
			auto& platForms{ hexGrid.at(uint32_t(targetRow)) };

			// early check if targetCol is in range of platforms
			if (targetCol >= 0 && targetCol < int(platForms.size()))
			{
				outNode.row = targetRow;
				outNode.col = targetCol;
			}
			// Check for disc
			else
			{
				if (level->IsFlyingDiscExist(Level::Node{ targetRow,targetCol }))
				{	
					outNode.row = targetRow;
					outNode.col = targetCol;

					level->UseFlyingDisc(shared_from_this(), targetRow, targetCol);
					
				}
				else
				{
					isDead = true;
				}
			}
		}
		// Dead No Disc
		else
		{
			isDead = true;
		}
	}

	if (isDead)
	{
		m_State = State::Fall;
		m_CurrentLives--;
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Lose, 0.5f);
		OnPlayerDied.BroadCast(m_CurrentLives);
	}

	return targetPos;
}
