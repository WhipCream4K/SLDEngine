
#ifndef SLDFRAMEWORK_BASECOMPONENT_H
#define SLDFRAMEWORK_BASECOMPONENT_H

#include "../Core/Base.h"
#include "../Core/ObservePtr.h"
#include "../Core/IdGenerator/UniqueIdGenerator.h"

namespace SLD
{
	class WorldEntity;
	using ComponentTypeId = size_t;
	using GameObjectId = size_t;
	class GameObject;
	class BaseComponent
	{
		friend class GameObject;
	
	public:
		
		virtual ~BaseComponent() = default;

		/// <summary>
		/// Calls Destructor of derived component
		/// NOTE: doesn't free any memory
		/// </summary>
		/// <param name="address"></param>
		virtual void DestroyData(uint8_t* address) = 0;
		virtual void MoveData(uint8_t* source, uint8_t* destination) const = 0;
		[[nodiscard]] virtual size_t GetSize() const = 0;

		
	};

	template<typename T>
	class ComponentT : public BaseComponent
	{
	public:

		static ComponentTypeId GetId();
		virtual void DestroyData(uint8_t* address) override;
		virtual void MoveData(uint8_t* source, uint8_t* destination) const override;
		[[nodiscard]] virtual size_t GetSize() const override;
		virtual ~ComponentT() override = default;
	};

	template <typename T>
	void ComponentT<T>::DestroyData(uint8_t* address)
	{
		T* object{ reinterpret_cast<T*>(address) };

		object->~T();
	}

	template <typename T>
	void ComponentT<T>::MoveData(uint8_t* source, uint8_t* destination) const
	{
		new (destination) T{ std::move(*reinterpret_cast<T*>(source)) };
	}

	template <typename T>
	size_t ComponentT<T>::GetSize() const
	{
		return sizeof(T);
	}

	template <typename T>
	ComponentTypeId ComponentT<T>::GetId()
	{
		return UniqueIdGenerator<BaseComponent>::GetNewId<T>();
	}
}



#endif
