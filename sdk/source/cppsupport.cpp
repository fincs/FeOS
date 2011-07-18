#include <feos.h>
#include <stdlib.h>

void* operator new(size_t size)  { return malloc(size); }
void operator delete(void *p)    { free(p);             }
void* operator new[](size_t size){ return malloc(size); }
void operator delete[](void *p)  { free(p);             }

extern "C" void __cxa_pure_virtual()
{
	FeOS_DebugPrint("{C++} Pure virtual call!");
	for(;;);
}
