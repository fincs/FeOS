#include "feos.h"
#include "fxe.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* stdout_hook = NULL;

FILE* FeOS_GetStdin()
{
	return stdin;
}

FILE* FeOS_GetStdout()
{
	return !stdout_hook ? stdout : stdout_hook;
}

FILE* FeOS_GetStderr()
{
	return stderr;
}

FILE* FeOS_SetStdout(FILE* newstdout)
{
	FILE* oldstdout = stdout_hook;
	stdout_hook = newstdout;
	return oldstdout;
}

BEGIN_TABLE(FEOSSTDIO)
	ADD_FUNC(FeOS_GetStdin),
	ADD_FUNC(FeOS_GetStdout),
	ADD_FUNC(FeOS_GetStderr),
	ADD_FUNC(FeOS_SetStdout),

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

	// Formatting
	ADD_FUNC_ALIAS(vfiprintf, vfprintf),
	ADD_FUNC_ALIAS(vsiprintf, vsprintf),
	ADD_FUNC_ALIAS(vfiscanf, vfscanf),
	ADD_FUNC_ALIAS(vsiscanf, vsscanf),

	// Strings and chars
	ADD_FUNC(fgetc), ADD_FUNC(fputc),
	ADD_FUNC(fgets), ADD_FUNC(fputs),
	ADD_FUNC(ungetc),

	// Misc
	ADD_FUNC(remove),
	ADD_FUNC(rename)
END_TABLE(FEOSSTDIO)

extern void* _inst_FEOSSTDIO;

fxe_runtime_header _header_FEOSSTDIO =
{
	&_inst_FEOSSTDIO, // hThis
	"FEOSSTDIO", // name
	1, // refcount
	-1, // file
	NULL, // entrypoint
	MAKE_EXPORTSTRUCT(FEOSSTDIO), // exp
	{ 0, NULL }, // imp
	NULL, // next
	NULL // prev
};

void* _inst_FEOSSTDIO = &_header_FEOSSTDIO;
