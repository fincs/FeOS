#include "farbuild.h"

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage:\n   %s [folderName] [filename]\n", argv[0]);
		return 0;
	}

#ifdef WIN32
	if(*argv[1] == '/')
	{
		// Fix MinGW paths.
		argv[1][0] = argv[1][1];
		argv[1][1] = ':';
	}
#endif

	FAR_Builder oBuilder;
	safe_call(oBuilder.ScanDir(argv[1]));
	safe_call(oBuilder.OutputFile(argv[2]));
	return 0;
}
