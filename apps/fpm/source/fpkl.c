#include "common.h"

void fpkl_getbyid(fpkl_pkginfo_t* pInfo, fpkl_t* pPkgList, int id)
{
	pInfo->name = pPkgList->raw + pPkgList->packages[id].namePointer;
	pInfo->metadata = (metadata_t*)(pPkgList->raw + pPkgList->packages[id].metadataPointer);
	pInfo->metadataSize = pPkgList->packages[id].metadataSize;
	memcpy(pInfo->md5, pPkgList->packages[id].md5, 16);
}

bool fpkl_getbyname(fpkl_pkginfo_t* pInfo, fpkl_t* pPkgList, const char* name)
{
	int min = 0;
	int max = pPkgList->packageCount;
	while (max >= min)
	{
		int mid = (min + max) / 2;
		int rc = strcmp(pPkgList->raw + pPkgList->packages[mid].namePointer, name);
		if (rc < 0)
			min = mid + 1;
		else if (rc > 0)
			max = mid - 1;
		else
		{
			fpkl_getbyid(pInfo, pPkgList, mid);
			return true;
		}
	}
	return false;
}
