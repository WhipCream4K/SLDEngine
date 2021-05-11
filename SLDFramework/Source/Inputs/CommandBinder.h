
#ifndef SLDFRAMEWORK_COMMANDBINDING_H
#define SLDFRAMEWORK_COMMANDBINDING_H

#include "../Core/LoggingResource.h"
#include "../Core/Base.h"
#include "Key.h"

#include <variant>

namespace SLD
{
	template<typename ...Types>
	struct BindingCommand
	{
		std::vector<std::variant<Types...>> commands{};
		LoggingResource bindingResource{};
	};

	template<typename ...ComTypes>
	class CommandBinder
	{
	public:

		using BlockId = uint8_t;

		CommandBinder();
		
		void InitializeCommandBlock(std::initializer_list<Key> keys);

		template<typename Itr>
		void InitializeCommandBlock(Itr begin, Itr end);

		// This function will try to allocate resource if key value is non existent
		[[nodiscard]] LoggingResource* GetBindingResourceFromKey(const Key& value);

		template<typename T>
		void RegisterCommandWithKey(const Key& value, T&& command);

		void ExecuteIfExist(const Key& value,float extVal) const;

	private:

		std::unordered_map<Key, BlockId, KeyHasher> m_KeyPool;
		std::vector<BindingCommand<ComTypes...>> m_CommandBlocks;
	};

	template <typename ... ComTypes>
	CommandBinder<ComTypes...>::CommandBinder()
		: m_CommandBlocks(size_t(2))
	{
	}

	template <typename ... ComTypes>
	void CommandBinder<ComTypes...>::InitializeCommandBlock(std::initializer_list<Key> keys)
	{
		m_CommandBlocks.emplace_back(BindingCommand<ComTypes...>{});
		BlockId id{ BlockId(m_CommandBlocks.size() - 1) };
		for (const auto& val : keys)
		{
			m_KeyPool.try_emplace(val, id);
		}
	}

	template <typename ... ComTypes>
	template <typename Itr>
	void CommandBinder<ComTypes...>::InitializeCommandBlock(Itr begin, Itr end)
	{
		m_CommandBlocks.emplace_back(BindingCommand<ComTypes...>{});
		BlockId id{ BlockId(m_CommandBlocks.size() - 1) };
		for (auto it = begin; it != end; ++it)
		{
			m_KeyPool.try_emplace(*it, id);
		}
	}

	template <typename ... ComTypes>
	LoggingResource* CommandBinder<ComTypes...>::GetBindingResourceFromKey(const Key& value)
	{
		// can throw
		const auto fIt = m_KeyPool.find(value);
		if (fIt != m_KeyPool.end())
		{
			BlockId& id{ fIt->second };
			return &m_CommandBlocks[id].resource;
		}


	}

	template <typename ... Types>
	template <typename T>
	void CommandBinder<Types...>::RegisterCommandWithKey(const Key& value, T&& command)
	{
		// 1. find the key value
		const auto fIt = m_KeyPool.find(value);
		if (fIt != m_KeyPool.end()) // found
		{
			// 2. check for existing command
			BlockId& id{ fIt->second };

			auto& comblock{ m_CommandBlocks[id].commands };

			auto commHead{ comblock.begin() };
			auto commTail{ comblock.end() };

			auto commandIt = std::find(commHead, commTail, std::forward<T>(command));
			if (commandIt == commTail) // not found
			{
				// 3. add it to the block
				comblock.emplace_back(std::forward<T>(command));
			}
		}
	}
}

#endif
