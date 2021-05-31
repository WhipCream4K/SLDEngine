
#ifndef SLDFRAMEWORK_SPATIALRESOURCE_H
#define SLDFRAMEWORK_SPATIALRESOURCE_H

#include "../Base.h"

namespace SLD
{
	class SpaialResource
	{
	public:

		SpaialResource(size_t bufferSize);

		template<typename T, typename ...Args>
		RefPtr<ObservePtr<T>> DoAllocate(Args&&... args);

		void DoDeAllocate(void* ptr, size_t size);
		uint8_t* const & GetHead() const noexcept;

	private:

		template<typename T,typename ...Args>
		RefPtr<ObservePtr<T>> Alloc(Args&&... args);
		
		void IncreaseBuffer(size_t newSize);
	
		uint8_t* m_pHead;
		uint8_t* m_pCurrent;
		std::vector<uint8_t> m_Buffer;
		std::list<ObservePtr<std::nullptr_t>> m_Observer;

	};

	template <typename T, typename ... Args>
	RefPtr<ObservePtr<T>> SpaialResource::DoAllocate(Args&&... args)
	{
		constexpr size_t TSize{ sizeof(T) };
		const size_t sizeFromHead{ m_Buffer.size() };

		if (sizeFromHead + TSize < m_Buffer.capacity())
			return Alloc<T>(std::forward<Args>(args)...);

		IncreaseBuffer(sizeFromHead + TSize);

		return Alloc<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename ... Args>
	RefPtr<ObservePtr<T>> SpaialResource::Alloc(Args&&... args)
	{
		const size_t sizeFromHead{ m_Buffer.size() };
		new (m_pCurrent) T{ std::forward<Args>(args)... };
		auto& ob{ m_Observer.emplace_back(m_pHead,sizeFromHead) };
		auto out{ reinterpret_cast<ObservePtr<T>&>(ob) };
		m_pCurrent += sizeof(T);
		return RefPtr<ObservePtr<T>>{&out, [this](ObservePtr<T>* ptr)
			{
				DoDeAllocate(ptr->GetPtrPointTo(), sizeof(T));
				ptr = nullptr;
		}};
	}
}

#endif
