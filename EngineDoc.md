After, my last take on the game engine I wasn’t really thinking of programming patterns at all and focus on something else that doesn’t fit the whole topic. 
    
I finally have a chance to rethink and do the thing I was interested and suppose to do in the first time.


## Lite ECS
This is my take on ECS (Entity Component System). I’ll try to explain what I’ve done for this game engine

# GameObject
Speaking of ECS GameObject doesn’t really hold any state or variables. It only consists of a reference to world and unique Id to tell the world to point to the correct GameObject in the system although I put some functionality for ease of use and of course it’s final.

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

 