#include "Particle.h"
#include "MyComponents.h"
#include "ResourceManager.h"
#include "GalagaScene.h"

Particle::Particle(const SLD::Vector4<uint32_t>& rect)
	: m_SpriteRect(rect)
{
}

void Particle::OnCreate(const SharedPtr<SLD::GameObject>& gameObject)
{
	using namespace SLD;
	
	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if(mainTexture)
		gameObject->AddComponent<SpriteRenderComponent>({*mainTexture,m_SpriteRect,GalagaScene::GlobalScale});

	gameObject->AddComponent<ParticleComponent>({ 1.0f });
}
