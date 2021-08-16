After, my last take on the game engine I wasn’t really thinking of programming patterns at all and focus on something else that doesn’t fit the whole topic. 
    
I finally have a chance to rethink and do the thing I was interested and suppose to do in the first time.


# Lite ECS
This is my take on ECS (Entity Component System). I’ll try to explain what I’ve done for this game engine

## GameObject
Speaking of ECS GameObject doesn’t really hold any state or variables. It only consists of a reference to world and unique Id to tell the world to point to the correct GameObject in the system although I put some functionality for ease of use and **of course it’s final.**

```
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		
		GameObject(WorldEntity& world);


		static SharedPtr<GameObject> Create(SLD::WorldEntity& world);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GetComponent();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* AddComponent(ComponentType&& val);

		template<typename ComponentType, typename ...Args>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* AddComponent(Args&&... args);

#pragma region deprecated
		
		[[nodiscard]] const SharedPtr<ObservePtr<TransformComponent>>& GetTransform() const;
		template<typename ComponentType,
			typename = EnableIsBasedOf<BaseComponent, ComponentType>,
			typename ...Args>
			[[nodiscard]] WeakPtr<ObservePtr<ComponentType>> CreateComponent(Args&&... args);

		WeakPtr<ObservePtr<RenderingComponent>> CreateRenderingComponent(size_t elemSize, uint32_t elemCnt);
		
		
#pragma endregion 
		
		GameObjectId GetId() const;

		WorldEntity& GetWorld();

		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(GameObject&& other) noexcept;
		~GameObject();

	private:


		template<typename T, typename ...Args>
		T* AllocComponentInArchetype(const Record& gameObjectRec, const SharedPtr<Archetype>& archetype, Args&&... args);

		void RemoveGameObjectFromArchetype(const Record& oldRecord, GameObjectId id);

	private:
		
		GameObjectId m_Id;
		WorldEntity& m_World;

#pragma region deprecated
		SharedPtr<ObservePtr<TransformComponent>> m_Transform;
#pragma endregion 
	};

 ```

