#pragma once
#pragma interface

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//! \brief File icon interface.
struct IFileIcon
{
	virtual void Dispose() = 0; //!< Releases the object.
	virtual const color_t* GetData() = 0; //!< Gets the raw 16-bit 16x16 image data.
};

//! \brief File icon smart pointer datatype.
using FileIconPtr = ResPtr<IFileIcon>;

/** @} */

}
}
