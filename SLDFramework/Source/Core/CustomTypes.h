
#ifndef SLDFRAMEWORK_CUSTOMTYPES_H
#define SLDFRAMEWORK_CUSTOMTYPES_H

#include <memory>
#include <type_traits>

template<typename UserClass>
using SharedPtr = std::shared_ptr<UserClass>;

template<typename UserClass>
using WeakPtr = std::weak_ptr<UserClass>;

template<typename UserClass, typename Deleter = std::default_delete<UserClass>>
using OwnedPtr = std::unique_ptr<UserClass, Deleter>;

template<typename T>
using AlignedStorageT = std::aligned_storage_t<sizeof(T), alignof(T)>;

template<bool val, typename ret = void>
using EnableIf = std::enable_if_t<val,ret>;

template<typename B, typename D,typename ret = void>
using EnableIsBasedOf = std::enable_if_t<std::is_base_of_v<B, D>, ret>;

template<typename T>
using RemoveAllExt = std::remove_all_extents_t<T>;

template<typename T>
using PointTo = std::add_pointer_t<T>;

template<typename T>
using RefWrap = std::reference_wrapper<T>;

template<typename T>
using AddressOfPtr = T**;

template<typename T>
struct No_Op
{
	void operator()(T*) const {}
};


template<typename T>
struct MemHandle
{
	T* pointToHead;
	size_t maxSize;
};

#endif
