

#ifndef SLDFRAMEWORK_INPUTCOMPONENT_H
#define SLDFRAMEWORK_INPUTCOMPONENT_H

#include "../Core/ScriptableChecks.h"
#include "../Core/Event.h"
#include "../Inputs/InputSetting.h"
#include "NonTickComponent.h"
#include "../GameObject/GameObject.h"


namespace SLD
{
	class InputComponent;
	class ActionHandle
	{
	public:

		friend class InputComponent;
		bool GetKeyValueThisFrame() const { return *m_IsValid; }

	private:

		bool* m_IsValid{};
	};

	class AxisHandle
	{
	public:

		friend class InputComponent;
		float GetAxisValueThisFrame() const { return *m_ResultAxis; }
		
	private:
		
		float* m_ResultAxis{};
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
	//			const RefPtr<ObjectType>& instance,
	//			Args&&... args) noexcept(false) -> void;

	//private:

	//	// some vector of delegate
	//	std::pmr::vector<RefPtr<EventHandler>> m_Callbacks;
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
	//		constexpr auto RegisterCallback(FuncPtr fnPtr, const RefPtr<ObjectType>& instance) noexcept(false) -> void;

	//private:

	//	std::pmr::vector<RefPtr<VirMemDelegate<binding_type>>> m_Callbacks;
	//	AxisHandle m_Handle;
	//};

	class InputComponent final : public NonTickComponent
	{
	public:

		static constexpr const char* UniqueId{ "InputComponent" };
		
		InputComponent(WeakPtr<GameObject> gameObject);
		InputComponent(const RefPtr<GameObject>& gameObject);
		
		//InputComponent* GetMe();
		
		template<typename FuncPtr,
			typename = EnableIf<IsMemberFunc<FuncPtr>>,
			typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
			typename ...Args>
			[[maybe_unused]] constexpr auto BindAction(
				const std::string& actionName,
				InputEvent ie,
				FuncPtr fnPtr,
				const RefPtr<ObjectType>& instance,
				Args&&... args) noexcept(false) -> ActionHandle;
		
		template<typename FuncPtr,
			typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
			typename = EnableIf<IsFuncSameType<FuncPtr, AxisCallbackType>::value>>
			[[maybe_unused]] constexpr auto BindAxis(
				const std::string& axisName,
				FuncPtr fnPtr,
				const RefPtr<ObjectType>& instance) noexcept -> AxisHandle;

		template<typename FuncPtr,
			typename = EnableIf<IsMemberFunc<FuncPtr>>,
			typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
			typename ...Args>
			[[maybe_unused]] constexpr auto BindAction(
				const std::string& actionName,
				InputEvent ie,
				FuncPtr fnPtr,
				ObjectType* instance,
				Args&&... args) noexcept ->ActionHandle;

		template<typename FuncPtr,
			typename ObjectType = typename Function_Traits<FuncPtr>::element_type,
			typename = EnableIf<IsFuncSameType<FuncPtr, AxisCallbackType>::value>>
			[[maybe_unused]] constexpr auto BindAxis(
				const std::string& axisName,
				FuncPtr fnPtr,
				ObjectType* instance) noexcept -> AxisHandle;

		~InputComponent() override;
	
	private:

		// InputComponent is the only component that hardly tied to the owner (GameObject)
		// Because in order to destroy commands create by this InputComponent
		// We need some sort of reference point to delete from the table
		WeakPtr<GameObject> m_Parent;
	};

	template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent ie, FuncPtr fnPtr,
		const RefPtr<ObjectType>& instance, Args&&... args) noexcept(false) -> ActionHandle
	{
		ActionHandle handle{};

		if (auto parentPointer{ m_Parent.lock() }; parentPointer)
			parentPointer->GetWorld().get().GetWorldInputSetting().CreateActionCommand(
				actionName,
				m_Parent,
				ie,
				fnPtr,
				instance.get(),
				&handle.m_IsValid,
				std::forward<Args>(args)...
			);

		return handle;
	}

	template <typename FuncPtr, typename ObjectType, typename>
	constexpr auto InputComponent::BindAxis(
		const std::string& axisName,
		FuncPtr fnPtr,
		const RefPtr<ObjectType>& instance) noexcept -> AxisHandle
	{
		AxisHandle handle{};
		if (auto parentPtr{ m_Parent.lock() }; parentPtr)
			parentPtr->GetWorld().get().GetWorldInputSetting().CreateAxisCommand(
				axisName,
				m_Parent,
				fnPtr,
				instance.get(),
				&handle.m_ResultAxis
			);

		return handle;
	}

	template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent ie, FuncPtr fnPtr,
		ObjectType* instance, Args&&... args) noexcept -> ActionHandle
	{
		ActionHandle handle{};

		if (auto parentPointer{ m_Parent.lock() }; parentPointer)
			parentPointer->GetWorld().get().GetWorldInputSetting().CreateActionCommand(
				actionName,
				m_Parent,
				ie,
				fnPtr,
				instance,
				&handle.m_IsValid,
				std::forward<Args>(args)...
			);
		
		return handle;
	}

	template <typename FuncPtr, typename ObjectType, typename>
	constexpr auto InputComponent::BindAxis(const std::string& axisName, FuncPtr fnPtr,
		ObjectType* instance) noexcept -> AxisHandle
	{
		AxisHandle handle{};
		if (auto parentPtr{ m_Parent.lock() }; parentPtr)
			parentPtr->GetWorld().get().GetWorldInputSetting().CreateAxisCommand(
				axisName,
				m_Parent,
				fnPtr,
				instance,
				&handle.m_ResultAxis
		);

		return handle;
	}

	//template <typename FuncPtr, typename ObjectType, typename ... Args>
	//constexpr auto InputActionKeyBinding::RegisterCallback(
	//	FuncPtr fnPtr,
	//	const RefPtr<ObjectType>& instance,
	//	Args&&... args) noexcept(false) -> void
	//{
	//	RefPtr<EventHandler> acEvent{ std::make_shared<Event<FuncPtr>>(fnPtr,instance,std::forward<Args>(args)...) };
	//	m_Callbacks.emplace_back(acEvent);
	//}

	//template <typename FuncPtr, typename ObjectType>
	//constexpr auto InputAxisKeyBinding::RegisterCallback(
	//	FuncPtr fnPtr,
	//	const RefPtr<ObjectType>& instance) noexcept(false) -> void
	//{
	//	RefPtr<VirMemDelegate<AxisHandle::Signature>> axAction{ std::make_shared<CAction<AxisHandle::Signature,ObjectType>>(fnPtr,instance) };
	//	m_Callbacks.emplace_back(axAction);
	//}

	//template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	//constexpr auto InputComponent::BindAction(
	//	const std::string& actionName,
	//	InputEvent ie,
	//	FuncPtr fnPtr,
	//	const RefPtr<ObjectType>& instance,
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
	//	const RefPtr<ObjectType>& instance) noexcept -> const AxisHandle&
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
	//	const RefPtr<ObjectTpye>& instance) noexcept -> void
	//{
	//	
	//}
}

#endif
