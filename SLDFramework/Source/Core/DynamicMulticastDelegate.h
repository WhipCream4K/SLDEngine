
#ifndef SLDFRAMEWORK_MULTICASTDELEGATE_H
#define SLDFRAMEWORK_MULTICASTDELEGATE_H

//#include "Delegate.h"
#include "DynamicDelegate.h"
#include "ScriptableChecks.h"

#include <vector>

namespace SLD
{
	template<typename FnType>
	class DynamicMulticastDelegate;

	// TODO: Support const function

	template<typename Ret, typename ...Args>
	class DynamicMulticastDelegate<Ret(Args...)>
	{
	public:

		using FnSignature = Ret(Args...);

		void BroadCast(Args... args);

		template<
			typename FnSig,
			typename Object = typename Function_Traits<FnSig>::element_type,
			typename = EnableIf<std::is_same_v<FnSignature, typename Function_Traits<FnSig>::fn_type>>>
			size_t AddDynamic(FnSig fnPtr, Object* instance);

		template<
			typename FnSig,
			typename Object = typename Function_Traits<FnSig>::element_type,
			typename = EnableIf<std::is_same_v<FnSignature, typename Function_Traits<FnSig>::fn_type>>>
			size_t AddDynamic(FnSig fnPtr,const SharedPtr<Object>& instance);

		void UnBind(size_t idx);

		size_t AddDynamic(FnSignature fn);

	private:

		std::vector<SharedPtr<DynamicDelegate<FnSignature>>> m_Events;
	};

	template <typename Ret, typename ... Args>
	void DynamicMulticastDelegate<Ret(Args...)>::BroadCast(Args... args)
	{
		for (auto& del : m_Events)
		{
			if (del)
				del->Invoke(std::forward<Args>(args)...);
		}
	}

	template <typename Ret, typename ... Args>
	template <typename FnSig, typename Object, typename>
	size_t DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(FnSig fnPtr, Object* instance)
	{
		if(instance)
			m_Events.emplace_back(std::make_shared<CAction<FnSignature, Object>>(fnPtr, instance));
		return m_Events.size();
	}

	template <typename Ret, typename ... Args>
	template <typename FnSig, typename Object, typename>
	size_t DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(FnSig fnPtr, const SharedPtr<Object>& instance)
	{
		if(instance)
			m_Events.emplace_back(std::make_shared<CAction<FnSignature, Object>>(fnPtr, instance));
		return m_Events.size();
	}

	template <typename Ret, typename ... Args>
	void DynamicMulticastDelegate<Ret(Args...)>::UnBind(size_t idx)
	{
		if (idx > m_Events.size() ||
			idx < 0)
			return;

		m_Events.erase(std::find(m_Events.begin(),m_Events.end(),m_Events[idx]));
	}

	template <typename Ret, typename ... Args>
	size_t DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(FnSignature fn)
	{
		m_Events.emplace_back(std::make_shared<CAction<FnSignature,SLD::Empty>>(fn));
		return m_Events.size();
	}

	//template <typename Ret, typename ... Args>
	//size_t DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(typename Function_Traits<FnSignature>::fn_type&& fn)
	//{
	//	return m_Events.size();
	//}
}


#endif
