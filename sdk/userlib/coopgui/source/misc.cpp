#include <coopgui.h>

FEOS_EXPORT memchunk_t MemChunk_Alloc(size_t size)
{
	size_t* mem = (size_t*) malloc(size+sizeof(size_t));
	if (!mem) return nullptr;
	*mem++ = size;
	return mem;
}

FEOS_EXPORT memchunk_t File_Load(const char* fileName)
{
	FILE* f = fopen(fileName, "rb");
	if (!f) return nullptr;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	rewind(f);

	memchunk_t mem = MemChunk_Alloc(size);
	if (!mem)
	{
		fclose(f);
		return nullptr;
	}

	if (fread(mem, 1, size, f) != size)
	{
		MemChunk_Free(mem);
		fclose(f);
		return nullptr;
	}

	fclose(f);
	return mem;
}

FEOS_EXPORT FeOS::UI::GrfFile::~GrfFile()
{
	if (gfxData)  MemChunk_Free(gfxData);
	if (mapData)  MemChunk_Free(mapData);
	if (mtilData) MemChunk_Free(mtilData);
	if (mmapData) MemChunk_Free(mmapData);
	if (palData)  MemChunk_Free(palData);
}

FEOS_EXPORT bool FeOS::UI::IGuiManager::LoadGrf(GrfFile& grf, const char* aFileName)
{
	FILE* f = fopen(aFileName, "rb");
	if (!f) return false;

	bool rc = LoadGrf(grf, f);
	fclose(f);
	return rc;
}
