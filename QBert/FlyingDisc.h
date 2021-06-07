#pragma once

#include <SLDFramework.h>

#include "Level.h"

class Player;
class FlyingDisc
{
public:

	FlyingDisc(SLDWorldEntity& world, int row, int col);

	void Update(float deltaTime);
	void AttachToPlayer(const RefPtr<Player>& player);
	void SetTexture(const sf::Texture& texture);
	void SetIsUsed(bool value);
	[[nodiscard]] bool IsUsed() const;
	Level::Node GetNode();
	void SetPos(const rtm::float3f& pos, int row, int col);
	void OnPlayerFinishedJump(const Level::Node& to);
	void PlayNaiveAnim(float deltaTime);
	void SetLevelState(Level::LevelState* address);

private:

	void SetUpSprite(SLDWorldEntity& world);

	Level::LevelState* m_LevelState;
	RefPtr<SLD::GameObject> m_GameObject;
	RefPtr<sf::Sprite> m_Sprite;
	RefPtr<SLD::Tracker> m_SpriteHandle;
	WeakPtr<Player> m_Parent;
	int m_RowPos;
	int m_ColPos;
	float m_StandingOffset{ 32.0f };
	float m_MoveSpeed{ 2.5f };
	float m_StayTime{ 1.0f };
	float m_StayTimeCount{};
	int m_AnimId{};
	float m_AnimTimeCount{};
	float m_AnimTime{ 0.1f };
	bool m_HasReached{};
	bool m_IsUsed{};
};

