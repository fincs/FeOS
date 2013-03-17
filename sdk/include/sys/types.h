//
// FeOS C Standard Library
// sys/types.h
//     POSIX-like type definitions
//

#pragma once
#include <stddef.h>
#include <time.h>

/** @file types.h
 *  \brief POSIX-like type definitions
 */

/** @addtogroup crt_io Input/Output
 *  @{
 */

#define NAME_MAX 767 //!< Maximum filename length.

typedef int dev_t; //!< TODO: description
typedef int mode_t; //!< TODO: description
typedef int ino_t; //!< TODO: description
typedef unsigned short uid_t; //!< TODO: description
typedef unsigned short gid_t; //!< TODO: description
typedef unsigned short nlink_t; //!< TODO: description

/** @} */
