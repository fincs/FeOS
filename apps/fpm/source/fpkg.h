#include "common.h"

typedef struct
{
	char magic[4];
	word_t version;
} fpkg_header_t;

typedef struct
{
	char magic[4];
	word_t size;
} fpkg_chunk_t;
