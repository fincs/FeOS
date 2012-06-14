#pragma once
#include <stdlib.h>
#include <feos.h>

typedef void (*atexitCb)(void* obj);

int __reg_atexit(atexitCb func, void* obj);
__attribute__((weak)) void __call_atexit();
