#include <far.h>

#define FAR_MAGIC 0x00524146 // 'FAR\0'

typedef struct
{
	word_t flags;
	word_t nameoff;
	word_t dataoff;
	word_t size;
} FARentry_t;

enum
{
	FAR_EFLAG_USRMASK = 0xFFFFFF00,
	FAR_EFLAG_TYPEMASK = 0xFF,
	
	FAR_ETYPE_FILE = 0,
	FAR_ETYPE_DIR = 1
};

#define FAR_EFLAG_GETUSR(x) ((x)>>8)
#define FAR_EFLAG_GETTYPE(x) ((x)&0xFF)

typedef struct
{
	word_t magic;
	word_t version;
	word_t nentries;
	word_t namesize;
	word_t topentries;
	//FARentry_t topdir[nentries];
} FARheader_t;

//-----------------------------------------------------------------------------
// Read interface
//-----------------------------------------------------------------------------

typedef struct
{
	//void* data;
	int (*read)(void* obj, void* buf, int size);
	int (*seek)(void* obj, int pos, int mode);
	int (*tell)(void* obj);
	void (*close)(void* obj);
} readiface_t;

static int _FILE_read(void* obj, void* buf, int size)
{
	return fread(buf, 1, size, (FILE*)obj);
}

static int _FILE_seek(void* obj, int pos, int mode)
{
	return fseek((FILE*)obj, pos, mode);
}

static int _FILE_tell(void* obj)
{
	return ftell((FILE*)obj);
}

static void _FILE_close(void* obj)
{
	fclose((FILE*)obj);
}

static int _MOD_read(void* obj, void* buf, int size)
{
	return FeOS_ModuleExtraRead(obj, buf, size);
}

static int _MOD_seek(void* obj, int pos, int mode)
{
	return FeOS_ModuleExtraSeek(obj, pos, mode);
}

static int _MOD_tell(void* obj)
{
	return FeOS_ModuleExtraTell(obj);
}

static void _MOD_close(void* obj)
{
}

static const readiface_t FILE_iface = { _FILE_read, _FILE_seek, _FILE_tell, _FILE_close };
static const readiface_t MOD_iface = { _MOD_read, _MOD_seek, _MOD_tell, _MOD_close };

//-----------------------------------------------------------------------------
// Actual code
//-----------------------------------------------------------------------------

typedef struct
{
	const readiface_t* vt;
	void* vt_obj;

	int nentries, ntopentries;
	FARentry_t* entries;
	char* namebuf;
	int namepos;
} FARobj_t;

#define FAR_RawRead(x,a,b) (x)->vt->read((x)->vt_obj,a,b)
#define FAR_RawSeek(x,a,b) (x)->vt->seek((x)->vt_obj,a,b)
#define FAR_RawTell(x)     (x)->vt->tell((x)->vt_obj)
#define FAR_RawClose(x)    (x)->vt->close((x)->vt_obj)

#define F(x) ((FARobj_t*)(x))
#define new_F() F(malloc(sizeof(FARobj_t)))

far_t FAR_OpenCommon(FARobj_t* x)
{
	FARheader_t head;
	FAR_RawRead(x, &head, sizeof(FARheader_t));
	if (head.magic != FAR_MAGIC || head.version != 0)
	{
		FAR_RawClose(x);
		return NULL;
	}

	x->nentries = head.nentries;
	x->ntopentries = head.topentries;

	x->entries = (FARentry_t*) malloc(sizeof(FARentry_t) * head.nentries);
	if (!x->entries)
	{
		FAR_RawClose(x);
		return NULL;
	}

	FAR_RawRead(x, x->entries, sizeof(FARentry_t) * head.nentries);

	x->namebuf = (char*) malloc(head.namesize);
	if (!x->namebuf)
	{
		free(x->entries);
		FAR_RawClose(x);
		return NULL;
	}

	x->namepos = FAR_RawTell(x);
	FAR_RawRead(x, x->namebuf, head.namesize);

	return x;
}

far_t FAR_OpenFile(const char* path)
{
	FILE* f = fopen(path, "rb");
	if (!f) return NULL;
	FARobj_t* x = new_F();
	if (!x) return NULL;
	x->vt = &FILE_iface;
	x->vt_obj = f;
	return FAR_OpenCommon(x);
}

far_t FAR_OpenModule(instance_t hInst)
{
	if (FeOS_ModuleGetExtraSize(hInst) < sizeof(FARheader_t))
		return NULL;
	FARobj_t* x = new_F();
	if (!x) return NULL;
	x->vt = &MOD_iface;
	x->vt_obj = hInst;
	return FAR_OpenCommon(x);
}

