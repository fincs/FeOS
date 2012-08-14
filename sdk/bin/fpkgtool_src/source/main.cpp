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

static inline int bad_format()
{
	fprintf(stderr, "Bad stdin format\n");
	return 1;
}

static inline void skipString(FileClass& f)
{
	while (f.ReadByte());
}

// Code based on http://atariarea.krap.pl/x-asm/inflate.html
int GzipToDeflate(FileClass& f, int otherPos)
{
	FileClass inf(stdin);
	if (inf.ReadByte() == 0x1F)
	{
		if (inf.ReadByte() == 0x8B)
		{
			if (inf.ReadByte() != 8)
				return bad_format();
		}else return bad_format();
	}else return bad_format();

	int flg = inf.ReadByte();
	inf.Skip(6);
	if (flg & 4)
		inf.Skip(inf.ReadHword());
	if (flg & 8)
		skipString(inf);
	if (flg & 16)
		skipString(inf);
	if (flg & 2)
		inf.Skip(2);

	// Copy everything except the last 8 bytes
	char buf[8 + 4096];
	int len = 0;
	int r = f.Tell();
	for (;;)
	{
		len += inf._RawRead(buf + len, sizeof(buf) - len);
		if (len != sizeof(buf))
			break;
		f.WriteRaw(buf, sizeof(buf) - 8);
		memcpy(buf, buf + sizeof(buf) - 8, 8);
		len = 8;
	}
	if (len < 8)
	{
		fprintf(stderr, "Unexpected end of stream!\n");
		return 1;
	}
	f.WriteRaw(buf, len - 8);
	int q = f.Tell();
	r = q - r;
	word_t uncompSize = eswap_word(*(word_t*)(buf + len-8 + 4)); // find size
	f.Seek(otherPos);
	f.WriteWord(r);
	f.WriteWord(uncompSize);
	f.Seek(q);
	return 0;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage:\n   %s [manifest] [outputFilename]\n", argv[0]);
		return 0;
	}

#ifdef WIN32
	_setmode(_fileno(stdin), _O_BINARY);
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
		mp.Add("FeOS-Version", FEOS_VERSION_TEXT);
		mp.ParseBlob(manifestString);
		free(manifestString);

		FileClass f(argv[2], "wb+");
		if (f.openerror())
		{
			fprintf(stderr, "Can't open output file!\n");
			return 1;
		}

		f.WriteWord(0x474B5046); // FPKG
		f.WriteWord(0); // version 0

		f.WriteWord(0x4154454D); // META
		f.WriteWord(mp.Size());
		mp.Write(f);

		f.WriteWord(0x41544144); // DATA
		int pos = f.Tell();
		f.WriteWord(0); // compressed size placeholder
		f.WriteWord(0); // uncompressed size placeholder

		int rc = GzipToDeflate(f, pos);
		if (rc)
			return rc;
	}

	return 0;
}
