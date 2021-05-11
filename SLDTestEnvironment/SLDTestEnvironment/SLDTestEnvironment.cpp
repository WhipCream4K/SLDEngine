// SLDTestEnvironment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <chrono>


#include "Components/RenderingComponent.h"
//#include "Components/SceneComponent.h"
#include "Core/Delegate.h"
#include "Rendering/RendererInterface.h"
#include "Core/Event.h"


using namespace SLD;

int mul(int x, int y)
{
	return x * y;
}

TEST_CASE("Test Event System")
{

	class UserClass
	{
	public:
		void Update() const
		{
			
		}
	};

	RefPtr<UserClass> subject{ std::make_shared<UserClass>() };
	
	RefPtr<EventHandler> ev1{ std::make_shared<Event<decltype(&UserClass::Update)>>(&UserClass::Update,subject) };

	ev1->Invoke();
	//int p1{ 2 }, p2{ 3 };
	//EventHandler ev1 = Event<int(int, int)>::Create(&mul, p1, p2);
	//
	//bool test{};
	//if(!test)
	//{
	//	ev1->Invoke();
	//	test = true;
	//}
	//
	//REQUIRE(test == true);
}

TEST_CASE("Test Delegate System")
{

	class World
	{
	public:
		int mul(int num1, int num2) { return num1 * num2; }
		void Update()
		{

		}
	};

	std::shared_ptr<World> smartWorld{ std::make_shared<World>() };

	const int p1{ 2 }, p2{ 3 };
	const short s1{ 2 }, s2{ 3 };
	Delegate<int(int, int)> myDel{ &mul };
	Delegate<int(int, int), World> myMemberDel{ &World::mul,smartWorld };
	std::cout << sizeof(myMemberDel) << '\n';
	REQUIRE(myDel(p1, p2) == mul(p1, p2));
	REQUIRE(myMemberDel(p1, p2) == mul(p1, p2));
	REQUIRE(myMemberDel(s1, s2) == mul(p1, p2));

	Delegate<int()> moveTest{};
	moveTest = []() {return 42; };
	std::cout << moveTest() << '\n';

	//std::cout << std::boolalpha << std::true_type::value << '\n';
	//std::cout << std::boolalpha << std::false_type::value << '\n';
	//std::cout << typeid(&ServiceLocator::mul).name() << '\n';
	//std::cout << std::is_same_v<decltype(&mul), decltype(&ServiceLocator::mul)> << '\n';
}

//TEST_CASE("Test SceneComponent", "[.]")
//{
//
//	class UserClass
//	{
//	public:
//		void OnUpdate(float dt)
//		{
//			m_ElapsedTime++;
//			dt;
//		}
//		void OnCreate()
//		{
//
//		}
//	private:
//		float m_ElapsedTime{};
//	};
//
//
//	class Base
//	{
//	public:
//		virtual void Update() = 0;
//		virtual ~Base() = default;
//	protected:
//		float m_ElapsedTime{};
//	};
//
//	class DerivedScene : public Base
//	{
//	public:
//		void Update() override
//		{
//			m_ElapsedTime++;
//		}
//	};
//
//	SECTION("Functionality")
//	{
//		std::shared_ptr<UserClass> subject{ std::make_shared<UserClass>() };
//		std::vector<SceneComponent> scenesVec{ 2 };
//
//		scenesVec[0] = SceneComponent("Test");
//		scenesVec[1] = SceneComponent("Test2");
//
//
//		scenesVec[0].Bind(subject);
//		scenesVec[1].Bind(subject);
//	}
//
//	SECTION("Benchmarking")
//	{
//		const size_t subjectCount{ 1000000 };
//		const size_t testAmount{ 500 };
//
//		//std::vector<std::shared_ptr<UserClass>> subject{ subjectCount };
//		std::shared_ptr<UserClass> subject{ std::make_shared<UserClass>() };
//
//		std::vector<SceneComponent> lambda{ subjectCount };
//		std::vector<Base*> virtualFunc{ subjectCount };
//		std::weak_ptr<UserClass> pWeak{ subject };
//
//
//		for (size_t i = 0; i < lambda.size(); ++i)
//		{
//			lambda[i].Bind<UserClass>(subject);
//		}
//
//		for (auto& val : virtualFunc)
//		{
//			val = new DerivedScene();
//		}
//
//
//		// Benchmark
//		float totalTime{};
//		float totalTime2{};
//
//		for (size_t i = 0; i < testAmount; ++i)
//		{
//			auto startClock = std::chrono::high_resolution_clock::now();
//
//			for (auto& lam : lambda)
//			{
//				lam(1.0f);
//			}
//
//			auto endclock = std::chrono::high_resolution_clock::now();
//
//			const float elapsedTime{ (float)std::chrono::duration_cast<std::chrono::microseconds>(endclock - startClock).count() };
//
//			totalTime += elapsedTime;
//		}
//
//		for (size_t i = 0; i < testAmount; ++i)
//		{
//			auto startClock = std::chrono::high_resolution_clock::now();
//
//			for (const auto& obj : virtualFunc)
//			{
//				obj->Update();
//			}
//
//			auto endclock = std::chrono::high_resolution_clock::now();
//
//			const float elapsedTime{ (float)std::chrono::duration_cast<std::chrono::microseconds>(endclock - startClock).count() };
//
//			totalTime2 += elapsedTime;
//		}
//
//
//		std::cout << "Avg time lambda function: " << totalTime / testAmount << '\n';
//		std::cout << "Avg time virtual function: " << totalTime2 / testAmount << '\n';
//	}
//}

