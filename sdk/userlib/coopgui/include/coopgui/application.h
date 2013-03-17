#pragma once
#pragma interface

#ifndef DOXYGEN
#define __IFACEVER(x) static inline constexpr int __iface_ver() { return (x); }
#else
#define __IFACEVER(x)
#endif

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// Application Interface
//-----------------------------------------------------------------------

// Application flags
#ifndef DOXYGEN
__ENUM_BEGIN(AppFlags, word_t)
	UsesSelect = BIT(0),
	Closed = (word_t)BIT(31),
	// More to come...
__ENUM_END(AppFlags);
#endif

//! \brief Application info structure
struct AppInfo
{
	int InterfaceVersion; //!< Specifies the interface version
	const char* Title; //!< \brief Specifies the title of the application
	//! \brief Specifies behavioural flags.
	//!
	//! - `AppFlags::UsesSelect` -- Disables default handling of the SELECT button
	//! - `AppFlags::Closed` -- Used internally in order to close the application
	word_t Flags;
	color_t* Icon; //!< Specifies the icon of the application (16x16 16bit image)
};

//! Application cookie.
#ifndef DOXYGEN
typedef struct { } _tagAppCookie, *AppCookie;
#else
typedef void* AppCookie;
#endif

//! Application interface
struct IApplication
{
	__IFACEVER(1);

	//! \brief Get information about the application.
	//! \returns The returned pointer MUST be valid and constant during the lifetime of the object
	virtual const AppInfo* GetInfo() = 0;

	//! \brief Get or set the application cookie (nullptr = get, otherwise set).
	//! \returns The (old in case of setting) application cookie.
	virtual AppCookie GetOrSetCookie(AppCookie cookie) = 0;

	//! \brief Called when an application is activated (aka switched to).
	//!
	//! Perform any necessary video initialization here.
	virtual void OnActivate() = 0;

	//! \brief Called when an application is deactivated (aka switched from).
	//!
	//! Perform any necessary cleanups here.
	virtual void OnDeactivate() = 0;

	//! \brief Called after VBlank, intended for video and key updates (graphics loading if necessary)
	virtual void OnVBlank() = 0;

	//! \brief Called after all VBlank processing is done, even for non-active apps.
	virtual void OnBgProcess() = 0;
};

/** @} */

}
}

#undef __IFACEVER
