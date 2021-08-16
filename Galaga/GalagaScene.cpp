#include "GalagaScene.h"
#include <SLDFramework.h>
#include "SpriteRenderSystem.h"
#include "PlayerInputSystem.h"
#include "OnBulletContact.h"
#include "Player.h"
#include "Box2DDebugDraw.h"
#include "LimitPlayerToPlayArea.h"
#include "UpdateProjectile.h"
#include <Helpers/utils.h>

#include "ResourceManager.h"
#include "HandleSpriteOutOfWindow.h"
#include "GameStateSystem.h"
#include "LineDebugDraw.h"
#include "EnemyPath.h"
#include "SpawnStages.h"
#include "Tracer/minitrace.h"


GalagaScene::GalagaScene()
	: m_Framework(std::make_shared<SLD::Core>())
{
	m_Framework->CreateWindowFrame(1280, 720, "SAD_GALAGA_");
}

void GalagaScene::WorldCreation()
{
	using namespace SLD;
	using namespace InputParams;

	SLD::WorldEntity& world{ m_Framework->GetDefaultWorldEntity() };

	bool loadFileSuccess{};

	sf::Texture* mainTexture{ SLD::Instance<ResourceManager>()->Add<sf::Texture>(MainSpriteSheet,{}) };
	sf::Texture* background{ SLD::Instance<ResourceManager>()->Add<sf::Texture>("Background",{}) };
	
	loadFileSuccess = mainTexture->loadFromFile("./Resources/SpriteSheet/GeneralSprite.png");
	loadFileSuccess = background->loadFromFile("./Resources/SpriteSheet/BG.png");

	if (loadFileSuccess)
	{

		const auto stateObject{ GameObject::Create(world) };
		stateObject->AddComponent<InputListener>({ 0 });
		stateObject->AddComponent<GameStateComponent>({ GameState::Menu });

		const auto bg{ GameObject::Create(world) };
		bg->GetComponent<TransformComponent>()->Translate(0.0f, 0.0f, float(Layer::Background));
		bg->AddComponent<SpriteRenderComponent>({ *background,{0,0,200,256},GlobalScale });

		//const auto testLine{ GameObject::Create(world) };
		//testLine->AddComponent<LineComponent>({ {-100.0f,0.0f},{100.0f,0.0f} });

		
		
		world.AddSystem<SpriteRenderSystem>(world);
		world.AddSystem<HandleSpriteOutOfWindow>({ world,{1280,720} });
		world.AddSystem<GameStateSystem>({ world });

		Instance<EnemyPath>()->Initialize();
		Instance<SpawnStages>()->Initialize();

		const auto& enemyPath{ Instance<EnemyPath>() };
		
		const auto testLine{ GameObject::Create(world) };
		LineComponent* line{ testLine->AddComponent<LineComponent>() };
		
		line->points = enemyPath->GetPath(SpawnDirection::TopLeft);
		
		const auto& topRight{ enemyPath->GetPath(SpawnDirection::TopRight) };
		const auto& bottomLeft{ enemyPath->GetPath(SpawnDirection::BottomLeft) };
		
		line->points.insert(line->points.end(),topRight.begin(),topRight.end());

		const auto test2{ GameObject::Create(world) };
		line = test2->AddComponent<LineComponent>();
		line->points.insert(line->points.end(), bottomLeft.begin(), bottomLeft.end());

		const auto& bottomRight{ enemyPath->GetPath(SpawnDirection::BottomRight) };
		
		const auto test3{ GameObject::Create(world) };
		line = test3->AddComponent<LineComponent>();
		
		line->points.insert(line->points.end(), bottomRight.begin(), bottomRight.end());

		const auto test4{ GameObject::Create(world) };
		line = test4->AddComponent<LineComponent>();

		const auto& firstFormation{ enemyPath->GetFormationWayPoints()[4] };
		line->points.insert(line->points.end(),firstFormation.begin(),firstFormation.end());
		
		//InstantiatePrefab<Player>(world, { 0 }, {-200,0.0f,float(Layer::Player)});
		//InstantiatePrefab<Player>(world, { 0 }, {-140.0f,0.0f,float(Layer::Player)});
		//InstantiatePrefab<Player>(world, { 0 }, {0.0f,0.0f,float(Layer::Player)});
		//m_GIdTest = InstantiatePrefab<Player>(world, { 0 }, {140.0f,0.0f,float(Layer::Player)});
		//InstantiatePrefab<Player>(world, { 0 }, {200.0f,0.0f,float(Layer::Player)});
		
		//world.AddSystem<PlayerInputSystem>(world);
		//world.AddSystem<OnBulletContact>(world);
		//world.AddSystem<LimitPlayerToPlayArea>({world,playArea});
		//world.AddSystem<UpdateProjectile>({world,370.0f});
		//world.AddSystem<ReloadShooter>({world});
		//world.AddSystem<UpdateParticle>({ world });

#ifdef  _DEBUG
		world.AddSystem<LineDebugDraw>(world);
		world.AddSystem<Box2DDebugDraw>(world);
#endif

		// Set World input
		InputSettings& inputSettings{ world.GetWorldInputSetting() };

		inputSettings.AddAxisMapping("Horizontal",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Left},-1.0f},
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Right},1.0f},
				{Key{InputDevice::D_Gamepad,KeyType(GamePadKey::GPK_Left_AxisX)},1.0f}
			});

		inputSettings.AddAxisMapping("Vertical",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Up},1.0f},
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Down},-1.0f}
			});

		inputSettings.AddActionMapping("Shoot",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Space}},
				{Key{InputDevice::D_Gamepad,KeyType(GamePadKey::GPK_A)}}
			});

		inputSettings.AddActionMapping("Enter",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::P}}
			});


	}
}

void GalagaScene::Run()
{
	using namespace SLD;
	bool passFirstFrame{};
	float timePassed{};

	WorldClock& clock{ m_Framework->GetClock() };
	WorldEntity& world{ m_Framework->GetDefaultWorldEntity() };

	const float dt = clock.GetDeltaTime();

	if (passFirstFrame)
	{
		timePassed += dt;
		world;
		//if(timePassed > 3.0f)
		//	world.DestroyGameObject(m_GIdTest);
	}
	
		passFirstFrame = true;
	while (m_Framework->TranslateUserInputs())
	{
		/*
		 * a bunch of code here
		 */

		m_Framework->Step();
	}
}
