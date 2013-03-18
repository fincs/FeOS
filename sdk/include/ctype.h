//
// FeOS C Standard Library
// ctype.h
//     Character handling functions
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/** @file ctype.h
 *  \brief Character handling functions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

int isalnum(int); //!< Checks if a character is alphanumeric.
int isalpha(int); //!< Checks if a character is alphabetic.
int iscntrl(int); //!< Checks if a character is a control character.
int isdigit(int); //!< Checks if a character is a digit.
int isgraph(int); //!< Checks if a character is a non-whitespace printable character.
int islower(int); //!< Checks if a character is a lowercase letter.
int isprint(int); //!< Checks if a character is a printable character.
int ispunct(int); //!< Checks if a character is punctuation.
int isspace(int); //!< Checks if a character is whitespace.
int isupper(int); //!< Checks if a character is an uppercase letter.
int isxdigit(int); //!< Checks if a character is a hexadecimal digit.
int tolower(int); //!< Converts the specified character into lowercase.
int toupper(int); //!< Converts the specified character into uppercase.

/** @} */

#ifdef __cplusplus
}
#endif
