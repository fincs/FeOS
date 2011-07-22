//
// FeOS C Standard Library
// sys/stat.h
//     POSIX-like type definitions
//

#pragma once
#include <stddef.h>
#include <time.h>

#define NAME_MAX 767

typedef int dev_t;
typedef int mode_t;
typedef int ino_t;
typedef int uid_t;
typedef int gid_t;
typedef int off_t;
typedef unsigned short nlink_t;
