#include "common.h"

int parse_feosversion(const char* str)
{
	int maj, min;
	sscanf(str, "%d.%d", &maj, &min);
	return FEOS_VPACK_MAKE(maj, min);
}

metadata_t* get_installed_meta(const char* pkgName)
{
	char tempbuf[256];
	snprintf(tempbuf, sizeof(tempbuf), "/data/FeOS/FPM/%s.meta", pkgName);
	FILE* f = fopen(tempbuf, "rb");
	if (!f)
		return NULL;
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	metadata_t* meta = (metadata_t*) malloc(size);
	if (!meta)
	{
		fclose(f);
		return NULL;
	}
	fread(meta, 1, size, f);
	fclose(f);
	return meta;
}

void get_installed_pkgver(char* buf, int buflen, const char* pkgName)
{
	metadata_t* meta = get_installed_meta(pkgName);
	if (!meta)
	{
		*buf = 0;
		return;
	}

	strncpy(buf, metadata_read(meta, "version"), buflen);
	free(meta);
}

static void chop_trailing_spaces(char* buf)
{
	char* pos;
	for (pos = buf + strlen(buf) - 1; pos >= buf && my_isspace(*pos); pos --)
		*pos = 0;
}

int install_dep(fpkl_t* pPkgList, const char* pkgName, const char* parenVer)
{
	if (parenVer)
		printf("Dependency: %s >= %s\n", pkgName, parenVer);
	else
		printf("Dependency: %s\n", pkgName);

	return main_install(pPkgList, pkgName, false);
}

int get_dependencies(fpkl_t* pPkgList, const char* cdepends)
{
	char* depends = strdup(cdepends);
	if (!depends)
	{
		fprintf(stderr, "Out of memory!\n");
		return 1;
	}

	char* pos = depends;
	for (;;)
	{
		char* endPos = strchr(pos, ',');
		if (endPos)
			*endPos = 0;

		while (my_isspace(*pos)) pos ++;

		char* parenVer = NULL;
		do
		{
			char* parenPos = strstr(pos, "(>=");
			if (!parenPos)
				break;
			*parenPos = 0;
			parenPos += 3;
			char* endParen = strchr(parenPos, ')');
			*endParen = 0;

			parenVer = parenPos;
			while (my_isspace(*parenVer)) parenVer ++;
			chop_trailing_spaces(parenVer);
		} while(0);

		chop_trailing_spaces(pos);

		int rc = install_dep(pPkgList, pos, parenVer);
		if (rc != 0)
			return rc;

		if (!endPos)
			break;
		pos = endPos + 1;
	}
	
	free(depends);
	return 0;
}

int installPackage(const char* pkgName, metadata_t* meta, byte_t* md5, int metaSize);

int main_install(fpkl_t* pPkgList, const char* pkgName, bool bInteractive)
{
	fpkl_pkginfo_t info;
	if (!fpkl_getbyname(&info, pPkgList, pkgName))
	{
		fprintf(stderr, "Package '%s' doesn't exist!\n", pkgName);
		return 1;
	}

	metadata_t* meta = info.metadata;
	const char* pkgVer = metadata_read(meta, "version");
	{
		const char* min_feosver = metadata_read(meta, "feos-version");
		if (parse_feosversion(min_feosver) > FeOS_GetVersion())
		{
			fprintf(stderr, "Package '%s' requires at least FeOS v%s!\n", pkgName, min_feosver);
			return 1;
		}
	}

	do
	{
		char verbuf[256];
		get_installed_pkgver(verbuf, sizeof(verbuf), pkgName);
		if (!*verbuf)
			break; // not installed or error

		if (strcmp(verbuf, pkgVer) == 0)
		{
			printf("%s up to date (%s)\n", pkgName, pkgVer);
			return 0;
		}

		printf("%s is out of date.\n", pkgName);
	} while (0);

	if (bInteractive)
	{
		printf("%s (%s)\nAuthor: %s\n\n%s\n\nDo you wish to install it?\nA = OK, B = Cancel\n",
			metadata_read(meta, "title"),
			pkgVer,
			metadata_read(meta, "author"),
			metadata_read(meta, "description"));

		for (;;)
		{
			swiWaitForVBlank();
			word_t kDown = keysDown();
			if (kDown & KEY_A)
				break;
			if (kDown & KEY_B)
				return 0;
		}
	}

	const char* depends = metadata_read(meta, "depends");
	if (depends)
	{
		printf("Checking dependencies...\n");
		int rc = get_dependencies(pPkgList, depends);
		if (rc != 0)
			return rc;
	}

	return installPackage(pkgName, meta, info.md5, info.metadataSize);
}

MD5_CTX md5Ctx;

static void md5Callback(const void* buf, int size);

#define DOWNLOAD_TMPNAME "/data/FeOS/FPM/~download.tmp"
#define FAR_TMPNAME "/data/FeOS/FPM/~install.far"

static int farExtract(void* userData, far_t hFar, const char* path, bool isDir);

