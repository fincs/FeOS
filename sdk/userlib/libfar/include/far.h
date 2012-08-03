#pragma once
#include <feos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBFAR_BUILD
#define LIBFAR_API FEOS_EXPORT
#else
#define LIBFAR_API
#endif

typedef void* far_t;
typedef void* farfile_t;

enum { FAR_ENUM_CONTINUE = 0, FAR_ENUM_BREAK = 1 };
typedef int (* farEnumCb)(void* userData, far_t hFar, const char* path, bool isDir);

LIBFAR_API far_t FAR_OpenArchive(const char* path);
LIBFAR_API far_t FAR_OpenModule(instance_t hInst);
LIBFAR_API void FAR_Close(far_t hArc);
#define FAR_OpenSelf() FAR_OpenModule(FeOS_GetInstance())

LIBFAR_API farfile_t FAR_GetFile(far_t hArc, const char* path);
LIBFAR_API size_t FAR_FileGetSize(farfile_t hFile);
LIBFAR_API int FAR_FileRead(farfile_t hFile, void* buffer, int size);
LIBFAR_API int FAR_FileSeek(farfile_t hFile, int pos, int mode);
LIBFAR_API int FAR_FileTell(farfile_t hFile);
LIBFAR_API void FAR_FileClose(farfile_t hFile);
LIBFAR_API FILE* FAR_WrapFile(farfile_t hFile, bool bOwn);

LIBFAR_API int FAR_EnumFiles(far_t hArc, farEnumCb callback, void* userData);

static inline FILE* FAR_OpenArcFile(far_t hArc, const char* path)
{
	farfile_t hFile = FAR_GetFile(hArc, path);
	if (!hFile) return NULL;
	return FAR_WrapFile(hFile, true);
}

static inline void* FAR_LoadFile(far_t hArc, const char* path)
{
	farfile_t hFile = FAR_GetFile(hArc, path);
	if (!hFile) return NULL;
	size_t size;
	void* mem = malloc(size = FAR_FileGetSize(hFile));
	if (!mem)
	{
		FAR_FileClose(hFile);
		return NULL;
	}
	FAR_FileRead(hFile, mem, size);
	FAR_FileClose(hFile);
	return mem;
}

#ifdef __cplusplus
}
#endif
