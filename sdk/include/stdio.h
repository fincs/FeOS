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

/** @file stdio.h
 *  \brief C standard library I/O functions
 */

/** @addtogroup crt_io Input/Output
 *  @brief Input/Output API, mixture of C standard library and POSIX-like elements.
 *  @code
 *  #include <stdio.h>
 *  #include <dirent.h>
 *  #include <fcntl.h>
 *  #include <unistd.h>
 *  #include <sys/stat.h>
 *  #include <sys/types.h>
 *  @endcode
 *  @{
 */

//! \brief C data stream object.
//!
//! This is an opaque datatype that contains no public fields.
typedef struct
{
} FILE;

//! \brief FeOS custom stream callback structure.
//!
//! Set any of these fields to NULL in order to specify that it doesn't support the said operation.
typedef struct
{
	int     (*Open) (void*); //!< Called when the stream is opened. \return 0 = success, anything else (errno.h): failure
	void    (*Close)(void*); //!< Called when the stream is closed.
	ssize_t (*Write)(void*, const char*, size_t); //!< Called when the stream is written to.
	ssize_t (*Read) (void*, char*, size_t); //!< Called when the stream is read from.
	off_t   (*Seek) (void*, off_t, int); //!< Called when the stream is sought.
} stream_t;

FILE* FeOS_GetStdin(); //!< Gets the standard input stream.
FILE* FeOS_GetStdout(); //!< Gets the standard output stream.
FILE* FeOS_GetStderr(); //!< Gets the standard error stream.
FILE* FeOS_SetStdin(FILE*); //!< Hooks the standard input stream. Returns the previous hook.
FILE* FeOS_SetStdout(FILE*); //!< Hooks the standard output stream. Returns the previous hook.
FILE* FeOS_SetStderr(FILE*); //!< Hooks the standard error stream. Returns the previous hook.
FILE* FeOS_OpenStream(const stream_t*, void*); //!< Opens a custom data stream with the specified callbacks and user data.

#define stdin  FeOS_GetStdin()  //!< C stdin object.
#define stdout FeOS_GetStdout() //!< C stdout object.
#define stderr FeOS_GetStderr() //!< C stderr object.

#define	SEEK_SET 0 //!< Specifies seeking from the beginning of the stream.
#define	SEEK_CUR 1 //!< Specifies seeking from the current position.
#define	SEEK_END 2 //!< Specifies seeking from the end of the stream.

#define EOF (-1) //!< Signals end-of-file.
#define FOPEN_MAX 20 //!< Maximum number of simultaneous opened files.
#define FILENAME_MAX 1024 //!< Maximum filename length.

FILE* fopen(const char*, const char*); //!< Opens a file. Refer to other documentation for more details.
FILE* freopen(const char*, const char*, FILE*); //!< Reopens a file. Refer to other documentation for more details.
int fclose(FILE*); //!< Closes a C data stream object.

FILE* fdopen(int fd, const char*); //!< Wraps a POSIX file descriptor into a C data stream object.

size_t fread(void*, size_t, size_t, FILE*); //!< Reads data from a C stream.
size_t fwrite(const void*, size_t, size_t, FILE*); //!< Writes data to a C stream.
int feof(FILE*); //!< Checks if a C stream is at the end.

int fseek(FILE*, int, int); //!< Sets the position of a C stream.
int ftell(FILE*); //!< Retrieves the position of a C stream.
void rewind(FILE*); //!< Sets the position of a C stream to the beginning.

int fflush(FILE*); //!< Flushes the write buffer of a  C stream.
int ferror(FILE*); //!< Returns whether a C stream had an error.
void clearerr(FILE*); //!< Clears the C stream error flag.

#define _IOFBF 0 //!< Specifies full buffering.
#define _IOLBF 1 //!< Specifies line buffering.
#define _IONBF 2 //!< Disables buffering.

int setvbuf(FILE*, char*, int, size_t); //!< Sets the C stream buffering mode.

int vfprintf(FILE*, const char*, va_list) __attribute__ ((format (__printf__, 2, 0))); //!< TODO: description
int vsprintf(char*, const char*, va_list) __attribute__ ((format (__printf__, 2, 0))); //!< TODO: description
int vsnprintf(char*, size_t, const char*, va_list) __attribute__ ((format (__printf__, 3, 0))); //!< TODO: description
int vfscanf(FILE*, const char*, va_list) __attribute__ ((format (__scanf__, 2, 0))); //!< TODO: description
int vsscanf(const char*, const char*, va_list) __attribute__ ((format (__scanf__, 2, 0))); //!< TODO: description

int vprintf(const char*, va_list) __attribute__ ((format (__printf__, 1, 0))); //!< TODO: description
int vscanf(const char*, va_list) __attribute__ ((format (__scanf__, 1, 0))); //!< TODO: description

int printf(const char*, ...) __attribute__ ((format (__printf__, 1, 2))); //!< TODO: description
int fprintf(FILE*, const char*, ...) __attribute__ ((format (__printf__, 2, 3))); //!< TODO: description
int sprintf(char*, const char*, ...) __attribute__ ((format (__printf__, 2, 3))); //!< TODO: description
int snprintf(char*, size_t, const char*, ...) __attribute__ ((format (__printf__, 3, 4))); //!< TODO: description

int scanf(const char*, ...) __attribute__ ((format (__scanf__, 1, 2))); //!< TODO: description
int fscanf(FILE*, const char*, ...) __attribute__ ((format (__scanf__, 2, 3))); //!< TODO: description
int sscanf(const char*, const char*, ...) __attribute__ ((format (__scanf__, 2, 3))); //!< TODO: description

int fgetc(FILE*); //!< Reads a character (byte) from a C stream. Can return EOF.
int fputc(int, FILE*); //!< Writes a character (byte) to a C stream.
char* fgets(char*, int, FILE*); //!< Reads a newline-terminated string from a C stream.
int fputs(const char*, FILE*); //!< Writes a string into a C stream.

#define getc fgetc //!< Alias for fgetc.
#define putc fputc //!< Alias for fputc.

int getchar(); //!< Alias for fgetc(stdin).
char* gets(char*); //!< Alias for fgets(<param>, <big number>, stdin).

int ungetc(int, FILE*); //!< Puts a character back into a C stream (?).

int putchar(int); //!< Alias for fputc(<param>, stdout).
int puts(const char*); //!< Alias for fputs(<param>, stdout) followed by fputc('\\n', stdout).

int remove(const char*); //!< Removes a file.
int rename(const char*, const char*); //!< Renames a file.

int fileno(FILE*); //!< Gets the internal POSIX file descriptor of a C stream.

//! TODO: description
ssize_t getdelim(char** ppBuf, size_t* pSize, int delim, FILE* f);

//! TODO: description
static inline ssize_t getline(char** ppBuf, size_t* pSize, FILE* f)
{
	return getdelim(ppBuf, pSize, '\n', f);
}

// newlib compatibility function aliases

#define iprintf   printf //!< newlib compatibility alias for printf.
#define viprintf  vprintf //!< newlib compatibility alias for vprintf.
#define siprintf  sprintf //!< newlib compatibility alias for sprintf.
#define vsiprintf vsprintf //!< newlib compatibility alias for vsprintf.
#define fiprintf  fprintf //!< newlib compatibility alias for fprintf.
#define vfiprintf vfprintf //!< newlib compatibility alias for vfprintf.

#define iscanf  scanf //!< newlib compatibility alias for scanf.
#define siscanf sscanf //!< newlib compatibility alias for sscanf.
#define fiscanf fscanf //!< newlib compatibility alias for fscanf.

/** @} */

#ifdef __cplusplus
}
#endif
