#include "common.h"

// Binary search
static const char* metadata_find(metadata_t* pMetadata, const char* key, int min, int max)
{
	while (max >= min)
	{
		int mid = (min + max) / 2;
		int rc = strcmp(pMetadata->buf + pMetadata->keys[mid].keyPos, key);
		if (rc < 0)
			min = mid + 1;
		else if (rc > 0)
			max = mid - 1;
		else
			return pMetadata->buf + pMetadata->keys[mid].valPos;
	}
	return NULL;
}

const char* metadata_read(metadata_t* pMetadata, const char* key)
{
	return metadata_find(pMetadata, key, 0, pMetadata->keyCount - 1);
}
