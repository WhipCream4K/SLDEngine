

#ifndef SLDFRAMEWORK_INPUTCOMPONENT_H
#define SLDFRAMEWORK_INPUTCOMPONENT_H

#include "../Core/WorldEntity.h"
#include "../Inputs/InputSettings.h"
#include "NonTickComponent.h"
#include "../Inputs/InputParams.h"

namespace SLD
{
	struct ActionHandle
	{
		bool isValid{};
	};

	class AxisHandle
	{
		float axisValue{};
	};

	struct InputCallbackHandle
	{
		float axisValue{};
		bool isTriggered{};
	};
	
	//class InputActionKeyBinding
	//{
	//public:

	//	InputActionKeyBinding(std::pmr::memory_resource* callbackResource);
	//	
	//	[[nodiscard]] const ActionHandle& GetRefHandle() const { return m_Handle; }

	//	void ShouldExecute();

	//	template<typename FuncPtr,
	//		typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
	//		typename ...Args>
	//		constexpr auto RegisterCallback(
	//			FuncPtr fnPtr,
	//			const SharedPtr<ObjectType>& instance,
	//			Args&&... args) noexcept(false) -> void;

	//private:

	//	// some vector of delegate
	//	std::pmr::vector<SharedPtr<EventHandler>> m_Callbacks;
	//	std::string m_ActionName;
	//	ActionHandle m_Handle;
	//};

	//class InputAxisKeyBinding
	//{
	//public:

	//	using binding_type = void(float);
	//	
	//	InputAxisKeyBinding(std::pmr::memory_resource* callbackResource);

	//	[[nodiscard]] const AxisHandle& GetRefHandle() const { return m_Handle; }

	//	// send axis value to every callbacks we have
	//	void Update(float axisValue);

	//	template<typename FuncPtr,
	//		typename ObjectType = typename Function_Traits<FuncPtr>::element_type>
	//		constexpr auto RegisterCallback(FuncPtr fnPtr, const SharedPtr<ObjectType>& instance) noexcept(false) -> void;

	//private:

	//	std::pmr::vector<SharedPtr<DynamicDelegate<binding_type>>> m_Callbacks;
	//	AxisHandle m_Handle;
	//};


	class InputComponent final : public NonTickComponent
	{
	public:

		friend class InputSystem;
		
		static constexpr const char* UniqueId{ "InputComponent" };
		
		//InputComponent(WeakPtr<GameObject> gameObject);
		//InputComponent(const SharedPtr<GameObject>& gameObject);
		
		InputComponent(WorldEntity& world, GameObjectId id);

		const ActionHandle& GetActionHandleFromActionGroup(
			const std::string& groupName,
			InputEvent eventType) const;
		
		const std::array<ActionHandle,
		InputParams::InputEventCount>& GetActionHandlesFromActionGroup(const std::string& groupName) const;
		
		const AxisHandle& GetAxisHandleFromAxisGroup(const std::string& groupName) const;

		void SetReceiveIndex(size_t idx);
		size_t GetReceiveIndex() const; 

		template<typename FuncPtr,typename ...Args>
		void BindAction(
			const std::string& groupName,
			FuncPtr&& callback,
			Args&&... message);

		template<typename FuncPtr, typename ...Args>
		void BindAxis(
			const std::string& groupName,
			FuncPtr&& callback,
			Args&&... message);
		
		//template<typename FuncPtr,
		//	typename = EnableIf<IsMemberFunc<FuncPtr>>,
		//	typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
		//	typename ...Args>
		//	[[maybe_unused]] constexpr auto BindAction(
		//		const std::string& actionName,
		//		InputEvent ie,
		//		FuncPtr fnPtr,
		//		const SharedPtr<ObjectType>& instance,
		//		Args&&... args) noexcept(false) -> ActionHandle;
		//
		//template<typename FuncPtr,
		//	typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
		//	typename = EnableIf<IsFuncSameType<FuncPtr, AxisCallbackType>::value>>
		//	[[maybe_unused]] constexpr auto BindAxis(
		//		const std::string& axisName,
		//		FuncPtr fnPtr,
		//		const SharedPtr<ObjectType>& instance) noexcept -> AxisHandle;
		
		//template<typename FuncPtr,
		//	typename = EnableIf<IsMemberFunc<FuncPtr>>,
		//	typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
		//	typename ...Args>
		//	[[maybe_unused]] constexpr auto BindAction(
		//		const std::string& actionName,
		//		InputEvent ie,
		//		FuncPtr fnPtr,
		//		ObjectType* instance,
		//		Args&&... args) noexcept ->ActionHandle;

		//template<typename FuncPtr,
		//	typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
		//	typename = EnableIf<IsFuncSameType<FuncPtr, AxisCallbackType>::value>>
		//	[[maybe_unused]] constexpr auto BindAxis(
		//		const std::string& axisName,
		//		FuncPtr fnPtr,
		//		ObjectType* instance) noexcept -> AxisHandle;

		using KeyToActionHandleMap = std::unordered_map<std::string, std::array<ActionHandle, InputParams::InputEventCount>>;
		using KeyToAxisHandleMap = std::unordered_map<std::string, AxisHandle>;
	
	private:

	
		KeyToActionHandleMap& GetActionHandleMap();
		KeyToAxisHandleMap& GetAxisHandleMap();

		WorldEntity& m_WorldRef;
		GameObjectId m_Parent;
		KeyToActionHandleMap m_ActionHandleMap;
		KeyToAxisHandleMap m_AxisHandleMap;
		size_t m_ReceiveIndex{};
		
		// InputComponent is the only component that hardly tied to the owner (GameObject)
		// Because in order to destroy commands create by this InputComponent
		// We need some sort of reference point to delete from the table

