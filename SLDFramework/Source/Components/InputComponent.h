

#ifndef SLDFRAMEWORK_INPUTCOMPONENT_H
#define SLDFRAMEWORK_INPUTCOMPONENT_H

#include "../Core/ScriptableChecks.h"
#include "../Core/Event.h"
#include "../Inputs/InputSetting.h"
#include "NonTickComponent.h"



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
		
		InputComponent(InputSetting& inputRef);

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

		//template<typename FuncPtr,
		//	typename = EnableIf<IsMemberFunc<FuncPtr>>,
		//	typename ObjectTpye = typename Function_Traits<FuncPtr>::element_type>
		//	constexpr auto BindWindowEvent(uint8_t eventType, FuncPtr fnPtr, const RefPtr<ObjectTpye>& instance) noexcept -> void;

		//template<typename FuncPtr,
		//	typename = EnableIf<IsMemberFunc<FuncPtr>>,
		//	typename ObjectTpye = typename Function_Traits<FuncPtr>::element_type>
		//	constexpr auto BindWindowEvent(sf::Event::EventType eventType, FuncPtr fnPtr, const RefPtr<ObjectTpye>& instance) noexcept -> void;

		//[[nodiscard]] std::optional<ActionHandle const*> GetActionHandleFromMappingGroup(const std::string& actionName, InputEvent ie) noexcept;
		//[[nodiscard]] std::optional<AxisHandle const*> GetAxisHandleFromMappingGroup(const std::string& axisName) noexcept;
		//[[nodiscard]] std::optional<ActionHandle const*> GetActionHandleFromMappingGroup(uint32_t keyId) noexcept;
		//[[nodiscard]] std::optional<AxisHandle const*> GetAxisHandleFromMappingGroup(uint32_t keyId) noexcept;

		//[[nodiscard]] InputActionKeyBinding& GetActionKeyManager(uint32_t keyId);
		//[[nodiscard]] InputAxisKeyBinding& GetAxisKeyManager(uint32_t keyId);
	
	private:

		// NOTE: resource for all the bindings

		InputSetting& m_InputSettingRef;
	};

	template <typename FuncPtr, typename, typename ObjectType, typename ... Args>
	constexpr auto InputComponent::BindAction(
		const std::string& actionName,
		InputEvent ie,
		FuncPtr fnPtr,
		const RefPtr<ObjectType>& instance,
		Args&&... args) noexcept(false) -> ActionHandle
	{
		ActionHandle handle{};
		m_InputSettingRef.RegisterActionCommand(actionName, ie, fnPtr, instance, &handle.m_IsValid, std::forward<Args>(args)...);
		return handle;
	}

	template <typename FuncPtr, typename ObjectType, typename>
	constexpr auto InputComponent::BindAxis(
		const std::string& axisName,
		FuncPtr fnPtr,
		const RefPtr<ObjectType>& instance) noexcept -> AxisHandle
	{
		AxisHandle handle{};
		m_InputSettingRef.RegisterAxisCommand(axisName, fnPtr, instance, &handle.m_ResultAxis);
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
