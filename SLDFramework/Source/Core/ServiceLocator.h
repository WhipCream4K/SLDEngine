
#ifndef SLDFRAMEWORK_WORLD_H
#define SLDFRAMEWORK_WORLD_H

#include "Singleton.h"
#include <unordered_set>
#include <variant>


namespace SLD
{
	class InputManager;
	class ServiceLocator final : public Singleton<ServiceLocator>
	{
	public:
		
		using service_type = std::variant<SharedPtr<InputManager>>;

		void RegisterService(const SharedPtr<service_type>& sv);
		
		template<typename T>
		SharedPtr<std::remove_all_extents_t<T>> GetService();
	
	private:
	};

	template <typename T>
	SharedPtr<std::remove_all_extents_t<T>> ServiceLocator::GetService()
	{
		return nullptr;
	}
}

#endif




