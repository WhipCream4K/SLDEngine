
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
			typename = EnableIf<std::is_same_v<FnSignature,typename Function_Traits<FnSig>::fn_type>>>
			void AddDynamic(FnSig fnPtr, Object* instance);

		template<
			typename FnSig,
			typename Object = typename Function_Traits<FnSig>::element_type,
			typename = EnableIf<std::is_same_v<FnSignature, typename Function_Traits<FnSig>::fn_type>>>
			void AddDynamic(FnSig fnPtr,const RefPtr<Object>& instance);

	private:

		std::vector<RefPtr<DynamicDelegate<FnSignature>>> m_Events;
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
	void DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(FnSig fnPtr, Object* instance)
	{
		if(instance)
			m_Events.emplace_back(std::make_shared<CAction<FnSignature, Object>>(fnPtr, instance));
	}

	template <typename Ret, typename ... Args>
	template <typename FnSig, typename Object, typename>
	void DynamicMulticastDelegate<Ret(Args...)>::AddDynamic(FnSig fnPtr, const RefPtr<Object>& instance)
	{
		if(instance)
			m_Events.emplace_back(std::make_shared<CAction<FnSignature, Object>>(fnPtr, instance));\
	}
}


#endif
