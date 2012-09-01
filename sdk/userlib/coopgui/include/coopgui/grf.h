#pragma once

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// GRF API
//-----------------------------------------------------------------------

struct GrfHeader
{
	u8  gfxAttr, mapAttr, mmapAttr, palAttr;
	u8  tileWidth, tileHeight;
	u8  metaWidth, metaHeight;
	u32 gfxWidth, gfxHeight;
};

struct GrfFile
{
	GrfHeader hdr;
	memchunk_t gfxData, mapData, mtilData, mmapData, palData;

	inline GrfFile() : gfxData(nullptr), mapData(nullptr), mtilData(nullptr), mmapData(nullptr), palData(nullptr) { }
	~GrfFile();
};

__COOPGUI_NAMESPACE_END
