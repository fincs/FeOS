//
// FeOS C Standard Library
// ctype.h
//     Date and time functions
//

#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct tm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

typedef unsigned int time_t;

time_t mktime(struct tm*);
time_t time(time_t*);
char* asctime(const struct tm*);
char* ctime(const time_t*);
struct tm* gmtime(const time_t*);
struct tm* localtime(const time_t*);
size_t strftime(char*, size_t, const char*, const struct tm*) __attribute__ ((format (__strftime__, 3, 0)));

#ifdef __cplusplus
}
#endif
