#include "Player.h"

#include <SLDFramework.h>

#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "Rendering/RenderParams.h"
#include "Components/InputComponent.h"
#include "Sound/SoundManager.h"

#include "QBertParams.h"


Player::Player(SLD::WorldEntity& world)
{
	m_GameObject = world.CreateGameObject();
	m_MoveSpeed = 10.0f;

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->GetPtr()->SetScale(2.5f, 2.5f, 1.0f);
	const auto transform{ m_TransformComponent.lock() };

	// Setup Render Component
	const size_t renderData{ sizeof(sf::Sprite) };
	const uint32_t renderElemCnt{ 1 };
	//auto renderComponent = m_GameObject->CreateRenderingComponent(renderData, renderElemCnt);

	m_CharacterSprite = std::make_shared<sf::Sprite>();

	//renderComponent.lock()->GetPtr()->PushToRenderBuffer(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());

	// Sprite Creation
	//m_CharacterSprite = renderComponent->GetPtr()->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

	m_CharacterSpriteHandle = world.GetRenderBuffer().PushRenderElement(transform, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());

	transform->GetPtr()->Translate(-20.0f, 0.0f, float(QBert::Layer::Player));

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
	const size_t renderData{ sizeof(sf::Sprite) };
	const uint32_t renderElemCnt{ 1 };
	//auto renderComponent = m_GameObject->CreateRenderingComponent(renderData, renderElemCnt);

	m_CharacterSprite = std::make_shared<sf::Sprite>();

	//renderComponent.lock()->GetPtr()->PushToRenderBuffer(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());
	
	// Sprite Creation
	//m_CharacterSprite = renderComponent->GetPtr()->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

	m_CharacterSpriteHandle = world.GetRenderBuffer().PushRenderElement(transform, SLD::RenderIdentifier(SFMLRenderElement::RenderSprite), m_CharacterSprite.get());
	
	transform->GetPtr()->Translate(-20.0f, 0.0f, float(QBert::Layer::Player));

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
	//const auto temp{ m_CharacterSpriteHandle.get() };
	//std::copy_n((uint8_t*)m_CharacterSprite.get(), sizeof(sf::Sprite), *temp);
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

void Player::SetPos(const rtm::float3f& pos) const
{
	if (m_TransformComponent.lock())
		m_TransformComponent.lock()->GetPtr()->Translate(pos);
}

void Player::MoveUpRight()
{
	if(!m_IsMoving)
	{
		m_IsMoving = true;
		m_MoveDirection = MoveDirection::UpRight;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);

	}
}

void Player::MoveUpLeft()
{
	if(!m_IsMoving)
	{
		m_IsMoving = true;
		m_MoveDirection = MoveDirection::UpLeft;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::MoveDownRight()
{
	if(!m_IsMoving)
	{
		m_IsMoving = true;
		m_MoveDirection = MoveDirection::DownRight;
		m_CalculatedLocation = CalculatePath(m_MoveDirection, m_CurrentNode);

		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);
	}
}

void Player::MoveDownLeft()
{
	if(!m_IsMoving)
	{
		m_IsMoving = true;
		m_MoveDirection = MoveDirection::DownLeft;
		m_CalculatedLocation = CalculatePath(m_MoveDirection,m_CurrentNode);
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Jump, 0.5f);

	}
}

void Player::MoveHorizontal(float value)
{
	if (const auto transform{ m_TransformComponent.lock() })
	{
		const rtm::float3f& pos{ transform->GetPtr()->GetWorldPos() };
		const float speed{ 100.0f };
		const float horizon{ pos.x + (speed * value * m_World->GetDeltaTime()) };
		transform->GetPtr()->Translate(horizon, pos.y, pos.z);
	}
}

void Player::Update(float deltaTime)
{
	if(m_IsMoving)
	{

		// Check if we are near that Calculated Position
		if(auto transform = m_TransformComponent.lock();
			transform)
		{
			auto ptr{ transform->GetPtr() };

			rtm::vector4f currentPos{ rtm::vector_load3(&ptr->GetWorldPos()) };
			const rtm::vector4f targetPos{ rtm::vector_load3(&m_CalculatedLocation) };

			// Lerp To
			currentPos = rtm::vector_lerp(currentPos, targetPos, m_MoveSpeed * deltaTime);

			rtm::float3f floatCurrentPos{};
			rtm::vector_store3(currentPos, &floatCurrentPos);

			ptr->Translate(floatCurrentPos);
			
			// imma use intrinsic function here rather than distance squared
			const float threshold{ 1.0f };
			if (rtm::vector_all_near_equal3(currentPos, targetPos,threshold))
			{
				SetPos(m_CalculatedLocation);
				m_MoveDirection = MoveDirection::None;
				m_IsMoving = false;
			}
		}
	}
}


void Player::LerpTo(const rtm::float3f& to)
{
	to;
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

rtm::float3f Player::CalculatePath(MoveDirection dir, Level::Node& outNode)
{
	int targetRow{};
	int targetCol{};
	rtm::float3f targetPos{};
	
	switch (dir)
	{
	case MoveDirection::None: break;
	case MoveDirection::DownRight:

		targetRow = int(outNode.row) + 1;
		targetCol = int(outNode.col) + 1;

		break;
	case MoveDirection::DownLeft: 

		targetRow = int(outNode.row) + 1;
		targetCol = int(outNode.col);

		break;
	case MoveDirection::UpRight:

		targetRow = int(outNode.row) - 1;
		targetCol = int(outNode.col);

		break;
	case MoveDirection::UpLeft: 

		targetRow = int(outNode.row) - 1;
		targetCol = int(outNode.col) - 1;

		break;
	default: break;
	}

	// Check if this node is valid
	// if not then character is dead
	if(auto level = m_GameLevel.lock();
		level)
	{
		// early check if targetrow is in range of hexgrid
		auto& hexGrid{ level->GetGrid() };
		if(targetRow >= 0 && targetRow < int(hexGrid.size()))
		{
			auto& platForms{ hexGrid.at(uint32_t(targetRow)) };

			// early check if targetCol is in range of platforms
			if (targetCol >= 0 && targetCol < int(platForms.size()))
			{
				auto platFormTransform{ platForms[targetCol].gameObject->GetTransform()->GetPtr() };
				targetPos = platFormTransform->GetWorldPos();
				targetPos.y += QBert::LevelPixelY / 2.0f * QBert::GlobalScaleY;

				outNode.row = targetRow;
				outNode.col = targetCol;
			}
			// Dead
			else
			{
				
			}
		}
		// Dead
		else
		{
			
		}
	}

	return targetPos;
}
