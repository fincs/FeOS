#include "common.h"

static bool get_cur_pkgtime(time_t* pTime)
{
	FILE* f = fopen_web("/fpkg/time", NULL);
	if (!f)
	{
		fprintf(stderr, "Cannot retrieve server pkg time!\n");
		return false;
	}

	fscanf(f, "%u", pTime);
	fclose(f);

	return true;
}

static FILE* open_fpkl()
{
	if (system("mkdir -p /data/FeOS/FPM") != 0)
		return NULL;

	time_t srvTime;
	if (!get_cur_pkgtime(&srvTime))
		return NULL;

	time_t localTime = 0;

	do
	{
		FILE* f = fopen("/data/FeOS/FPM/time", "rb");
		if (!f) break;

		fread(&localTime, sizeof(localTime), 1, f);
		fclose(f);
	} while(0);

	if (srvTime > localTime)
	{
		// Need to upgrade!
		printf("Downloading package list...\n");
		if (!download("/fpkg/", "/data/FeOS/FPM/packages.fpkl", NULL))
			return NULL;
		FILE* f = fopen("/data/FeOS/FPM/time", "wb");
		if (!f)
			return NULL;
		fwrite(&srvTime, sizeof(srvTime), 1, f);
		fclose(f);
	}

	return fopen("/data/FeOS/FPM/packages.fpkl", "rb");
}

fpkl_t* get_pkg_list()
{
	FILE* f = open_fpkl();
	if (!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);

	fpkl_t* pPkgList = (fpkl_t*) malloc(size);
	if (!pPkgList)
	{
		fclose(f);
		fprintf(stderr, "Out of memory!\n");
		return NULL;
	}

	fread(pPkgList, 1, size, f);
	fclose(f);
	return pPkgList;
}
