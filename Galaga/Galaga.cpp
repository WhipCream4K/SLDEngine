// Galaga.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GalagaScene.h"
#include <vld.h>
#include <Tracer/minitrace.h>

int main()
{
	GalagaScene sc{};

#ifdef MTR_ENABLED
	
	mtr_init("./tracer.json");

	MTR_META_PROCESS_NAME("gameloop test");
	MTR_META_THREAD_NAME("main thread");

#endif
	
	sc.WorldCreation();

	sc.Run();

#ifdef MTR_ENABLED
	mtr_flush();
	mtr_shutdown();
#endif
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
