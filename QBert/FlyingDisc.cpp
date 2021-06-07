#include "FlyingDisc.h"

#include "Player.h"
#include "QBertParams.h"
#include "Components/TransformComponent.h"
#include "Sound/SoundManager.h"

FlyingDisc::FlyingDisc(SLDWorldEntity& world, int row, int col)
	: m_RowPos(row)
	, m_ColPos(col)
{
	m_GameObject = world.CreateGameObject();
	SetUpSprite(world);
}

void FlyingDisc::Update(float deltaTime)
{
	if (m_IsUsed)
	{
		if (auto player{ m_Parent.lock() };
			player)
		{
			{
				player->SetPlayerState(Player::State::Lock);

				const auto playerTransform{ player->GetGameObject()->GetTransform()->GetPtr() };

				const auto discTransform{ m_GameObject->GetTransform()->GetPtr() };
				rtm::vector4f discPos{ rtm::vector_load3(&discTransform->GetWorldPos()) };

				// Both Lerp to Top
				rtm::vector4f targetPos{ rtm::vector_load3(&Level::TopOfTheMountain) };
				discPos = rtm::vector_lerp(discPos, targetPos, deltaTime * m_MoveSpeed);

				// Check if we are near that Calculated Position
				// imma use intrinsic function here rather than distance squared
				const float threshold{ 1.0f };
				if (rtm::vector_all_near_equal3(discPos, targetPos, threshold))
				{
					m_IsUsed = false;
					m_RowPos = uint32_t(-1);
					m_ColPos = uint32_t(-1);
					m_HasReached = true;
				}

				rtm::float3f floatDiscPos{};
				rtm::vector_store3(discPos, &floatDiscPos);
				floatDiscPos.z = float(QBert::Layer::Map);
				discTransform->Translate(floatDiscPos);

				auto playerPos{ playerTransform->GetWorldPos() };
				playerPos.y = (floatDiscPos.y + m_StandingOffset);
				playerPos.x = (floatDiscPos.x);

				playerTransform->Translate(playerPos);
			}
		}
	}

	if (m_HasReached)
	{
		m_StayTimeCount += deltaTime;
		if (m_StayTimeCount >= m_StayTime)
		{
			m_StayTimeCount -= m_StayTime;
			m_HasReached = false;
			if (auto parent{ m_Parent.lock() };
				parent)
			{
				parent->SetPlayerState(Player::State::None);
				parent->Move(Level::Node{ 0,0 });
			}


			// LMAO JUST WHOOSH AWAY
			m_GameObject->GetTransform()->GetPtr()->Translate(0.0f, 1000.0f, float(QBert::Layer::Map));
		}
	}

	PlayNaiveAnim(deltaTime);
}

void FlyingDisc::AttachToPlayer(const RefPtr<Player>& player)
{
	m_Parent = player;
}

void FlyingDisc::SetTexture(const sf::Texture& texture)
{
	m_Sprite->setTexture(texture);
	CopyTextureRegion(m_Sprite.get(), m_SpriteHandle);
}

void FlyingDisc::SetIsUsed(bool value)
{
	m_IsUsed = value;
}

bool FlyingDisc::IsUsed() const
{
	return m_IsUsed;
}

Level::Node FlyingDisc::GetNode()
{
	return Level::Node{ m_RowPos,m_ColPos };
}

void FlyingDisc::SetPos(const rtm::float3f& pos, int row, int col)
{
	m_GameObject->GetTransform()->GetPtr()->Translate(pos);
	m_RowPos = row;
	m_ColPos = col;
}

void FlyingDisc::OnPlayerFinishedJump(const Level::Node& to)
{
	if (to.row == m_RowPos && to.col == m_ColPos)
	{
		m_IsUsed = true;
		SLD::Instance<SLD::SoundManager>()->PlayStream(QBert::Sound::Disc, 0.5f);
	}
}

void FlyingDisc::PlayNaiveAnim(float deltaTime)
{
	m_AnimTimeCount += deltaTime;

	if (m_AnimTimeCount >= m_AnimTime)
	{
		m_AnimTimeCount = 0;

		auto textureRect{ m_Sprite->getTextureRect() };

		sf::IntRect mainRect{};

		switch (*m_LevelState)
		{
		case Level::LevelState::Level1:

			mainRect = QBert::Level1::FlyingDisc;

			break;
		case Level::LevelState::Level2:	mainRect = QBert::Level2::FlyingDisc; break;
		case Level::LevelState::Level3:	mainRect = QBert::Level3::FlyingDisc; break;
		case Level::LevelState::Count:break;
		default:;
		}

		const int spriteInAnim{ 4 };
		m_AnimId = (m_AnimId + 1) % spriteInAnim;
		textureRect.left = mainRect.left + (16 * m_AnimId);
		m_Sprite->setTextureRect(textureRect);

		CopyTextureRegion(m_Sprite.get(), m_SpriteHandle);
	}
}

void FlyingDisc::SetLevelState(Level::LevelState* address)
{
	m_LevelState = address;
}

void FlyingDisc::SetUpSprite(SLDWorldEntity& world)
{
	m_Sprite = std::make_shared<sf::Sprite>();
	m_Sprite->setTextureRect(QBert::Level1::FlyingDisc);
	m_Sprite->setOrigin(float(QBert::Level1::FlyingDisc.width) * 0.5f, float(QBert::Level1::FlyingDisc.height) * 0.5f);

	auto transform{ m_GameObject->GetTransform() };

	transform->GetPtr()->SetScale(QBert::GlobalScaleX, QBert::GlobalScaleY, float(QBert::Layer::Map));

	m_SpriteHandle = world.GetRenderBuffer().PushRenderElement(
		transform,
		SLD::RenderIdentifier(SFMLRenderElement::RenderSprite),
		m_Sprite.get()
	);
}
