#include "common.h"

static struct sockaddr_in addr;
static bool bDidInit = false;

struct sockaddr_in* get_pkg_server_addr()
{
	if (!bDidInit)
	{
		struct hostent* host = gethostbyname(PACKAGE_SERVER_NAME);
		if (!host)
			return NULL;

		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);
		addr.sin_addr.s_addr = *(word_t*)(host->h_addr_list[0]);
		bDidInit = true;
	}
	return &addr;
}
