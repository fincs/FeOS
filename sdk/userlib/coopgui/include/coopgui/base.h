#pragma once

#define COOPGUI_GUIMANAGER_VER 1

// Strongly typed enums that can be implicitly converted to their underlying type

#define __ENUM_BEGIN(type, inh) \
	namespace __enum_##type { \
	enum type : inh {

#define __ENUM_END(type) \
	}; } \
	using __enum_##type::type

// Memory chunks

typedef void* memchunk_t;

static inline size_t MemChunk_GetSize(memchunk_t chunk)
{
	return ((size_t*)chunk)[-1];
}

static inline void MemChunk_Free(memchunk_t chunk)
{
	free((size_t*)chunk - 1);
}

template <typename T>
static inline T* MemChunk_Get(memchunk_t chunk)
{
	return (T*) chunk;
}

extern "C" memchunk_t MemChunk_Alloc(size_t size);
extern "C" memchunk_t File_Load(const char* fileName);

#define __COOPGUI_NAMESPACE_BEGIN \
	namespace FeOS { namespace UI {

#define __COOPGUI_NAMESPACE_END \
	} }
