#pragma once
#include <feos.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \file sndlock.h
	\brief FeOS sound hardware locking support
*/

typedef int sndlock_t; //!< Sound hardware lock datatype

//! \brief Acquires the sound hardware.
//! \param name The name of the requesting application or library.
//! \param pLock Pointer to a location in memory where to store the lock.
//! \returns true if successful, false if failure.
bool SndLock_Acquire(const char* name, sndlock_t* pLock);
//! \brief Releases the sound hardware.
bool SndLock_Release(sndlock_t lock);
//! \brief Retrieves the name of the application or library that is using the sound hardware, if any.
const char* SndLock_GetName();

#ifdef __cplusplus
}
#endif
