//
// FeOS Standard Library
// feoserror.h
//     FeOS error codes
//

#pragma once

/** @file feoserror.h
 *  \brief FeOS error codes
 */

/** @addtogroup api_base Core FeOS API
 *  @{
 */

#define E_OUTOFMEMORY (-4) //!< Out of memory.
#define E_FILENOTFOUND (-10) //!< File not found.
#define E_INVALIDARG (-11) //!< Invalid argument.
#define E_APPKILLED (-12) //!< Application was violently murdered by the kernel.

/** @} */