#include "Rendering/RenderTarget.h"

TEST_CASE("RenderComponent Test")
{
	class UserClass
	{
	public:
		void OnDraw(const RefPtr<RendererInterface>&) const
		{
			std::cout << "Hello OnDraw\n";
		}
	};

	class TestRenderer : public RendererInterface
	{
	public:
		bool Present() override
		{
			return false;
		}
		bool Init(const RenderTarget&) override
		{
			return false;
		}
		bool ClearBackBuffer() override
		{
			return false;
		}
	};

	std::shared_ptr<UserClass> subject{ std::make_shared<UserClass>() };
	//RenderTarget rtv;
	//RenderingComponent renderComponent{ rtv };
	//renderComponent.Bind(subject);
	//renderComponent(std::make_shared<TestRenderer>());
}

TEST_CASE("Renderer Test")
{

}

#include "Core/CustomStaticArray.h"
#include "Core/MemAllocator.h"

class TestSubject
{
public:
	int GetVal() const { return m_pSomeMem ? *m_pSomeMem : 0; }
	TestSubject()
		: m_pSomeMem(new int(42))
	{
	}

	TestSubject& operator=(TestSubject&& other) noexcept
	{
		if(this != &other)
		{
			m_pSomeMem = other.m_pSomeMem;
			other.m_pSomeMem = nullptr;
		}

		return *this;
	}
	
	~TestSubject()
	{
		delete m_pSomeMem;
		m_pSomeMem = nullptr;
	}
private:
	int* m_pSomeMem;
};

template<size_t Count>
auto Scope()
{
	RefPtr<TestSubject[]> stack{ new TestSubject[Count] };
	return CustomStaticArray{ stack, Count, Count };
}

TEST_CASE("Custom Static Array Test")
{

	std::array<int, 5> globalResource{ 1,2,3,4,5 };

	SECTION("Insert and Erase test")
	{
		CustomStaticArray<int> myArr{ globalResource };

		REQUIRE(!myArr.insert(42).has_value());
		REQUIRE(myArr.erase(1) == CustomStaticArray<int>::Iterator(myArr.data() + 1));
		REQUIRE(myArr.insert(42).has_value());
	}

	SECTION("Iteration Test")
	{
		CustomStaticArray<int> myArr{ globalResource };

		for (const auto& val : myArr)
		{
			REQUIRE(val);
			std::cout << "Has value : " << val << '\n';
		}
	}

	SECTION("Object Test")
	{
		CustomStaticArray myArr{ Scope<5>() };

		myArr.erase(2);

		for (const auto& val : myArr)
		{
			std::cout << val.GetVal() << "\n";
		}

		myArr.insert(TestSubject());

		for (const auto& val : myArr)
		{
			std::cout << val.GetVal() << "\n";
		}
	}
}


#include "Core/VirMemDelegate.h"

inline void Update(float some)
{
	std::cout << some << '\n';
}

#include "Components/InputComponent.h"

TEST_CASE("InputComponent")
{
	//class UserClass
	//{
	//public:
	//	
	//	void Jump() const
	//	{
	//		std::cout << "Hello Jump\n";
	//	}

	//	void Update(float dt)
	//	{
	//		dt++;
	//	}
	//};

	//RefPtr<UserClass> obj{ std::make_shared<UserClass>() };
	//InputComponent subject{};
	//subject.BindAction("Jump", InputEvent::IE_Pressed,&UserClass::Jump,obj);
	//subject.BindAxis("Horizontal", &UserClass::Update, obj);
	//const auto some = subject.GetActionHandleFromMappingGroup("Jump", InputEvent::IE_Pressed);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
