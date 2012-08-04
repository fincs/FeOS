#include "common.h"

void progressbar(int pos, int size)
{
	printf("\x1b[s[");
	int parts = (pos*24)/size;
	int perct = (pos*100)/size;
	int i;
	for (i = 0; i < parts; i ++)
		putchar('=');
	for (i = 0; i < (24 - parts); i ++)
		putchar(' ');
	printf("] %d%%\x1b[u", perct);
}

static void copyFromTo(FILE* fout, FILE* fin, int size, dlCallback cb)
{
	char aBuf[1024];
	int curSize = size;
	while (curSize)
	{
		int chunk = (curSize > sizeof(aBuf)) ? sizeof(aBuf) : curSize;
		progressbar(size - curSize, size);
		fread(aBuf, 1, chunk, fin);
		if (cb)
			cb(aBuf, chunk);
		fwrite(aBuf, 1, chunk, fout);
		curSize -= chunk;
	}
	//printf("Download complete              \n");
	progressbar(size, size);
	printf("\n\n");
}

bool download(const char* srvPath, const char* localPath, dlCallback cb)
{
	FILE* fout = fopen(localPath, "wb");
	if (!fout)
	{
		fprintf(stderr, "Can't open %s!\n", localPath);
		return false;
	}

	int size;
	FILE* fin = fopen_web(srvPath, &size);
	if (!fin)
	{
		fclose(fout);
		fprintf(stderr, "Can't download %s!\n", srvPath);
		return false;
	}
	if (size == -1)
	{
		fclose(fout);
		fclose(fin);
		fprintf(stderr, "Unknown download size!\n");
		return false;
	}
	copyFromTo(fout, fin, size, cb);
	fclose(fout);
	fclose(fin);
	return true;
}
