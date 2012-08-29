#include "common.h"

#ifdef WIN32
static inline void FixMinGWPath(char* buf)
{
	if (*buf == '/')
	{
		buf[0] = buf[1];
		buf[1] = ':';
	}
}
#endif

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage:\n   %s [manifest] [outputFilename]\n", argv[0]);
		return 0;
	}

#ifdef WIN32
	FixMinGWPath(argv[1]);
	FixMinGWPath(argv[2]);
#endif

	char* manifestString = StringFromFile(argv[1]);
	if (!manifestString)
	{
		fprintf(stderr, "Can't open manifest!\n");
		return 1;
	}
	{
		MetadataParser mp;
		mp.ParseBlob(manifestString);
		free(manifestString);

		FileClass f(argv[2], "wb");
		if (f.openerror())
		{
			fprintf(stderr, "Can't open output file!\n");
			return 1;
		}

		mp.Write(f);
	}

	return 0;
}
