#pragma once

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// GRF API
//-----------------------------------------------------------------------

//! \brief GRF header. Read <a href="http://www.coranac.com/man/grit/html/grit.htm#ssec-info-grf">the Grit documentation</a> for more details.
//! \headerfile coopgui.h <coopgui.h>
struct GrfHeader
{
	u8  gfxAttr, mapAttr, mmapAttr, palAttr;
	u8  tileWidth, tileHeight;
	u8  metaWidth, metaHeight;
	u32 gfxWidth, gfxHeight;
};

//! \brief GRF graphics object. Read <a href="http://www.coranac.com/man/grit/html/grit.htm#ssec-info-grf">the Grit documentation</a> for more details.
//! \headerfile coopgui.h <coopgui.h>
struct GrfFile
{
	GrfHeader hdr; //!< GRF header.
	memchunk_t gfxData, mapData, mtilData, mmapData, palData;

	inline GrfFile() : gfxData(nullptr), mapData(nullptr), mtilData(nullptr), mmapData(nullptr), palData(nullptr) { }
	~GrfFile();
};

/** @} */

}
}
