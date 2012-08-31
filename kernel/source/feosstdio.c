#include "feos.h"
#include "fxe.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define stdin_hook curExecStatus->stdin_hook
#define stdout_hook curExecStatus->stdout_hook
#define stderr_hook curExecStatus->stderr_hook

FILE* FeOS_GetStdin()
{
	return !stdin_hook ? stdin : (FILE*)stdin_hook;
}

FILE* FeOS_GetStdout()
{
	return !stdout_hook ? stdout : (FILE*)stdout_hook;
}

FILE* FeOS_GetStderr()
{
	return !stderr_hook ? stderr : (FILE*)stderr_hook;
}

FILE* FeOS_SetStdin(FILE* newstdin)
{
	FILE* oldstdin = (FILE*)stdin_hook;
	stdin_hook = newstdin;
	return oldstdin;
}

FILE* FeOS_SetStdout(FILE* newstdout)
{
	FILE* oldstdout = (FILE*)stdout_hook;
	stdout_hook = newstdout;
	return oldstdout;
}

FILE* FeOS_SetStderr(FILE* newstderr)
{
	FILE* oldstderr = (FILE*)stderr_hook;
	stderr_hook = newstderr;
	return oldstderr;
}

FILE* FeOS_OpenStream(const void*);

BEGIN_TABLE(FEOSSTDIO)
	ADD_FUNC(FeOS_GetStdin),
	ADD_FUNC(FeOS_GetStdout),
	ADD_FUNC(FeOS_GetStderr),
	ADD_FUNC(FeOS_SetStdin),
	ADD_FUNC(FeOS_SetStdout),
	ADD_FUNC(FeOS_SetStderr),
	ADD_FUNC(FeOS_OpenStream),

	// Basic I/O
	ADD_FUNC(fopen),
	ADD_FUNC(freopen),
	ADD_FUNC(fclose),
	ADD_FUNC(fwrite),
	ADD_FUNC(fread),
	ADD_FUNC(feof),
	ADD_FUNC(fseek),
	ADD_FUNC(ftell),
	ADD_FUNC(fflush),
	ADD_FUNC(ferror),
	ADD_FUNC(clearerr),
	ADD_FUNC(setvbuf),

	// Formatting
	ADD_FUNC_ALIAS(vfiprintf, vfprintf),
	ADD_FUNC_ALIAS(vsiprintf, vsprintf),
	ADD_FUNC_ALIAS(vsniprintf, vsnprintf),
	ADD_FUNC_ALIAS(vfiscanf, vfscanf),
	ADD_FUNC_ALIAS(vsiscanf, vsscanf),

	// Strings and chars
	ADD_FUNC(fgetc), ADD_FUNC(fputc),
	ADD_FUNC(fgets), ADD_FUNC(fputs),
	ADD_FUNC(ungetc),

	// Misc
	ADD_FUNC(remove),
	ADD_FUNC(rename),
	ADD_FUNC_ALIAS(__getdelim, getdelim)
END_TABLE(FEOSSTDIO)

MAKE_FAKEMODULE(FEOSSTDIO)
