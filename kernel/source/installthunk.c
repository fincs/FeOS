#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/iosupport.h>
#include <nds.h>

#define DECLAREHOOK(ret, name, prm) \
	extern ret (*_##name##hook_addr) prm; \
	extern ret _##name##hook prm

#define HOOK(element, name) do { \
	_##name##hook_addr = element; \
	element = _##name##hook; \
}while(0)

// FAT hooks
DECLAREHOOK(int, fatopen, (struct _reent*, void*, const char*, int, int));
DECLAREHOOK(int, fatclose, (struct _reent*, int));
DECLAREHOOK(ssize_t, fatwrite, (struct _reent*, int, const char *ptr, size_t));
DECLAREHOOK(ssize_t, fatread, (struct _reent*, int, char *ptr, size_t));
DECLAREHOOK(off_t, fatseek, (struct _reent*, int, off_t, int));
DECLAREHOOK(int, fatfstat, (struct _reent*, int, struct stat*));
DECLAREHOOK(int, fatstat, (struct _reent*, const char*, struct stat*));
DECLAREHOOK(int, fatlink, (struct _reent*, const char*, const char*));
DECLAREHOOK(int, fatunlink, (struct _reent*, const char*));
DECLAREHOOK(int, fatchdir, (struct _reent*, const char*));
DECLAREHOOK(int, fatrename, (struct _reent*, const char*, const char*));
DECLAREHOOK(int, fatmkdir, (struct _reent*, const char*, int));
DECLAREHOOK(DIR_ITER*, fatdiropen, (struct _reent*, DIR_ITER*, const char*));
DECLAREHOOK(int, fatdirreset, (struct _reent*, DIR_ITER*));
DECLAREHOOK(int, fatdirnext, (struct _reent*, DIR_ITER*, char*, struct stat*));
DECLAREHOOK(int, fatdirclose, (struct _reent*, DIR_ITER*));
DECLAREHOOK(int, fatstatvfs, (struct _reent*, const char*, struct statvfs*));
DECLAREHOOK(int, fatftruncate, (struct _reent*, int, off_t));
DECLAREHOOK(int, fatfsync, (struct _reent*, int));

ssize_t DSKeybdRead(struct _reent*, int, char*, size_t);

static ssize_t DummyRead(struct _reent* r, int fd, char* buf, size_t count)
{
	memset(buf, 0, count);
	return count;
}

static ssize_t DummyWrite(struct _reent* r, int fd, const char* buf, size_t count)
{
	nocashWrite(buf, count);
	return count;
}

ssize_t (* _conout)(struct _reent*, int, const char *ptr, size_t);
ssize_t (* _conerr)(struct _reent*, int, const char *ptr, size_t);

void IoRestoreStdStreams()
{
	devoptab_t** dotabs = (devoptab_t**) devoptab_list; // force non-constness
	dotabs[STD_OUT]->write_r = _conout;
	dotabs[STD_IN]->read_r = DSKeybdRead;
	dotabs[STD_ERR]->write_r = _conerr;
}

void IoMothballStdStreams()
{
	devoptab_t** dotabs = (devoptab_t**) devoptab_list; // force non-constness
	dotabs[STD_OUT]->write_r = DummyWrite;
	dotabs[STD_IN]->read_r = DummyRead;
	dotabs[STD_ERR]->write_r = DummyWrite;
}

void IoInstallThunks()
{
	devoptab_t** dotabs = (devoptab_t**) devoptab_list; // force non-constness
	_conout = dotabs[STD_OUT]->write_r;
	_conerr = dotabs[STD_ERR]->write_r;
	dotabs[STD_IN]->read_r = DSKeybdRead;

	devoptab_t* dotab = (devoptab_t*) GetDeviceOpTab("/"); // force non-constness
	HOOK(dotab->open_r, fatopen);
	HOOK(dotab->close_r, fatclose);
	HOOK(dotab->write_r, fatwrite);
	HOOK(dotab->read_r, fatread);
	HOOK(dotab->seek_r, fatseek);
	HOOK(dotab->fstat_r, fatfstat);
	HOOK(dotab->stat_r, fatstat);
	HOOK(dotab->link_r, fatlink);
	HOOK(dotab->unlink_r, fatunlink);
	HOOK(dotab->chdir_r, fatchdir);
	HOOK(dotab->rename_r, fatrename);
	HOOK(dotab->mkdir_r, fatmkdir);
	HOOK(dotab->diropen_r, fatdiropen);
	HOOK(dotab->dirreset_r, fatdirreset);
	HOOK(dotab->dirnext_r, fatdirnext);
	HOOK(dotab->dirclose_r, fatdirclose);
	HOOK(dotab->statvfs_r, fatstatvfs);
	HOOK(dotab->ftruncate_r, fatftruncate);
	HOOK(dotab->fsync_r, fatfsync);
}
