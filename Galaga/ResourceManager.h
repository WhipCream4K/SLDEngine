#pragma once

#include <Core/Singleton.h>

class ResourceManager : public SLD::Singleton<ResourceManager>
{
public:

	friend class Singleton<ResourceManager>;

	template<typename T>
	T* Add(const std::string& name,T&& item);

	template<typename T>
	T* Get(const std::string& name);

private:

	std::unordered_map<std::string, SharedPtr<uint8_t[]>> m_ResourceMap;
};

template <typename T>
T* ResourceManager::Add(const std::string& name, T&& item)
{
	auto& ref{ m_ResourceMap[name] };
	ref = SharedPtr<uint8_t[]>{ new uint8_t[sizeof(T)]{} };

	new (ref.get()) T{ std::forward<T>(item) };

	return reinterpret_cast<T*>(ref.get());
}

template <typename T>
T* ResourceManager::Get(const std::string& name)
{
	return reinterpret_cast<T*>(m_ResourceMap.at(name).get());
}

