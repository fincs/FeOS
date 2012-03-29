#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <feos.h>

typedef struct
{
	char buf[256];
	const char* argv[32];
	int rc;
} cmd_data;

static inline cmd_data* cmd_new()
{
	return (cmd_data*) malloc(sizeof(cmd_data));
}

#define cmd_free(x) free(x)

int cmd_execute(cmd_data* data);

typedef struct
{
	const char* stdout_file;
	const char* stdout_mode;
} redir_data;

char* trim_whitespace(char* buf);
int parse_cmdline(char* cmdline, const char* argv[], redir_data* redir);

#define NO_HOOK ((FILE*)-1)

typedef struct
{
	FILE* orgStdout;
	FILE* hookStdout;
} hook_data;

void redir_hook(hook_data* hook, redir_data* redir);
void redir_unhook(hook_data* hook);
