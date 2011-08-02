#include "feos.h"
#include "fxe.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/iosupport.h>
#include <errno.h>

typedef struct
{
	size_t  CustomSize;
	int     (*Open) (void*);
	void    (*Close)(void*);
	ssize_t (*Write)(void*, const char*, size_t);
	ssize_t (*Read) (void*, char*, size_t);
	off_t   (*Seek) (void*, off_t, int);
} stream_t;

typedef struct
{
	const stream_t* pStm;
	void* cData;
} stmfile_t;

static int stm_open_r(struct _reent*, void*, const char*, int, int);
static int stm_close_r(struct _reent*, int);
static ssize_t stm_write_r(struct _reent*, int, const char*, size_t);
static ssize_t stm_read_r(struct _reent*, int, char*, size_t);
static off_t stm_seek_r(struct _reent*, int, off_t, int);

static const devoptab_t dotab_stm =
{
	"stm",
	sizeof(stmfile_t),
	stm_open_r,
	stm_close_r,
	stm_write_r,
	stm_read_r,
	stm_seek_r,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

void FeOS_InitStreams()
{
	AddDevice(&dotab_stm);
}

FILE* FeOS_OpenStream(const stream_t* pStm)
{
	char buf[14];
	siprintf(buf, "stm:/%08X", (word_t) pStm);
	return fopen(buf, "r+");
}

int stm_open_r(struct _reent* r, void* fileStruct, const char* path, int flags, int mode)
{
	stmfile_t* sFile = (stmfile_t*) fileStruct;

	// Skip the stm: if present
	if(strchr(path, ':') != NULL)
		path = strchr (path, ':') + 1;
	if(strchr(path, ':') != NULL)
	{
		r->_errno = EINVAL;
		return -1;
	}

	word_t d = strtoul(path+1, NULL, 16);
	if (!d)
	{
		r->_errno = EINVAL;
		return -1;
	}

	const stream_t* pStm = (const stream_t*) d;

	sFile->pStm = pStm;
	sFile->cData = NULL;

	if (pStm->CustomSize)
	{
		sFile->cData = malloc(pStm->CustomSize);
		if (!sFile->cData)
		{
			r->_errno = ENOMEM;
			return -1;
		}
	}

	if (pStm->Open)
		r->_errno = pStm->Open(sFile->cData);

	return 0;
}

int stm_close_r(struct _reent* r, int fd)
{
	stmfile_t* sFile = (stmfile_t*) fd;
	const stream_t* pStm = sFile->pStm;

	if (pStm->Close)
		pStm->Close(sFile->cData);

	if (sFile->cData)
		free(sFile->cData);

	return 0;
}

ssize_t stm_write_r(struct _reent* r, int fd, const char* ptr, size_t len)
{
	stmfile_t* sFile = (stmfile_t*) fd;
	const stream_t* pStm = sFile->pStm;

	if (!pStm->Write)
	{
		r->_errno = EBADF;
		return -1;
	}

	return pStm->Write(sFile->cData, ptr, len);
}

ssize_t stm_read_r(struct _reent* r, int fd, char* ptr, size_t len)
{
	stmfile_t* sFile = (stmfile_t*) fd;
	const stream_t* pStm = sFile->pStm;

	if (!pStm->Read)
	{
		r->_errno = EBADF;
		return -1;
	}

	return pStm->Read(sFile->cData, ptr, len);
}

off_t stm_seek_r(struct _reent* r, int fd, off_t pos, int dir)
{
	stmfile_t* sFile = (stmfile_t*) fd;
	const stream_t* pStm = sFile->pStm;

	if (!pStm->Seek)
	{
		r->_errno = EBADF;
		return -1;
	}

	return pStm->Seek(sFile->cData, pos, dir);
}
