
#ifndef SLDFRAMEWORK_SPATIALRESOURCE_H
#define SLDFRAMEWORK_SPATIALRESOURCE_H

#include "../Base.h"

namespace SLD
{
	class UnSynchronizedSpatialResource : public std::pmr::memory_resource
	{
	public:

		UnSynchronizedSpatialResource(size_t bufferSize);

		void* do_allocate(size_t _Bytes, size_t _Align) override;
		void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		bool do_is_equal(const memory_resource& _That) const noexcept override;

		uint8_t* const & GetHead() const noexcept;
		size_t GetUsedSize() const noexcept;

	private:

		void* Alloc(size_t bytes, size_t alignment);
	
		
		void IncreaseBuffer(size_t newSize);
		void MoveObserver(uint8_t* oldHeadPointer);
	
		uint8_t* m_pHead;
		uint8_t* m_pCurrent;
		std::vector<uint8_t> m_Buffer;
		std::list<uint8_t*> m_Observer;
	};

	//template <typename T, typename ... Args>
	//SharedPtr<ObservePtr<T>> UnSynchronizedSpatialResource::DoAllocate(Args&&... args)
	//{
	//	constexpr size_t TSize{ sizeof(T) };
	//	const size_t sizeFromHead{ m_Buffer.size() };

	//	if (sizeFromHead + TSize < m_Buffer.capacity())
	//		return Alloc<T>(std::forward<Args>(args)...);

	//	IncreaseBuffer(sizeFromHead + TSize);

	//	return Alloc<T>(std::forward<Args>(args)...);
	//}

	//template <typename T, typename ... Args>
	//SharedPtr<ObservePtr<T>> UnSynchronizedSpatialResource::Alloc(Args&&... args)
	//{
	//	const size_t sizeFromHead{ m_Buffer.size() };
	//	new (m_pCurrent) T{ std::forward<Args>(args)... };
	//	auto& ob{ m_Observer.emplace_back(m_pHead,sizeFromHead) };
	//	auto out{ reinterpret_cast<ObservePtr<T>&>(ob) };
	//	m_pCurrent += sizeof(T);
	//	return SharedPtr<ObservePtr<T>>{&out, [this](ObservePtr<T>* ptr)
	//		{
	//			DoDeAllocate(ptr->GetPtrPointTo(), sizeof(T));
	//			ptr = nullptr;
	//	}};
	//}
}

#endif
