//
// FeOS Standard Library
// feosuser.h
//     FeOS API
//

#pragma once
#include <feos.h>

#define FeOS_GetInstance() FeOS_hInstance
#define CALL_FP(a) ((fp_t)(a))()

void FeOS_DebugPrint(const char*);
void FeOS_WaitForVBlank();

/*
void* FeOS_Malloc(word_t);
void FeOS_Free(void*);
*/

#define FEOS_EP_INIT 0x00000001
#define FEOS_EP_FINI 0x00000002
#define FEOS_EP_MAIN 0x00000100

#define FEOS_RC_OK 0
#define FEOS_RC_ERR 0xFFFFFFFF

/*
#define MAX_PATH 255
#define FEOS_FILEFAIL 0xFFFFFFFF // for GetFileSize, FileGetSize, FileRead, FileWrite and FileSeek

DECL_FEOS_API(file_t, FeOS_OpenFile, (const char*, word_t));
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2
#define O_APPEND 0x0008
#define O_CREAT  0x0200
#define O_TRUNC  0x0400
#define O_MAKEVALID(n) ((n) & (3 | O_APPEND | O_CREAT | O_TRUNC))
DECL_FEOS_API(bool_t, FeOS_IsValidPath, (const char*));
DECL_FEOS_API(word_t, FeOS_GetFileSize, (const char*)); // for filenames
DECL_FEOS_API(bool_t, FeOS_RenameFile, (const char*, const char*));
DECL_FEOS_API(bool_t, FeOS_DeleteFile, (const char*));

DECL_FEOS_API(bool_t, FeOS_FileClose, (file_t));
DECL_FEOS_API(word_t, FeOS_FileGetSize, (file_t)); // for file descriptors
DECL_FEOS_API(word_t, FeOS_FileRead, (file_t, void*, word_t));
DECL_FEOS_API(word_t, FeOS_FileWrite, (file_t, void*, word_t));
DECL_FEOS_API(word_t, FeOS_FileSeek, (file_t, word_t, off_t));
#define FeOS_FileGetPos(file) FeOS_FileSeek(file, SEEK_CUR, 0)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
*/
