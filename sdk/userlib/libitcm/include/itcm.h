#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* itcm_malloc(size_t);
void itcm_free(void*);

#ifdef __cplusplus
}
#endif