void FAR_Close(far_t hArc)
{
	FARobj_t* pArc = F(hArc);
	free(pArc->entries);
	free(pArc->namebuf);
	FAR_RawClose(pArc);
	free(pArc);
}

typedef struct
{
	FARobj_t* a;
	word_t size;
	int base, pos;
} FARfileobj_t;

#define FF(x) ((FARfileobj_t*)(x))
#define new_FF() FF(malloc(sizeof(FARfileobj_t)))

FARentry_t* FindDir(FARobj_t* x, FARentry_t* entArr, int entNum, const char* name)
{
	for(; entNum && FAR_EFLAG_GETTYPE(entArr->flags) == FAR_ETYPE_DIR && strcmp(name, x->namebuf + entArr->nameoff - x->namepos) != 0; entArr ++, entNum --);
	return entNum ? entArr : NULL;
}

FARentry_t* FindFile(FARobj_t* x, FARentry_t* entArr, int entNum, const char* name)
{
	for(; entNum && FAR_EFLAG_GETTYPE(entArr->flags) == FAR_ETYPE_DIR; entArr ++, entNum --);
	if (!entNum) return NULL;
	for(; entNum && strcmp(name, x->namebuf + entArr->nameoff - x->namepos) != 0; entArr ++, entNum --);
	return entNum ? entArr : NULL;
}

farfile_t FAR_GetFile(far_t hArc, const char* path)
{
	FARobj_t* pArc = F(hArc);
	if (*path == '/') path ++;

	FARentry_t* entArr = pArc->entries;
	int entNum = pArc->ntopentries;

	for (;;)
	{
		char* endPos = strchr(path, '/');
		if (endPos)
		{
			// Directory
			int z = endPos - path;
			char buf[z + 1];
			memcpy(buf, path, z); buf[z] = 0;
			FARentry_t* eDir = FindDir(pArc, entArr, entNum, buf);
			if (!eDir) return NULL;
			entArr = (FARentry_t*)((byte_t*)pArc->entries + eDir->dataoff - sizeof(FARheader_t));
			entNum = eDir->size;
			path = endPos + 1;
			continue;
		}

		FARentry_t* eFile = FindFile(pArc, entArr, entNum, path);
		if (!eFile) return NULL;

		FARfileobj_t* pFile = new_FF();
		if (!pFile) return NULL;

		pFile->a = pArc;
		pFile->size = eFile->size;
		pFile->base = eFile->dataoff;
		pFile->pos = 0;
		return pFile;
	}
}

void FAR_FileClose(farfile_t hFile)
{
	//FARfileobj_t* pFile = FF(hFile);
	free(hFile);
}

size_t FAR_FileGetSize(farfile_t hFile)
{
	return FF(hFile)->size;
}

int FAR_FileRead(farfile_t hFile, void* buffer, int size)
{
	FARfileobj_t* pFile = FF(hFile);
	int* pPos = &pFile->pos;
	FAR_RawSeek(pFile->a, pFile->base + *pPos, SEEK_SET);
	int diff = pFile->size - *pPos;
	if (size > diff) size = diff;
	int rc = FAR_RawRead(pFile->a, buffer, size);
	*pPos += rc;
	return rc;
}

int FAR_FileSeek(farfile_t hFile, int pos, int mode)
{
	FARfileobj_t* pFile = FF(hFile);
	int size = pFile->size;
	switch (mode)
	{
		case SEEK_SET:
			break;
		case SEEK_CUR:
			pos += pFile->pos;
			break;
		case SEEK_END:
			pos += pFile->size;
			break;
	}
	if (pos > size) pos = size;
	if (pos < 0) pos = 0;
	pFile->pos = pos;
	return pos;
}

int FAR_FileTell(farfile_t hFile)
{
	return FF(hFile)->pos;
}

const stream_t FARFILE_stmown = { NULL, FAR_FileClose, NULL, (ssize_t(*)(void*,char*,size_t))FAR_FileRead, FAR_FileSeek };
const stream_t FARFILE_stm = { NULL, NULL, NULL, (ssize_t(*)(void*,char*,size_t))FAR_FileRead, FAR_FileSeek };

FILE* FAR_WrapFile(farfile_t hFile, bool bOwn)
{
	return FeOS_OpenStream(bOwn ? &FARFILE_stmown : &FARFILE_stm, hFile);
}
