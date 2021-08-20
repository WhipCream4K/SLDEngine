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
#include "TextRenderSystem.h"
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

	sf::Font* font{ Instance<ResourceManager>()->Add<sf::Font>(MainFont, {}) };

	loadFileSuccess = mainTexture->loadFromFile("./Resources/SpriteSheet/GeneralSprite.png");
	loadFileSuccess = background->loadFromFile("./Resources/SpriteSheet/BG.png");
	loadFileSuccess = font->loadFromFile("./Resources/Fonts/VCR_OSD_MONO_1.001.ttf");

	if (loadFileSuccess)
	{
		const auto stateObject{ GameObject::Create(world) };
		stateObject->AddComponent<InputListener>({ 0 });
		GameStateComponent* gameState = stateObject->AddComponent<GameStateComponent>({ int(MaxEnemyCount),3 });

		gameState->state = std::make_shared<MenuState>();
		gameState->state->Enter(world,gameState);

		const auto bg{ GameObject::Create(world) };
		bg->GetComponent<TransformComponent>()->Translate(0.0f, 0.0f, float(Layer::Background));
		bg->AddComponent<SpriteRenderComponent>({ *background,{0,0,200,256},GlobalScale });


		world.AddSystem<SpriteRenderSystem>(world);
		world.AddSystem<HandleSpriteOutOfWindow>({ world,{1280,720} });
		world.AddSystem<GameStateSystem>({ world });
		world.AddSystem<TextRenderSystem>(world);

		Instance<EnemyPath>()->Initialize();
		Instance<SpawnStages>()->Initialize();

#ifdef  _DEBUG

		const auto& enemyPath{ Instance<EnemyPath>() };

		const auto testLine{ GameObject::Create(world) };
		LineComponent* line{ testLine->AddComponent<LineComponent>() };

		line->points = enemyPath->GetPath(SpawnDirection::TopLeft);

		const auto& topRight{ enemyPath->GetPath(SpawnDirection::TopRight) };
		const auto& bottomLeft{ enemyPath->GetPath(SpawnDirection::BottomLeft) };

		line->points.insert(line->points.end(), topRight.begin(), topRight.end());

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
		line->points.insert(line->points.end(), firstFormation.begin(), firstFormation.end());
		
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

		inputSettings.AddActionMapping("UI",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Up}}
			});

	}
}

void GalagaScene::Run()
{
	using namespace SLD;
	//bool passFirstFrame{};
	//float timePassed{};

	//WorldClock& clock{ m_Framework->GetClock() };
	//WorldEntity& world{ m_Framework->GetDefaultWorldEntity() };

	//const float dt = clock.GetDeltaTime();

	MTR_BEGIN("main", "Loop");

	while (m_Framework->TranslateUserInputs())
	{

		//if (passFirstFrame)
		//{
		//	timePassed += dt;
		//	world;
		//	//if(timePassed > 3.0f)
		//	//	world.DestroyGameObject(m_GIdTest);
		//}

		//passFirstFrame = true;
		m_Framework->Step();
	}

	MTR_END("main", "Loop");
}
