#include "common.h"

typedef struct
{
	int keyCount;
	union
	{
		struct
		{
			int keyPos;
			int valPos;
		} keys[0];
		char buf[0];
	};
} metadata_t;

const char* metadata_read(metadata_t* pMetadata, const char* key);
