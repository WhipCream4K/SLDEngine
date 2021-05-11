

#ifndef SLDFRAMEWORK_SCRIPTABLECHECKS_H
#define SLDFRAMEWORK_SCRIPTABLECHECKS_H

#include <type_traits>
#include <tuple>
#include <iostream>

template<typename Ret, typename UserClass, typename ...Args>
using MemberFuncPtr = Ret(UserClass::*)(Args...);

template<typename T, typename FnType, typename = void>
constexpr bool IsOnUpdateImpl{ false };

template<typename T, typename Ret, typename ...Args>
constexpr bool IsOnUpdateImpl<T, Ret(Args...), std::void_t<decltype(std::declval<T>().OnUpdate())>>{
	std::is_same_v<decltype(&T::OnUpdate), MemberFuncPtr<Ret, T, Args...>>
		//true
};

template<typename FnType>
struct Function_Traits {};

// Function pointer
template<typename Ret, typename ...Args>
struct Function_Traits<Ret(*)(Args...)> : Function_Traits<Ret(Args...)> {};


template<typename Ret, typename ...Args>
struct Function_Traits<Ret(Args...)>
{
	using return_type = Ret;
	using fn_type = Ret(Args...);

	static constexpr size_t argSize = sizeof...(Args);

	template<size_t N>
	using args_N_type = std::tuple_element_t<N, std::tuple<Args...>>;

	struct TupleArgs
	{
		template<typename Is = std::make_index_sequence<argSize>>
		struct ArgsVal{};

		template<size_t... Is>
		struct ArgsVal<std::index_sequence<Is...>>
		{
			using type = std::tuple<std::tuple_element_t<Is, std::tuple<Args...>>...>;
		};

		using type = typename ArgsVal<>::type;
	};

};

template<size_t N, typename Is = std::make_index_sequence<N>>
struct reset {};

template<size_t N, size_t... Is>
struct reset<N, std::index_sequence<Is...>>
{
	using Indices = std::make_index_sequence<N>;
};

// member function pointer
template<typename Ret, typename UserClass, typename ...Args>
struct Function_Traits<Ret(UserClass::*)(Args...)> : Function_Traits<Ret(UserClass&, Args...)>
{
	using element_type = UserClass;
	using fn_type = Ret(Args...);
};

// const member function pointer
template<typename Ret, typename UserClass, typename ...Args>
struct Function_Traits<Ret(UserClass::*)(Args...) const> : Function_Traits<Ret(UserClass&, Args...)>
{
	using fn_type = Ret(Args...);
};

//template<typename T, typename Ret, typename ...Args>
//constexpr bool IsFuncSameType{
//	std::is_same_v<typename Function_Traits<T>::fn_type,typename Function_Traits<Ret(Args...)>::fn_type>
//};

template<typename L, typename R>
struct IsFuncSameType
{
	static constexpr bool value{ false };
};

template<typename L, typename Ret, typename ...Args>
struct IsFuncSameType<L, Ret(Args...)>
{
	using left_hand = typename Function_Traits<L>::fn_type;
	using right_hand = Ret(Args...);

	static constexpr bool value{
		std::is_same_v<left_hand,right_hand>
	};
};

struct Something
{
	template<typename ...Args, typename = std::enable_if_t<std::is_invocable_r_v<void, void(float, float), Args...>>>
	static void Test(Args&&... args)
	{
		((std::cout << args << '\n'), ...);
	}
};

#endif
