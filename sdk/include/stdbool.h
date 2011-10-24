//
// FeOS C Standard Library
// stdbool.h
//     Boolean definitions
//

#pragma once

#define TRUE 1
#define FALSE 0

#ifndef __cplusplus
typedef enum { false, true } bool;
#endif
