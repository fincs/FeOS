#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static inline int my_isspace(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

char* trim_whitespace(char* buf)
{
	// Remove trailing whitespace
	int pos;
	for(pos = strlen(buf)-1; pos >= 0 && my_isspace(buf[pos]); pos --) buf[pos] = '\0';

	// Remove leading whitespace
	char* newbuf = buf;
	for(; my_isspace(*newbuf); newbuf ++);

	return newbuf;
}

int parse_cmdline(char* cmdline, const char* argv[])
{
	char* bufp;
	int argc;

	argc = 0;
	for(bufp = cmdline; *bufp && argc < 32;)
	{
		// Skip leading whitespace
		for(; my_isspace(*bufp); bufp ++);

		// Skip over argument
		if(*bufp == '"')
		{
			bufp ++;
			if(*bufp) argv[argc++] = bufp;

			// Skip over word
			for(; *bufp && *bufp != '"'; bufp ++);
		}else
		{
			if(*bufp) argv[argc++] = bufp;

			// Skip over word
			for(; *bufp && !my_isspace(*bufp); bufp ++ );
		}

		if (*bufp) *bufp++ = '\0';
	}
	
	return argc;
}

typedef struct
{
	char buf[256];
	const char* argv[32];
} cmd_data;

int main()
{
	cmd_data* data = (cmd_data*) malloc(sizeof(cmd_data));
	if (!data) abort();

	printf("\nFeOS command prompt v0.0\n\n");
	for(;;)
	{
		FILE* hook = NULL;

		printf("> ");
		fgets(data->buf, sizeof(data->buf), stdin);
		int argc = parse_cmdline(data->buf, data->argv);
		if (argc == 0) continue;
		const char* cmd = data->argv[0];
		const char* lastarg = data->argv[argc-1];

		if (*lastarg == '>')
		{
			const char* filename = lastarg + 1;
			const char* mode = "w";

			if (*filename == '>') filename ++, mode = "a";

			argc --;
			if (argc == 0) continue;

			hook = fopen(filename, mode);
			if (hook == NULL)
			{
				fprintf(stderr, "Error opening '%s': %s\n", filename, strerror(errno));
				continue;
			}

			hook = FeOS_SetStdout(hook);
		}

		if (strcmp(cmd, "exit") == 0) break;

		int rc = FeOS_Execute(argc, data->argv);

		hook = FeOS_SetStdout(hook);
		if (hook) fclose(hook);

		switch(rc)
		{
			case 0: break;
			case E_INVALIDARG:
			case E_FILENOTFOUND: printf("Bad command or filename\n"); break;
			case E_APPKILLED: printf("'%s' was terminated!\n", cmd); break;
			default: printf("'%s' returned with RC=%d\n", cmd, rc);
		}
	}

	free(data);
	return 0;
}
