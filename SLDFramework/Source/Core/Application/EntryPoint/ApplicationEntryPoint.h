
#ifndef SLD_FRAMEWORK_ENTRYPOINT_H
#define SLD_FRAMEWORK_ENTRYPOINT_H

#include "../Application.h"

template<typename AppClass,typename ...Args>
int ConsoleEntry(Args... args)
{
	SharedPtr<AppClass> pApp{ std::make_shared<AppClass>(std::forward<Args>(args)...) };

	pApp->OnAppStartUp();
	
	return 0;
}

#define SLD_CONSOLEAPP_ENTRYPOINT(AppClass,...)	\
	int main()	{return ConsoleEntry<AppClass>(__VA_ARGS__);}	\

#endif