int installPackage(const char* pkgName, metadata_t* meta, byte_t* md5, int metaSize)
{
	{
		byte_t dl_md5[16];
		{
			char aBuf[256];
			snprintf(aBuf, sizeof(aBuf), "/fpkg/%s.fpkg", pkgName);
			printf("Downloading %s...\n", pkgName);
			MD5_Init(&md5Ctx);
			if (!download(aBuf, DOWNLOAD_TMPNAME, md5Callback))
				return 1;
			MD5_Final(dl_md5, &md5Ctx);
		}

		if (memcmp(dl_md5, md5, 16) != 0)
		{
			remove(DOWNLOAD_TMPNAME);
			fprintf(stderr, "Corrupted download!\n");
			return 1;
		}
	}

	printf("Decompressing %s...\nThis may take a while...\n\n", pkgName);
	int rc = decompress_fpkg(DOWNLOAD_TMPNAME, FAR_TMPNAME);
	remove(DOWNLOAD_TMPNAME);

	if (rc != 0)
	{
		fprintf(stderr, "Unpacking error!\n");
		return 1;
	}

	const char* cmdstr;
	cmdstr = metadata_read(meta, "pre-install");
	if (cmdstr)
	{
		printf("Running pre-install hook...\n");
		if (system(cmdstr) != 0)
		{
			remove(FAR_TMPNAME);
			fprintf(stderr, "Command fail!\n");
			return 1;
		}
	}

	printf("Extracting files...\n");
	far_t hFar = FAR_OpenArchive(FAR_TMPNAME);
	if (!hFar)
	{
		remove(FAR_TMPNAME);
		fprintf(stderr, "Unable to open archive!\n");
		return 1;
	}

	FILE* f;
	{
		char aBuf[256];
		snprintf(aBuf, sizeof(aBuf), "/data/FeOS/FPM/%s-uninstall.cmd", pkgName);
		f = fopen(aBuf, "w");
	}
	if (!f)
	{
		FAR_Close(hFar);
		remove(FAR_TMPNAME);
		fprintf(stderr, "Can't create uninst script!\n");
		return 1;
	}

	cmdstr = metadata_read(meta, "pre-uninstall");
	if (cmdstr)
		fprintf(f, "%s\n", cmdstr);

	rc = FAR_EnumFiles(hFar, farExtract, f);
	FAR_Close(hFar);
	remove(FAR_TMPNAME);

	if (rc == FAR_ENUM_BREAK)
	{
		fclose(f);
		fprintf(stderr, "Unable to extract files!\n");
		return 1;
	}

	cmdstr = metadata_read(meta, "post-uninstall");
	if (cmdstr)
		fprintf(f, "%s\n", cmdstr);

	fclose(f);

	{
		char aBuf[256];
		snprintf(aBuf, sizeof(aBuf), "/data/FeOS/FPM/%s.meta", pkgName);
		f = fopen(aBuf, "wb");
	}
	if (!f)
	{
		fprintf(stderr, "Cannot write package info!\n");
		return 1;
	}
	fwrite(meta, 1, metaSize, f);
	fclose(f);

	cmdstr = metadata_read(meta, "post-install");
	if (cmdstr)
	{
		printf("Running post-install hook...\n");
		if (system(cmdstr) != 0)
		{
			fprintf(stderr, "Command fail!\n");
			return 1;
		}
	}

	printf("Package successfully installed!\n");

	return 0;
}

byte_t farExtBuf[2048];

int farExtract(void* userData, far_t hFar, const char* path, bool isDir)
{
	FILE* f = (FILE*) userData;
	printf("%s\n", path);
	if (isDir)
	{
		char aBuf[256];
		snprintf(aBuf, sizeof(aBuf), "mkdir -p \"%s\"", path);
		if (system(aBuf) != 0)
			return FAR_ENUM_BREAK;
	} else
	{
		// File copy!
		farfile_t hFile = FAR_GetFile(hFar, path);
		if (!hFile) return FAR_ENUM_BREAK;

		int size = FAR_FileGetSize(hFile);

		FILE* fout = fopen(path, "wb");
		if (!fout)
		{
			FAR_FileClose(hFile);
			return FAR_ENUM_BREAK;
		}

		bool bSucceeded = true;

		int remaining = size;
		while (remaining)
		{
			progressbar(size - remaining, size);
			int read = FAR_FileRead(hFile, farExtBuf, sizeof(farExtBuf));
			if (read == 0)
			{
				bSucceeded = false;
				break;
			}
			fwrite(farExtBuf, 1, read, fout);
			remaining -= read;
		}

		progressbar(size - remaining, size);
		printf("\n\n");

		FAR_FileClose(hFile);
		fclose(fout);

		fprintf(f, "rm \"%s\"\n", path);

		if (!bSucceeded)
			return FAR_ENUM_BREAK;
	}
	
	return FAR_ENUM_CONTINUE;
}

void md5Callback(const void* buf, int size)
{
	MD5_Update(&md5Ctx, buf, size);
}

int main_remove(const char* pkgName, bool bInteractive)
{
	struct stat st;
	char aBuf[256];
	snprintf(aBuf, sizeof(aBuf), "/data/FeOS/FPM/%s.meta", pkgName);
	if (stat(aBuf, &st) < 0)
	{
		fprintf(stderr, "Package %s doesn't exist!\n", pkgName);
		return 1;
	}

	if (bInteractive)
	{
		fprintf(stderr, "Do you want to remove %s?\nA = yes, B = no\n", pkgName);
		for (;;)
		{
			swiWaitForVBlank();
			word_t kDown = keysDown();
			if (kDown & KEY_A)
				break;
			if (kDown & KEY_B)
				return 0;
		}
	}

	snprintf(aBuf, sizeof(aBuf), "cmd \"/data/FeOS/FPM/%s-uninstall.cmd\"", pkgName);
	printf("Removing %s...\n", pkgName);
	if (system(aBuf) != 0)
	{
		fprintf(stderr, "Could not remove package!\n");
		return 1;
	}

	snprintf(aBuf, sizeof(aBuf), "/data/FeOS/FPM/%s-uninstall.cmd", pkgName);
	remove(aBuf);
	snprintf(aBuf, sizeof(aBuf), "/data/FeOS/FPM/%s.meta", pkgName);
	remove(aBuf);

	printf("%s successfully removed!\n", pkgName);
	return 0;
}
