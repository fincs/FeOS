#include "common.h"

typedef int (* fpmCommand)(int argc, char* argv[]);

typedef struct
{
	const char* cmdName;
	fpmCommand func;
	bool requiresWifi;
	const char* usage;
} fpmCommandDef;

static int cmd_list(int argc, char* argv[]);
static int cmd_install(int argc, char* argv[]);
static int cmd_remove(int argc, char* argv[]);

static fpmCommandDef cmds[] =
{
	{ "list", cmd_list, false, "" },
	{ "install", cmd_install, true, "[--silent] packages..." },
	{ "remove", cmd_remove, true, "[--silent] packages..." },
	{ NULL, NULL, false, NULL }
};

int usage(const char* appName)
{
	printf("Usage:\n");
	fpmCommandDef* pCmd = cmds;
	for (; pCmd->cmdName; pCmd ++)
		printf("  %s %s %s\n", appName, pCmd->cmdName, pCmd->usage);
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc == 1)
		return usage(argv[0]);

	const char* commandName = argv[1];
	fpmCommandDef* pCmd = cmds;
	for (; pCmd->cmdName; pCmd ++)
		if (stricmp(commandName, pCmd->cmdName) == 0)
		{
			if (pCmd->requiresWifi)
				if (!Wifi_Startup())
				{
					fprintf(stderr, "Cannot start Wifi system!\n");
					return 1;
				}

			int rc = pCmd->func(argc - 2, argv + 2);

			if (pCmd->requiresWifi)
				Wifi_Cleanup();

			return rc;
		}
		return usage(argv[0]);
}

int cmd_list(int argc, char* argv[])
{
	printf("NOT IMPLEMENTED\n");
	return 0;
}

int cmd_install(int argc, char* argv[])
{
	fpkl_t* pPkgList = get_pkg_list();
	if (!pPkgList)
		return 1;

	int i;
	int rc = 0;
	bool bInteractive = true;
	for (i = 0; i < argc; i ++)
	{
		if (stricmp(argv[i], "--silent") == 0)
		{
			bInteractive = false;
			continue;
		}
		rc = main_install(pPkgList, argv[i], bInteractive);
		if (rc != 0)
			break;
	}

	free(pPkgList);
	return rc;
}

int cmd_remove(int argc, char* argv[])
{
	int i;
	int rc = 0;
	bool bInteractive = true;
	for (i = 0; i < argc; i ++)
	{
		if (stricmp(argv[i], "--silent") == 0)
		{
			bInteractive = false;
			continue;
		}
		rc = main_remove(argv[i], bInteractive);
		if (rc != 0)
			break;
	}

	return rc;
}