![GameObjectClass](https://i.imgur.com/Jav9NDR.png)

## Component
Component usually holds data for one aspect of a GameObject. This data will then be used in the “System” or the logic. It has a unique Id type for every created Component in the world and needed to register to the world to use some move or destruct operations.

Every component that the user has created must inherited from ```class ComponentT<typename T>``` to be able to register in the world

```	
class BaseComponent
	{
		friend class GameObject;
	
	public:
		
		virtual ~BaseComponent() = default;

		/// <summary>
		/// Calls Destructor of derived component
		/// NOTE: doesn't free any memory
		/// </summary>
		/// <param name="address"></param>
		virtual void DestroyData(uint8_t* address) = 0;
		virtual void MoveData(uint8_t* source, uint8_t* destination) const = 0;
		[[nodiscard]] virtual size_t GetSize() const = 0;

		
	};
 ```

```
	template<typename T>
	class ComponentT : public BaseComponent
	{
	public:

		static ComponentTypeId GetId();
		virtual void DestroyData(uint8_t* address) override;
		virtual void MoveData(uint8_t* source, uint8_t* destination) const override;
		[[nodiscard]] virtual size_t GetSize() const override;
		virtual ~ComponentT() override = default;
	};

	template <typename T>
	void ComponentT<T>::DestroyData(uint8_t* address)
	{
		T* object{ reinterpret_cast<T*>(address) };

		object->~T();
	}

	template <typename T>
	void ComponentT<T>::MoveData(uint8_t* source, uint8_t* destination) const
	{
		new (destination) T{ std::move(*reinterpret_cast<T*>(source)) };
	}

	template <typename T>
	size_t ComponentT<T>::GetSize() const
	{
		return sizeof(T);
	}

	template <typename T>
	ComponentTypeId ComponentT<T>::GetId()
	{
		return UniqueIdGenerator<BaseComponent>::GetNewId<T>();
	}
```
![Screenshot 2021-08-16 163249](https://i.imgur.com/5ScqX39.png)
![Screenshot 2021-08-16 163304](https://i.imgur.com/QVcnWSX.png)

## Archetype

So we got a GameObject and a component now how do we access and give it functionality for it. Enters ```Archetype``` I got interested in this idea when I stumble upon [Unity ECS Concepts](https://docs.unity3d.com/Packages/com.unity.entities@0.2/manual/ecs_core.html) and [Fast LightWeighted Entity Component System (flecs)](https://github.com/SanderMertens/flecs) a structure that groups all sorts of component types in one "Archetype" with a key generate from unique Id of components.

![Ec4KB](https://i.stack.imgur.com/Ec4KB.png)

From this diagram if two entities have the same type of components both entities will share the same Archetype and only one Archetype will be generated.

```
	struct Archetype
	{
		Archetype() = default;

		SharedPtr<Archetype> Clone()
		{
			SharedPtr<Archetype> out{ std::make_shared<Archetype>() };

			out->types = types;
			out->gameObjectIds = gameObjectIds;
			out->totalLength = totalLength;
			out->sizeOfStruct = sizeOfStruct;
			out->componentData = SharedPtr<uint8_t[]>{ new uint8_t[totalLength]{} };

			if(!gameObjectIds.empty())
			{
				std::copy_n(
					componentData.get(),
					sizeOfStruct * int(gameObjectIds.size()),
					out->componentData.get());
			}

			return out;
		}

		std::vector<ComponentTypeId> types; // types of components of this archetypes
		std::vector<GameObjectId> gameObjectIds;
		SharedPtr<uint8_t[]> componentData{}; // Array of component struct
		int sizeOfStruct{};
		int totalLength{};
	};
```
![Screenshot 2021-08-16 171533](https://i.imgur.com/W3nzoag.png)

As you can see that the "types" of Archetype is just a vector of ComponentTypeId so this can be easily checked against ```System``` or anything for that matter.

### SoA structure

my Archetype owns ```componentData``` which is and array of ```uint8``` or ```byte``` to store an actual data existing components. Now, for most ECS this ```componentData``` will be structured as vector of **Array of Struct** so it's easily accessd and keep track of but I decided to go with **Structure of Array** instead. From this point on I will go by my assumptions because I didn't profile both of them to compare to really checked but I think this is the most suitable structure

Why? 
- During System updates. The ```System``` will go through each archetype and then ```componentData``` if the update logic uses data directly from the memory "I most likely get the benefits from **Data Locality** and didn't waste the wait time to throw the whole **Cache line** and catch a new one for next component, basically every component laid out after one another like the exercise1 that we did."

![Note 16 Aug BE 2564 (1)](https://i.imgur.com/sjRzrJx.jpg)

The Bad
- It is more challenge though to keep track of the memory and to picture how it's occupied the memory and move stuff around.
- It is slower when randomly access because it has to iterate through every type first if it's matched then return the pointer to data with offset

```SizeOfStruct``` and ```TotalLength``` is used in rumtime operations where you need to know the size of Archetype and the whole structure of components in this Archetype.


## System

System, the logic of the world, The system will iterate all of the archetype given by ```WorldEntity``` and process the matched ```Archetype``` . The system consists of the layer that this system will activate, system key this key will be used to check against ```Archetpye``` type that basically if the key doesn't match the range of component, it will not get called, lastly a reference to the world.

```
	class SystemBase
	{
	public:

		SystemBase(WorldEntity& world, PipelineLayer layer);
		virtual ~SystemBase() = default;

		virtual void InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype) = 0;
		virtual void InternalRender(const SharedPtr<Window>& winHandle, const SharedPtr<Archetype>& archetype) = 0;
		virtual void InternalRender(const SharedPtr<Window>&, std::vector<RenderObject>&) {}
		
		const std::string& GetKey() const;
		PipelineLayer GetLayer() const;

	protected:

		PipelineLayer m_Layer;
		std::string m_Key;
		WorldEntity& m_World;
	};
```
![Screenshot 2021-08-16 174352](https://i.imgur.com/aWLxfkJ.png)

# Runtime

## Game Loop

I keep the same game loop where the user has to specify their own loop and then they need to manually tell the engine that it has to step somewhere kind of like a Physics system

User
```
	while (m_Framework->TranslateUserInputs())
	{
		/*
		 * a bunch of code here
		 */

		m_Framework->Step();
	}
```

Engne Query user input and start the clock

```
    bool SLD::Core::TranslateUserInputs()
    {   
	    m_WorldClock.StartTime();

	    bool isQuit{};
	
	    m_MainViewPort->PollWindowMessages(isQuit);

	    return !isQuit;
    }
```

Step part

```
void SLD::Core::Step()
{
	const float dt{ m_WorldClock.GetDeltaTime() };
	
	// *** UPDATE WORLD ***
	m_WorldEntity.OnStartFrame();

	m_WorldEntity.OnInputValidation(m_MainViewPort);
	
	m_WorldEntity.OnPreAsyncUpdate(dt);

	m_WorldEntity.OnPhysicsValidation(m_WorldClock.GetFixedDeltaTime());
	
	m_WorldEntity.OnPreRenderUpdate(dt);
	
	// *** Render ***
	// Point beyond data manipulation

	m_WorldEntity.OnValidation(); // Asyncally validate the delete or added GameObject
	
	m_MainViewPort->ClearBackBuffer();

	m_WorldEntity.OnRenderSystem(m_MainViewPort);

	m_WorldEntity.SwapCopyBufferToReadBuffer();
	
	m_WorldEntity.CleanArchetypeMarked();

	m_MainViewPort->Present();

	m_WorldClock.EndTime();
}
```