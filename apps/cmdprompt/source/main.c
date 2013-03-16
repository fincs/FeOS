#include "cmd.h"

#define ANSIESC_RED "\x1b[31;1m"
#define ANSIESC_GREEN "\x1b[32;1m"
#define ANSIESC_YELLOW "\x1b[33;1m"
#define ANSIESC_DEFAULT "\x1b[39;1m"
#define SHELL_PROGRAM "b0rkshell"

FEOS_EXPORT int __system(const char* command)
{
	cmd_data* data = cmd_new();
	if (!data)
	{
		fprintf(stderr, "'cmd': out of memory!\n");
		return E_OUTOFMEMORY;
	}

	strncpy(data->buf, command, sizeof(data->buf));
	cmd_execute(data);

	int rc = data->rc;
	cmd_free(data);
	return rc;
}

static int printUsage(const char* progname)
{
	printf("Usage:\n%s [cmdFile]\n", progname);
	return 0;
}

static int executeFile(cmd_data* data, const char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		fprintf(stderr, "Error opening '%s': %s\n", fileName, strerror(errno));
		return 1;
	}

	while (!feof(f) && fgets(data->buf, sizeof(data->buf), f) && cmd_execute(data));
	fclose(f);

	return data->rc;
}

static inline bool checkNoShellBoot()
{
	word_t k = keysHeld();
	return (k & KEY_DOWN) && (k & KEY_LEFT) && (k & KEY_A);
}

static bool executeShell(cmd_data* data)
{
	static const char* shell_argv[] = { SHELL_PROGRAM, NULL };
	static const int shell_argc = 1;

	bool result = true;

	if (checkNoShellBoot())
		result = false;
	else
	{
		int rc = FeOS_Execute(shell_argc, shell_argv);

		if (rc == E_FILENOTFOUND)
			return false; // Shortcut.

		if (rc != 0)
		{
			result = false;
			printf(ANSIESC_YELLOW "Shell failed to start: %d\n\n" ANSIESC_DEFAULT, rc);
		}
	}

	if (!result)
	{
		printf(ANSIESC_YELLOW "FeOS Fail-Safe mode\n");
		printf(ANSIESC_DEFAULT "Command prompt boot enabled\n");
		printf("\n");
	}

	return result;
}

static int interactiveMode(cmd_data* data)
{
	printf("FeOS command prompt v" FEOS_VERSION_TEXT "\n\n");

	for (;;)
	{
		printf("> ");
		fgets(data->buf, sizeof(data->buf), stdin);
		if (!cmd_execute(data))
			break;
	}

	return data->rc;
}

int main(int argc, const char* argv[])
{
	int rc = 0;
	cmd_data* data = cmd_new();
	if (!data) abort();

	if (argc > 2)
		rc = printUsage(argv[0]);
	else if (argc == 2)
	{
		bool bStartup = false;
		const char* filename = argv[1];

		if (strcmp(filename, ":startup") == 0)
		{
			filename = "/data/FeOS/autoexec.cmd";
			bStartup = true;
		}

		rc = executeFile(data, filename);

		if (bStartup && !executeShell(data))
			// Shell startup failed, was disabled or was not found - enter interactive mode
			rc = interactiveMode(data);
	} else
		rc = interactiveMode(data);

	cmd_free(data);
	return rc;
}
