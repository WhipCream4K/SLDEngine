#include "InputComponent.h"

#include "../Core/WorldEntity.h"

//SLD::InputActionKeyBinding::InputActionKeyBinding(std::pmr::memory_resource* callbackResource)
//	: m_Callbacks(callbackResource)
//{
//}
//
//void SLD::InputActionKeyBinding::ShouldExecute()
//{
//	// 1. Input layer got update first
//	// 2. Update layer got update later
//
//	// Input layer
//	for (const auto& cb : m_Callbacks)
//	{
//		cb->Invoke();
//	}
//
//	// Update layer
//	m_Handle.isPressed = true;
//}
//
//SLD::InputAxisKeyBinding::InputAxisKeyBinding(std::pmr::memory_resource* callbackResource)
//	: m_Callbacks(callbackResource)
//{
//}
//
//void SLD::InputAxisKeyBinding::Update(float axisValue)
//{
//	// 1. Input layer got update first
//	// 2. Update layer got update later
//
//	// Input layer
//	for (const auto& cb : m_Callbacks)
//	{
//		cb->Invoke(axisValue);
//	}
//
//	// Update layer
//	m_Handle.resultAxis = axisValue;
//}
//
//
//SLD::InputComponent::InputComponent(InputSetting& InputSetting)
//	: m_InputSettingRef(InputSetting)
//{
//}
//
//void SLD::InputComponent::OnCreate(const RefPtr<InputManager>& manager)
//{
//}
//
//void SLD::InputComponent::OnCreate(const InputManager& manager)
//{
//	// Bind to manager inputcomponent resource
//}
//
//std::optional<SLD::ActionHandle const*> SLD::InputComponent::GetActionHandleFromMappingGroup(
//	const std::string& actionName,
//	InputEvent ie) noexcept
//{
//	return GetActionHandleFromMappingGroup(InputManager::CreateHashId(actionName, ie));
//}
//
//std::optional<SLD::AxisHandle const*> SLD::InputComponent::GetAxisHandleFromMappingGroup(const std::string& axisName) noexcept
//{
//	return GetAxisHandleFromMappingGroup(InputManager::CreateHashId(axisName, InputEvent::IE_None));
//}
//
//std::optional<const SLD::ActionHandle*> SLD::InputComponent::GetActionHandleFromMappingGroup(uint32_t keyId) noexcept
//{
//	//const auto fIt = m_ActionHandleGroup.find(keyId);
//
//	//if (fIt != m_ActionHandleGroup.end())
//	//	return &fIt->second.GetRefHandle();
//
//	//return std::nullopt;
//}
//
//std::optional<const SLD::AxisHandle*> SLD::InputComponent::GetAxisHandleFromMappingGroup(uint32_t keyId) noexcept
//{
//	//const auto fIt = m_AxisHandleGroup.find(keyId);
//	//if (fIt != m_AxisHandleGroup.end())
//	//	return &fIt->second.GetRefHandle();
//	//return std::nullopt;
//}
//
//SLD::InputActionKeyBinding& SLD::InputComponent::GetActionKeyManager(uint32_t keyId)
//{
//	//return m_ActionHandleGroup.at(keyId);
//}
//
//SLD::InputAxisKeyBinding& SLD::InputComponent::GetAxisKeyManager(uint32_t keyId)
//{
//	//return m_AxisHandleGroup.at(keyId);
//}

//SLD::InputComponent::InputComponent(const RefPtr<WorldEntity>& world)
//	: NonTickComponent(world)
//	, m_InputSettingRef(const_cast<InputSetting&>(world->GetWorldInputSetting()))
//{
//}
SLD::InputComponent::InputComponent(InputSetting& inputRef)
	: NonTickComponent()
	, m_InputSettingRef(inputRef)
{
}
