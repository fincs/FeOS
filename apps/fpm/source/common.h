#pragma once
#include <feos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <md5.h>
#include <far.h>

#include "metadata.h"
#include "fpkg.h"
#include "fpkl.h"

#define PACKAGE_SERVER_NAME "feos.mtheall.com"
#define USER_AGENT "FeOS/FPM"

FILE* fopentcp(struct sockaddr_in* addr);
struct sockaddr_in* get_pkg_server_addr();
FILE* fopen_web(const char* path, int* length);

typedef void (* dlCallback)(const void* data, int size);
bool download(const char* srvPath, const char* localPath, dlCallback cb);
void progressbar(int pos, int size);

fpkl_t* get_pkg_list();
int main_install(fpkl_t* pPkgList, const char* pkgName, bool bInteractive);
int main_remove(const char* pkgName, bool bInteractive);

static inline bool my_isspace(int x)
{
	return x == ' ' || x == '\t';
}

int decompress_fpkg(const char* fileName, const char* outFile);
