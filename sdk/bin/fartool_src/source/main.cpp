#include "farbuild.h"

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
		fprintf(stderr, "Usage:\n   %s [folderName] [filename]\n", argv[0]);
		return 0;
	}

#ifdef WIN32
	FixMinGWPath(argv[1]);
	FixMinGWPath(argv[2]);
#endif

	FAR_Builder oBuilder;
	safe_call(oBuilder.ScanDir(argv[1]));
	safe_call(oBuilder.OutputFile(argv[2]));
	return 0;
}
