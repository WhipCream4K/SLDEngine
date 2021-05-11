
#ifndef SLD_APPLICATION_H
#define SLD_APPLICATION_H

#include "Core/Core.h"

namespace SLD
{
	class Application
	{
	public:

		Application();

		// User Defined
		virtual void OnAppBegin() {}
		virtual void OnGameConstruct() {}
		virtual ~Application() = default;

		void InitCore(const std::any& windowHandle);
		void UpdateCore();
		
		
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		Core* m_instance;	
	};
}

#endif
