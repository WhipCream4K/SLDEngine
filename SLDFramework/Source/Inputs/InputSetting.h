
#ifndef SLDFRAMEWORK_INPUTSETTING_H
#define SLDFRAMEWORK_INPUTSETTING_H

#include "../Core/Base.h"
#include "../Core/PMR_Resource/LoggingResource.h"
#include "../Core/PMR_Resource/UnsafeSpatialResource.h"
#include "../Core/PMR_Resource/PoolResource.h"
#include "../Core/ScriptableChecks.h"

#include "InputCommandType.h"
#include "InputTypes.h"

#include <unordered_set>

namespace SLD
{
	// TODO: Deal with input again later when we have GameObject class
	struct ActionMap
	{
		using KeyPoolType = std::unordered_set<Key, KeyHasher>;

		ActionMap(std::vector<ActionKey> keyVec, KeyPoolType pool)
			: keys(std::move(keyVec))
			, keyPool(std::move(pool))
			, pmrResource()
			, logger(&pmrResource)
			, commandTable()
			, mapTriggeredThisFrame()
		{
		}

		std::vector<ActionKey> keys;
		KeyPoolType keyPool;
		UnsafeSpatialResource<64> pmrResource;
		LoggingResource logger;
		std::vector<ActionCommandType> commandTable;
		bool mapTriggeredThisFrame;
	};

	struct AxisMap
	{
		using KeyPoolType = std::unordered_map<Key, float, KeyHasher>;

		AxisMap(std::vector<AxisKey> keyVec, KeyPoolType pool)
			: keys(std::move(keyVec))
			, keyPool(std::move(pool))
			, logger(&pmrResource)
			, mapTotalAxisValueThisFrame()
		{
		}

		std::vector<AxisKey> keys;
		KeyPoolType keyPool;
		PoolResource<64> pmrResource{};
		LoggingResource logger;
		std::vector<AxisCommandType> commandTable;
		float mapTotalAxisValueThisFrame;
	};

	class InputSetting
	{
	public:

		[[nodiscard]] const std::vector<ActionKey>& GetActionKeyByName(const std::string& groupName) const;
		[[nodiscard]] const std::vector<AxisKey>& GetAxisKeyByName(const std::string& groupName) const;

		void AddActionMapping(const std::string& groupName, std::initializer_list<ActionKey> keys);
		void AddAxisMapping(const std::string& groupName, std::initializer_list<AxisKey> keys);

		void AddActionKeyToMapping(const std::string& actionMappingName, const ActionKey& key, bool rebuildKeyPool = true);
		void AddAxisKeyToMapping(const std::string& axisMappingName, const AxisKey& key, bool rebuildKeyPool = true);

		void AddActionKeysToMapping(const std::string& actionMappingName, const std::vector<ActionKey>& keyVec, bool rebuildKeyPool = true);
		void AddAxisKeysToMapping(const std::string& axisMappingName, const std::vector<AxisKey>& keyVec, bool rebuildKeyPool = true);

		[[nodiscard]] const std::unordered_map<std::string, ActionMap>& GetAllActionMappings() const { return m_ActionKeyMappings; }
		[[nodiscard]] const std::unordered_map<std::string, AxisMap>& GetAllAxisMappings() const { return m_AxisKeyMappings; }

		void ParseMessage(const MessageBus& message) const;

		template<typename FuncPtr,
			typename Object = typename Function_Traits<FuncPtr>::element_type,
			typename ...Args>
			void RegisterActionCommand(
				const std::string& groupName,
				InputEvent ie,
				FuncPtr fnPtr,
				const RefPtr<Object>& objInstance,
				bool** outHandle,
				Args&&... args);

		template<typename FuncPtr,
			typename Object = typename Function_Traits<FuncPtr>::element_type>
			void RegisterAxisCommand(
				const std::string& groupName,
				FuncPtr fnPtr,
				const RefPtr<Object>& objInstance,
				float** outHandle);

	private:

		std::unordered_map<std::string, ActionMap> m_ActionKeyMappings;
		std::unordered_map<std::string, AxisMap> m_AxisKeyMappings;
	};

	template <typename FuncPtr, typename Object, typename ... Args>
	void InputSetting::RegisterActionCommand(
		const std::string& groupName,
		InputEvent ie,
		FuncPtr fnPtr,
		const RefPtr<Object>& objInstance,
		bool** outHandle,
		Args&&... args)
	{
		using EventType = Event<FuncPtr, Object>;
		using CallbackType = EventHandler;

		auto& actionMap{ m_ActionKeyMappings.at(groupName) };

		// give out isMapTriggeredThisFrame to out handle if anyone needs it
		if (outHandle)
			*outHandle = &actionMap.mapTriggeredThisFrame;

		// construct Event in logging resource in place
		constexpr size_t commandSize{ sizeof(EventType) };
		auto& loggerResource{ actionMap.logger };
		void* emplacePtr{ loggerResource.do_allocate(commandSize,alignof(EventType)) };
		CallbackType* ev{ new (emplacePtr) EventType{fnPtr,objInstance,std::forward<Args>(args)...} };

		// This smart pointer does nothing because our ptr is already on the resources
		RefPtr<CallbackType> smartEv{ ev,[&loggerResource](CallbackType* ptr)
		{
			loggerResource.do_deallocate(ptr,sizeof(EventType),alignof(EventType));
			ptr = nullptr;
		} };

		// Bring it to the command vector
		actionMap.commandTable.emplace_back(ActionCommandType{ smartEv,ie });
	}

	template <typename FuncPtr, typename Object>
	void InputSetting::RegisterAxisCommand(
		const std::string& groupName,
		FuncPtr fnPtr,
		const RefPtr<Object>& objInstance,
		float** outHandle)
	{
		using EventType = CAction<FuncPtr, Object>;
		using CallbackType = VirMemDelegate<AxisCallbackType>;

		auto& axisMap{ m_AxisKeyMappings.at(groupName) };

		// give out mapTotalAxisValueThisFrame to out handle if anyone needs it
		if (outHandle)
			*outHandle = &axisMap.mapTotalAxisValueThisFrame;

		// construct Event in logging resource in place
		constexpr size_t commandSize{ sizeof(EventType) };
		auto& resource{ axisMap.logger };
		void* emplacePtr{ resource.do_allocate(commandSize,alignof(EventType)) };
		CallbackType* ev{ new (emplacePtr) EventType{fnPtr,objInstance} };

		// This smart pointer does nothing because our ptr is already on the resources
		RefPtr<CallbackType> smartEv{ ev,[&resource](CallbackType* ptr)
		{
			resource.do_deallocate(ptr,sizeof(EventType),alignof(EventType));
			ptr = nullptr;
		} };

		// Bring it to the command vector
		axisMap.commandTable.emplace_back(smartEv);
	}
}

#endif
