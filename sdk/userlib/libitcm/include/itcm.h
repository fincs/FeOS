#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \file itcm.h
	\brief FeOS ITCM memory allocation support
*/

//! \brief Allocates the specified amount of bytes from ITCM.
void* itcm_malloc(size_t);
//! \brief Frees ITCM memory.
void itcm_free(void*);

#ifdef __cplusplus
}
#endif
