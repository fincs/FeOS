//
// FeOS C Standard Library
// stdbool.h
//     Boolean definitions
//

#pragma once

/** @file stdbool.h
 *  \brief Boolean definitions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

#ifndef TRUE
#define TRUE 1 //!< True.
#endif

#ifndef FALSE
#define FALSE 0 //!< False.
#endif

#ifndef __cplusplus
#ifndef false
#ifndef true
typedef enum { false, true } bool; //!< C++-compatible bool type.
#endif
#endif
#endif

/** @} */
