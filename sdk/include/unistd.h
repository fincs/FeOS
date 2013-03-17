//
// FeOS C Standard Library
// unistd.h
//     POSIX-like misc functions
//

#pragma once
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file unistd.h
 *  \brief POSIX-like misc functions
 */

/** @addtogroup crt_io Input/Output
 *  @{
 */

int chdir(const char*); //!< Changes the current directory.
char* getcwd(char*, size_t); //!< Gets the current directory.
int ftruncate(int fd, off_t length); //!< Truncates a file.

/** @} */

#ifdef __cplusplus
}
#endif
