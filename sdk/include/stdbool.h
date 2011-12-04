//
// FeOS C Standard Library
// stdbool.h
//     Boolean definitions
//

#pragma once

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef __cplusplus
#ifndef false
#ifndef true
typedef enum { false, true } bool;
#endif
#endif
#endif
