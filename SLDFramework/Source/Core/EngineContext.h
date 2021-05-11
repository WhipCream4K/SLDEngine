
#ifndef SLDFRAMEWORK_ENGINE_CONTEXT_H
#define SLDFRAMEWORK_ENGINE_CONTEXT_H

#include <cstdint>

namespace SLD
{
	class EngineContext
	{
	public:
		static constexpr const wchar_t* windowClassName{ L"SLD_FRAMEWORK" };
		static constexpr uint32_t WindowWidth{ 1280 };
		static constexpr uint32_t WindowHeight{ 720 };
		//static constexpr size_t DefaultInputComponentSize{ sizeof(InputComponent) * 2 };
	};

}


#endif