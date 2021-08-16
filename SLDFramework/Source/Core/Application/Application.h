
#ifndef SLD_FRAMEWORK_APPLICATION
#define SLD_FRAMEWORK_APPLICATION

#include "../WorldEntity.h"

namespace SLD
{
	class Application
	{
	public:

		virtual ~Application();

		virtual void OnAppStartUp() = 0;
		virtual void OnWorldCreation(const SharedPtr<WorldEntity>&) = 0;

		/// <summary>
		/// This function runs before the main world loop phase
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void OnPreRun(float) = 0;

		virtual void OnExit() = 0;
	};

	//static void RunApp(const SharedPtr<Application>& appInstance);
}


#endif


