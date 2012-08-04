#include "common.h"

typedef struct
{
	const char* name;
	metadata_t* metadata;
	size_t metadataSize;
	byte_t md5[16];
} fpkl_pkginfo_t;

typedef union
{
	struct
	{
		char magic[4];
		word_t version;
		int packageCount;
		struct
		{
			word_t namePointer;
			word_t metadataPointer;
			word_t metadataSize;
			byte_t md5[16];
		} packages[0];
	};
	char raw[0];
} fpkl_t;

void fpkl_getbyid(fpkl_pkginfo_t* pInfo, fpkl_t* pPkgList, int id);
bool fpkl_getbyname(fpkl_pkginfo_t* pInfo, fpkl_t* pPkgList, const char* name);
