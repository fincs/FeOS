//
// FeOS C Standard Library
// errno.h
//     [TODO]
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// These are pretty pointless...
#define EDOM   1
#define ERANGE 2

extern int errno;

#ifdef __cplusplus
}
#endif
