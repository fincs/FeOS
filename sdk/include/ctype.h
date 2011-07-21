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

int isalnum(int);
int isalpha(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);

#ifdef __cplusplus
}
#endif
