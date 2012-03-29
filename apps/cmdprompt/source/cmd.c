#include "cmd.h"

int cmd_execute(cmd_data* data)
{
	redir_data redir;

	int argc = parse_cmdline(data->buf, data->argv, &redir);
	if (argc == 0) return 1;

	const char* cmd = data->argv[0];
	if (stricmp(cmd, "exit") == 0)
	{
		data->rc = data->argv[1] ? atoi(data->argv[1]) : 0;
		return 0;
	}
	if (stricmp(cmd, "echo") == 0)
	{
		data->rc = 0;
		printf("%s\n", data->argv[1] ? data->argv[1] : "");
		return 1;
	}

	hook_data hook;
	redir_hook(&hook, &redir);
	data->rc = FeOS_Execute(argc, data->argv);
	redir_unhook(&hook);

	switch(data->rc)
	{
		case 0: break;
		case E_INVALIDARG:
		case E_FILENOTFOUND: printf("Bad command or filename\n"); break;
		case E_APPKILLED: printf("'%s' was terminated!\n", cmd); break;
		default: printf("'%s' returned with RC=%d\n", cmd, data->rc);
	}

	return 1;
}

char* trim_whitespace(char* buf)
{
	// Remove trailing whitespace
	int pos;
	for(pos = strlen(buf)-1; pos >= 0 && isspace(buf[pos]); pos --) buf[pos] = '\0';

	// Remove leading whitespace
	char* newbuf = buf;
	for(; isspace(*newbuf); newbuf ++);

	return newbuf;
}

int parse_cmdline(char* cmdline, const char* argv[], redir_data* redir)
{
	char* bufp;
	int argc;

	memset(redir, 0, sizeof(redir_data));

	argc = 0;
	for (bufp = cmdline; *bufp && argc < 32;)
	{
		// Skip leading whitespace
		for (; isspace(*bufp); bufp ++);

		// Skip over argument
		if (*bufp == '"')
		{
			bufp ++;
			if (*bufp) argv[argc++] = bufp;

			// Skip over word
			for (; *bufp && *bufp != '"'; bufp ++);
		} else
		{
			if (*bufp) argv[argc++] = bufp;

			// Skip over word
			for (; *bufp && !isspace(*bufp); bufp ++ );
		}

		if (*bufp) *bufp++ = '\0';
	}

	argv[argc] = NULL;

	if (argc > 1) do
	{
		const char* lastarg = argv[argc-1];
		if (*lastarg++ != '>') break;
		if (!*lastarg) break;
		argc --;
		argv[argc] = NULL;
		
		if (*lastarg != '>')
		{
			redir->stdout_file = lastarg;
			redir->stdout_mode = "w";
		} else
		{
			redir->stdout_file = lastarg + 1;
			redir->stdout_mode = "a";
		}
	} while(0);
	
	return argc;
}

void redir_hook(hook_data* hook, redir_data* redir)
{
	if (!redir->stdout_file)
	{
		hook->hookStdout = NO_HOOK;
		return;
	}
	hook->hookStdout = fopen(redir->stdout_file, redir->stdout_mode);
	if (!hook->hookStdout)
	{
		hook->hookStdout = NO_HOOK;
		fprintf(stderr, "Error opening '%s': %s\n", redir->stdout_file, strerror(errno));
		return;
	}
	hook->orgStdout = FeOS_SetStdout(hook->hookStdout);
}

void redir_unhook(hook_data* hook)
{
	if (hook->hookStdout == NO_HOOK) return;
	FeOS_SetStdout(hook->orgStdout);
	fclose(hook->hookStdout);
}
