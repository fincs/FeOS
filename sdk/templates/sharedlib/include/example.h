#pragma once
#include <feos.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef EXAMPLELIB_BUILD
#define EXAMPLELIB_API FEOS_EXPORT
#else
#define EXAMPLELIB_API
#endif

EXAMPLELIB_API int exampleFunction();

#ifdef __cplusplus
}
#endif
