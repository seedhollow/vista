#pragma once
#include <cstdio>

//#define DISABLE_LOGGING_CONSOLE

// check if build release then disable logging, otherwise it will cause performance issues
/*#ifndef _DEBUG
#define DISABLE_LOGGING_CONSOLE
#endif*/



#ifndef DISABLE_LOGGING_CONSOLE
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

namespace Console {
	void Alloc( );
	void Free( );
}
