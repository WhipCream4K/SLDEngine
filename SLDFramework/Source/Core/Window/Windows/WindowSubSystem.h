
#ifndef SLDFRAMEWORK_WINDOWSUBSYSTEM_H
#define SLDFRAMEWORK_WINDOWSUBSYSTEM_H

#include "../../Base.h"

namespace SLD
{
	class WindowSubSystem
	{
	public:

		class ImpleWindowSubSystem;

		void Present();
		void ClearBackBuffer(const float (&clearColor)[4]);
		
	private:

		OwnedPtr<ImpleWindowSubSystem> m_pImpleWindow;
		
	};
}

#endif
