//
// FeOS C Standard Library
// dirent.h
//     POSIX-like directory functions
//

#pragma once
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file dirent.h
 *  \brief POSIX-like directory functions.
 */

/** @addtogroup crt_io Input/Output
 *  @{
 */

#define DT_UNKNOWN 0 //!< Unknown directory entry.
#define DT_DIR     4 //!< Sub-directory.
#define DT_REG     8 //!< File.

//! \brief Directory object.
//!
//! This is an opaque datatype that contains no public fields.
typedef struct
{
} DIR;

//! \brief Directory entry information.
//!
//! This is an opaque datatype that contains no public fields.
struct dirent
{
	ino_t d_ino; //!< TODO: description.
	unsigned char d_type; //!< Type of the entry (see DT_*).
	char d_name[NAME_MAX+1]; //!< Name of the entry.
};

DIR* opendir(const char*); //!< Opens a directory.
int closedir(DIR*); //!< Closes a directory object.
struct dirent* readdir(DIR*); //!< Reads the next entry of a directory object.
void rewinddir(DIR*); //!< Seeks back to the beginning of a directory.
void seekdir(DIR*, int); //!< Seeks to a certain position in a directory.
int telldir(DIR*); //!< Retrieves the current enumeration position in a directory.

/** @} */

#ifdef __cplusplus
}
#endif
