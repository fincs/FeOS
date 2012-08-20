//
// FeOS C Standard Library
// stdio.h
//     Input/output functions
//

#pragma once
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
} FILE;

typedef struct
{
	int     (*Open) (void*); // 0 = success, anything else (errno.h): failure
	void    (*Close)(void*);
	ssize_t (*Write)(void*, const char*, size_t);
	ssize_t (*Read) (void*, char*, size_t);
	off_t   (*Seek) (void*, off_t, int);
} stream_t;

FILE* FeOS_GetStdin();
FILE* FeOS_GetStdout();
FILE* FeOS_GetStderr();
FILE* FeOS_SetStdin(FILE*);
FILE* FeOS_SetStdout(FILE*);
FILE* FeOS_SetStderr(FILE*);
FILE* FeOS_OpenStream(const stream_t*, void*);

#define stdin  FeOS_GetStdin()
#define stdout FeOS_GetStdout()
#define stderr FeOS_GetStderr()

#define	SEEK_SET 0
#define	SEEK_CUR 1
#define	SEEK_END 2

#define EOF (-1)
#define	FOPEN_MAX 20
#define	FILENAME_MAX 1024

FILE* fopen(const char*, const char*);
FILE* freopen(const char*, const char*, FILE*);
int fclose(FILE*);

FILE* fdopen(int fd, const char*);

size_t fread(void*, size_t, size_t, FILE*);
size_t fwrite(const void*, size_t, size_t, FILE*);
int feof(FILE*);

int fseek(FILE*, int, int);
int ftell(FILE*);
void rewind(FILE*);

int fflush(FILE*);
int ferror(FILE*);
void clearerr(FILE*);

int vfprintf(FILE*, const char*, va_list) __attribute__ ((format (__printf__, 2, 0)));
int vsprintf(char*, const char*, va_list) __attribute__ ((format (__printf__, 2, 0)));
int vsnprintf(char*, size_t, const char*, va_list) __attribute__ ((format (__printf__, 3, 0)));
int vfscanf(FILE*, const char*, va_list) __attribute__ ((format (__scanf__, 2, 0)));;
int vsscanf(const char*, const char*, va_list) __attribute__ ((format (__scanf__, 2, 0)));;

int vprintf(const char*, va_list) __attribute__ ((format (__printf__, 1, 0)));
int vscanf(const char*, va_list) __attribute__ ((format (__scanf__, 1, 0)));;

int printf(const char*, ...) __attribute__ ((format (__printf__, 1, 2)));
int fprintf(FILE*, const char*, ...) __attribute__ ((format (__printf__, 2, 3)));
int sprintf(char*, const char*, ...) __attribute__ ((format (__printf__, 2, 3)));
int snprintf(char*, size_t, const char*, ...) __attribute__ ((format (__printf__, 3, 4)));

int scanf(const char*, ...) __attribute__ ((format (__scanf__, 1, 2)));
int fscanf(FILE*, const char*, ...) __attribute__ ((format (__scanf__, 2, 3)));
int sscanf(const char*, const char*, ...) __attribute__ ((format (__scanf__, 2, 3)));

int fgetc(FILE*);
int fputc(int, FILE*);
char* fgets(char*, int, FILE*);
int fputs(const char*, FILE*);

#define getc fgetc
#define putc fputc

int getchar();
char* gets(char*);

int ungetc(int, FILE*);

int putchar(int);
int puts(const char*);

int remove(const char*);
int rename(const char*, const char*);

int fileno(FILE*);

#ifdef __cplusplus
}
#endif
