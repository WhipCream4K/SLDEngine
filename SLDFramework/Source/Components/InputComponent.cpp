#include "InputComponent.h"

#include "../Core/WorldEntity.h"
//#include "../GameObject/GameObject.h"

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
//SLD::InputComponent::InputComponent(InputSettings& InputSettings)
//	: m_InputSettingRef(InputSettings)
//{
//}
//
//void SLD::InputComponent::OnCreate(const SharedPtr<InputManager>& manager)
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

//SLD::InputComponent::InputComponent(const SharedPtr<WorldEntity>& world)
//	: NonTickComponent(world)
//	, m_InputSettingRef(const_cast<InputSettings&>(world->GetWorldInputSetting()))
//{
//}

//SLD::InputComponent::InputComponent(WeakPtr<GameObject> gameObject)
//	: m_Parent(gameObject)
//{
//}
//
//SLD::InputComponent::InputComponent(const SharedPtr<GameObject>& gameObject)
//	: m_Parent(gameObject)
//{
//}
//
//SLD::InputComponent::~InputComponent()
//{
//	if (auto parent{ m_Parent.lock() }; parent)
//	{
//		parent->GetWorld().GetWorldInputSetting().RemoveCommands(m_Parent);
//		std::cout << "Yp" << std::endl;
//	}
//}

SLD::InputComponent::InputComponent(WorldEntity& world, GameObjectId id)
	: m_WorldRef(world)
	, m_Parent(id)
{
}

const SLD::ActionHandle& SLD::InputComponent::GetActionHandleFromActionGroup(
	const std::string& groupName,
	InputEvent eventType) const
{
	return m_ActionHandleMap.at(groupName)[size_t(eventType)];
}

const std::array<SLD::ActionHandle,
	SLD::InputParams::InputEventCount>& SLD::InputComponent::GetActionHandlesFromActionGroup(
		const std::string& groupName) const
{
	return m_ActionHandleMap.at(groupName);
}

const SLD::AxisHandle& SLD::InputComponent::GetAxisHandleFromAxisGroup(const std::string& groupName) const
{
	return m_AxisHandleMap.at(groupName);
}

void SLD::InputComponent::SetReceiveIndex(size_t idx)
{
	m_ReceiveIndex = idx;
}

size_t SLD::InputComponent::GetReceiveIndex() const
{
	return m_ReceiveIndex;
}

SLD::InputComponent::KeyToActionHandleMap& SLD::InputComponent::GetActionHandleMap()
{
	return m_ActionHandleMap;
}

SLD::InputComponent::KeyToAxisHandleMap& SLD::InputComponent::GetAxisHandleMap()
{
	return m_AxisHandleMap;
}
