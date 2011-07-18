//
// FeOS C Standard Library
// stdio.h
//     Input/output functions
//

#pragma once
#include <stddef.h>
#include <limits.h>
#include <feosuser.h>

int printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));
