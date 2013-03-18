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

/*! \file far.h
	\brief FeOS ARchive library.
*/

typedef void* far_t; //!< FAR handle datatype
typedef void* farfile_t; //!< FAR file handle datatype

//! \brief FAR content enumeration return code
enum { FAR_ENUM_CONTINUE = 0, FAR_ENUM_BREAK = 1 };
//! \brief FAR content enumeration callback type
typedef int (* farEnumCb)(void* userData, far_t hFar, const char* path, bool isDir);

//! \brief Opens an archive.
LIBFAR_API far_t FAR_OpenArchive(const char* path);
//! \brief Opens an archive that is embedded in a module.
LIBFAR_API far_t FAR_OpenModule(instance_t hInst);
//! \brief Closes an archive.
LIBFAR_API void FAR_Close(far_t hArc);
//! \brief Opens the archive embedded in the caller's module.
#define FAR_OpenSelf() FAR_OpenModule(FeOS_GetInstance())

//! \brief Opens a file in the archive.
LIBFAR_API farfile_t FAR_GetFile(far_t hArc, const char* path);
//! \brief Retrieves the size of a file.
LIBFAR_API size_t FAR_FileGetSize(farfile_t hFile);
//! \brief Reads data from a file.
LIBFAR_API int FAR_FileRead(farfile_t hFile, void* buffer, int size);
//! \brief Moves the read pointer in a file.
LIBFAR_API int FAR_FileSeek(farfile_t hFile, int pos, int mode);
//! \brief Retrieves the position of a file's read pointer.
LIBFAR_API int FAR_FileTell(farfile_t hFile);
//! \brief Closes a file.
LIBFAR_API void FAR_FileClose(farfile_t hFile);
//! \brief Wraps a file handle into a C stream object.
//! \param bOwn Specifies whether fclose() should also close the underlying archive file handle.
LIBFAR_API FILE* FAR_WrapFile(farfile_t hFile, bool bOwn);

//! \brief Enumerates the contents of an archive.
LIBFAR_API int FAR_EnumFiles(far_t hArc, farEnumCb callback, void* userData);

//! \brief Opens a file in the archive, returning a C stream object.
static inline FILE* FAR_OpenArcFile(far_t hArc, const char* path)
{
	farfile_t hFile = FAR_GetFile(hArc, path);
	if (!hFile) return NULL;
	return FAR_WrapFile(hFile, true);
}

//! \brief Loads the contents of a file in the archive.
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
