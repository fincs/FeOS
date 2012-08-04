#include "common.h"

FILE* fopen_web(const char* path, int* length)
{
	char aBuf[1024];

	FILE* f = fopentcp(get_pkg_server_addr());
	if (!f) return NULL;

	fprintf(f, "GET %s HTTP/1.1\r\n"
		"Host: " PACKAGE_SERVER_NAME "\r\n"
		"User-Agent: " USER_AGENT "\r\n"
		"\r\n", path);

	fgets(aBuf, sizeof(aBuf), f);
	if (!strstr(aBuf, "200 OK"))
	{
		fclose(f);
		return NULL;
	}

	if (length)
		*length = -1; // means "not known"

	// Skip HTTP headers
	for (;;)
	{
		fgets(aBuf, sizeof(aBuf), f);
		if (strcmp(aBuf, "\r\n") == 0)
			break;
		// If the server cares to specify it, the
		// length variable should get eventually filled
		if (length) sscanf(aBuf, "Content-Length: %d", length);
	}
	
	return f;
}
