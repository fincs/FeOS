#include "cmd.h"

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

int main(int argc, const char* argv[])
{
	cmd_data* data = cmd_new();
	if (!data) abort();

	bool bInteractive = true;
	FILE* fFile = NULL;

	if (argc > 2)
	{
		printf("Usage:\n%s [cmdFile]\n", argv[0]);
		return 0;
	}

	if (argc == 2)
	{
		const char* filename = argv[1];
		if (strcmp(filename, ":startup") == 0)
			filename = "/data/FeOS/autoexec.cmd";
		else
			bInteractive = false;
		fFile = fopen(filename, "r");
		if (!fFile)
			fprintf(stderr, "Error opening '%s': %s\n", filename, strerror(errno));
	}

	if (fFile)
	{
		while (!feof(fFile) && fgets(data->buf, sizeof(data->buf), fFile) && cmd_execute(data));
		fclose(fFile);
	}

	if (bInteractive)
	{
		printf("FeOS command prompt v" FEOS_VERSION_TEXT "\n\n");

		for (;;)
		{
			printf("> ");
			fgets(data->buf, sizeof(data->buf), stdin);
			if (!cmd_execute(data))
				break;
		}
	}

	int rc = data->rc;
	cmd_free(data);
	return rc;
}
