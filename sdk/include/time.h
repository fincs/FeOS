//
// FeOS C Standard Library
// time.h
//     Date and time functions
//

#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file time.h
 *  \brief Date and time functions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

//! \brief Time structure.
struct tm
{
	int tm_sec; //!< Seconds
	int tm_min; //!< Minutes
	int tm_hour; //!< Hours
	int tm_mday; //!< Day of month
	int tm_mon; //!< Month
	int tm_year; //!< Year
	int tm_wday; //!< Day of week
	int tm_yday; //!< Day of year
	int tm_isdst; //!< Is daylight-saving time
};

typedef unsigned int time_t; //!< Packed time value (UNIX time)

time_t mktime(struct tm*); //!< Packs a time structure into a time value
time_t time(time_t*); //!< Gets the current time
char* asctime(const struct tm*); //!< Converts a time structure into a printable string
char* ctime(const time_t*); //!< Gets the time in string form
struct tm* gmtime(const time_t*); //!< Gets the current time
struct tm* localtime(const time_t*); //!< Gets the current time
size_t strftime(char*, size_t, const char*, const struct tm*) __attribute__ ((format (__strftime__, 3, 0)));  //!< TODO: description

/** @} */

#ifdef __cplusplus
}
#endif
