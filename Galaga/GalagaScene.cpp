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
#include "GameModeComponent.h"

#include "ResourceManager.h"
#include "HandleSpriteOutOfWindow.h"
#include "GameStateSystem.h"
#include "LineDebugDraw.h"
#include "EnemyPath.h"
#include "EnemyStateSystem.h"
#include "GameModeSystem.h"
#include "ReloadShooter.h"
#include "SpawnStages.h"
#include "TextRenderSystem.h"
#include "UpdateFormationWayPoints.h"
#include "UpdateParticle.h"
#include "Tracer/minitrace.h"
#include "ZakoDiveSystem.h"
#include "OnBodyContact.h"


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
	const auto& resourceManager{ Instance<ResourceManager>() };

	sf::Texture* mainTexture{ resourceManager->Add<sf::Texture>(MainSpriteSheet,{}) };
	sf::Texture* background{ resourceManager->Add<sf::Texture>("Background",{}) };
	sf::Texture* player{ resourceManager->Add<sf::Texture>("Player",{}) };
	sf::Texture* zako{ resourceManager->Add<sf::Texture>("Zako",{}) };
	sf::Texture* goei{ resourceManager->Add<sf::Texture>("Goei",{}) };
	sf::Texture* galagasGreen{ resourceManager->Add<sf::Texture>("GalagasYellow",{}) };
	sf::Texture* galagasPurple{ resourceManager->Add<sf::Texture>("GalagasPurple",{}) };
	sf::Texture* stage{ resourceManager->Add<sf::Texture>("Stage",{}) };

	sf::Font* font{ Instance<ResourceManager>()->Add<sf::Font>(MainFont, {}) };

	loadFileSuccess = mainTexture->loadFromFile("./Resources/SpriteSheet/GeneralSprite.png");
	loadFileSuccess = background->loadFromFile("./Resources/SpriteSheet/BG.png");
	loadFileSuccess = player->loadFromFile("./Resources/SpriteSheet/rocket.png");
	loadFileSuccess = zako->loadFromFile("./Resources/SpriteSheet/Blue Wings.png");
	loadFileSuccess = goei->loadFromFile("./Resources/SpriteSheet/Red Wings.png");
	loadFileSuccess = galagasGreen->loadFromFile("./Resources/SpriteSheet/Green.png");
	loadFileSuccess = galagasPurple->loadFromFile("./Resources/SpriteSheet/Purple.png");
	loadFileSuccess = stage->loadFromFile("./Resources/SpriteSheet/Flag.png");
	loadFileSuccess = font->loadFromFile("./Resources/Fonts/PressStart2P-Regular.ttf");

	if (loadFileSuccess)
	{
		// Set tiles for some texture
		{
			player->setRepeated(true);
			stage->setRepeated(true);
		}
		
		const auto gameModeObj{ GameObject::Create(world) };
		gameModeObj->AddComponent<GameModeComponent>({});

		const auto bg{ GameObject::Create(world) };
		bg->GetComponent<TransformComponent>()->Translate(0.0f, 0.0f, float(Layer::Background));
		bg->AddComponent<SpriteRenderComponent>({ *background,{0,0,200,256},GlobalScale });

		world.AddSystem<GameModeSystem>(world);
		world.AddSystem<GameStateSystem>(world);

		world.AddSystem<SpriteRenderSystem>(world);
		world.AddSystem<HandleSpriteOutOfWindow>({ world,{1280,720} });
		world.AddSystem<TextRenderSystem>(world);
		world.AddSystem<ZakoDiveSystem>(world);


		world.AddSystem<PlayerInputSystem>({ world });
		world.AddSystem<OnBulletContact>({ world });

		const rtm::float4f playArea{ 0,0,550.0f,720.0f };
		const float hopInterval{ 0.5f };

		world.AddSystem<LimitPlayerToPlayArea>({ world, playArea });
		world.AddSystem<UpdateProjectile>({ world,360.0f });
		world.AddSystem<UpdateParticle>({ world });
		world.AddSystem<ReloadShooter>({ world });

		world.AddSystem<UpdateFormationWayPoints>({ world,hopInterval });
		world.AddSystem<OnBodyContact>(world);

		Instance<EnemyPath>()->Initialize();
		Instance<SpawnStages>()->Initialize();

		const auto& enemyPath{ Instance<EnemyPath>() };
		const auto worldFormationTracker{ SLD::GameObject::Create(world) };
		worldFormationTracker->AddComponent<FormationWayPoints>({ enemyPath->GetFormationWayPoints() });
		enemyPath->SetFormationTracker(worldFormationTracker->GetId());
		
		world.AddSystem<EnemyStateSystem>({ world,worldFormationTracker->GetId() });

#ifdef  _DEBUG

		//const auto& enemyPath{ Instance<EnemyPath>() };

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

		inputSettings.AddActionMapping("Shoot",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Space}},
				{Key{InputDevice::D_Gamepad,KeyType(GamePadKey::GPK_A)}}
			});

		inputSettings.AddActionMapping("Enter",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::P}}
			});

		inputSettings.AddActionMapping("Select",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::LControl}}
			});

		inputSettings.AddActionMapping("Start",
			{
				{Key{InputDevice::D_Keyboard,sf::Keyboard::Enter}}
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
