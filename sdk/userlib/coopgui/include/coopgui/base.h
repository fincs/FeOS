#pragma once

// Strongly typed enums that can be implicitly converted to their underlying type

#define __ENUM_BEGIN(type, inh) \
	namespace __enum_##type { \
	enum type : inh {

#define __ENUM_END(type) \
	}; } \
	using __enum_##type::type

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

#define COOPGUI_GUIMANAGER_VER 1 //!< Current GUI manager interface version

// Memory chunks

typedef void* memchunk_t; //!< Memory chunk pointer.

//! \brief Retrives the size of a memory chunk.
static inline size_t MemChunk_GetSize(memchunk_t chunk)
{
	return ((size_t*)chunk)[-1];
}

//! \brief Frees a memory chunk.
static inline void MemChunk_Free(memchunk_t chunk)
{
	free((size_t*)chunk - 1);
}

//! \brief Casts a memory chunk pointer to a certain type.
template <typename T>
static inline T* MemChunk_Get(memchunk_t chunk)
{
	return (T*) chunk;
}

extern "C" memchunk_t MemChunk_Alloc(size_t size); //!< Allocates a memory chunk.
extern "C" memchunk_t File_Load(const char* fileName); //!< Loads the contents of a file into memory.

/** @} */