		std::vector<SharedPtr<EventHandler>> m_ActionCommand;
		std::vector<SharedPtr<EventHandler>> m_AxisCommand;
		
		//using CompActionCommand = SharedPtr<EventHandler>;
		//using CompAxisCommand = SharedPtr<DynamicDelegate<AxisCallbackType>>;
		
		//std::vector<CompActionCommand> m_ActionCommand;
		//std::vector<CompAxisCommand> m_AxisCommand;
	};

	template <typename FuncPtr, typename ... Args>
	void InputComponent::BindAction(const std::string& groupName, FuncPtr&& callback, Args&&... message)
	{
		m_ActionCommand.emplace_back(std::make_shared<Event<FuncPtr>>(callback,m_WorldRef,m_Parent,std::forward<Args>(message)...));
	}

	template <typename FuncPtr, typename ... Args>
	void InputComponent::BindAxis(const std::string& groupName, FuncPtr&& callback, Args&&... message)
	{
		
	}

	//template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	//constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent ie, FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance, Args&&... args) noexcept(false) -> ActionHandle
	//{
	//	ActionHandle handle{};

	//	if (auto parentPointer{ m_Parent.lock() }; parentPointer)
	//		parentPointer->GetWorld().GetWorldInputSetting().CreateActionCommand(
	//			actionName,
	//			m_Parent,
	//			ie,
	//			fnPtr,
	//			instance.get(),
	//			&handle.m_IsValid,
	//			std::forward<Args>(args)...
	//		);

	//	return handle;
	//}

	//template <typename FuncPtr, typename ObjectType, typename>
	//constexpr auto InputComponent::BindAxis(
	//	const std::string& axisName,
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance) noexcept -> AxisHandle
	//{
	//	AxisHandle handle{};
	//	if (auto parentPtr{ m_Parent.lock() }; parentPtr)
	//		parentPtr->GetWorld().GetWorldInputSetting().CreateAxisCommand(
	//			axisName,
	//			m_Parent,
	//			fnPtr,
	//			instance.get(),
	//			&handle.m_ResultAxis
	//		);

	//	return handle;
	//}

	//template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	//constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent ie, FuncPtr fnPtr,
	//	ObjectType* instance, Args&&... args) noexcept -> ActionHandle
	//{
	//	ActionHandle handle{};

	//	if (auto parentPointer{ m_Parent.lock() }; parentPointer)
	//		parentPointer->GetWorld().GetWorldInputSetting().CreateActionCommand(
	//			actionName,
	//			m_Parent,
	//			ie,
	//			fnPtr,
	//			instance,
	//			&handle.m_IsValid,
	//			std::forward<Args>(args)...
	//		);
	//	
	//	return handle;
	//}

	//template <typename FuncPtr, typename ObjectType, typename>
	//constexpr auto InputComponent::BindAxis(const std::string& axisName, FuncPtr fnPtr,
	//	ObjectType* instance) noexcept -> AxisHandle
	//{
	//	AxisHandle handle{};
	//	if (auto parentPtr{ m_Parent.lock() }; parentPtr)
	//		parentPtr->GetWorld().GetWorldInputSetting().CreateAxisCommand(
	//			axisName,
	//			m_Parent,
	//			fnPtr,
	//			instance,
	//			&handle.m_ResultAxis
	//	);

	//	return handle;
	//}

	//template <typename FuncPtr, typename ObjectType, typename ... Args>
	//constexpr auto InputActionKeyBinding::RegisterCallback(
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance,
	//	Args&&... args) noexcept(false) -> void
	//{
	//	SharedPtr<EventHandler> acEvent{ std::make_shared<Event<FuncPtr>>(fnPtr,instance,std::forward<Args>(args)...) };
	//	m_Callbacks.emplace_back(acEvent);
	//}

	//template <typename FuncPtr, typename ObjectType>
	//constexpr auto InputAxisKeyBinding::RegisterCallback(
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance) noexcept(false) -> void
	//{
	//	SharedPtr<DynamicDelegate<AxisHandle::Signature>> axAction{ std::make_shared<CAction<AxisHandle::Signature,ObjectType>>(fnPtr,instance) };
	//	m_Callbacks.emplace_back(axAction);
	//}

	//template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	//constexpr auto InputComponent::BindAction(
	//	const std::string& actionName,
	//	InputEvent ie,
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance,
	//	Args&&... args) noexcept(false) -> const ActionHandle&
	//{
	//	const uint32_t keyId{ InputManager::CreateHashId(actionName,ie) };
	//	InputActionKeyBinding& tryManager = m_ActionHandleGroup[keyId];
	//	tryManager.RegisterCallback(fnPtr, instance, std::forward<Args>(args)...);
	//	return tryManager.GetRefHandle();
	//}

	//template <typename FuncPtr, typename ObjectType, typename>
	//constexpr auto InputComponent::BindAxis(
	//	const std::string& axisName,
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectType>& instance) noexcept -> const AxisHandle&
	//{
	//	const uint32_t keyId{ InputManager::CreateHashId(axisName,InputEvent::IE_None) };
	//	InputAxisKeyBinding& tryManager{ m_AxisHandleGroup[keyId] };
	//	tryManager.RegisterCallback(fnPtr, instance);
	//	return tryManager.GetRefHandle();
	//}

	//template <typename FuncPtr, typename, typename ObjectTpye>
	//constexpr auto InputComponent::BindWindowEvent(
	//	uint8_t eventType,
	//	FuncPtr fnPtr,
	//	const SharedPtr<ObjectTpye>& instance) noexcept -> void
	//{
	//	
	//}
}

#endif
