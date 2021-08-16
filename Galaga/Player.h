#pragma once

#include <SLDFramework.h>

class Player : public SLD::Prefab
{
public:

	Player(size_t playerIndex);
	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;

	
	inline static constexpr SLD::Vector4<uint32_t> PlayerSpriteRect{109,1,16,16};

	SLD::GameObjectId GetPlayerId() const;

	void OnDestroy(SLD::GameObjectId id);

private:

	SharedPtr<SLD::GameObject> m_Instance;
	size_t m_PlayerIndex;
};

