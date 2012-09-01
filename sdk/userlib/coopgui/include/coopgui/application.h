#pragma once
#pragma interface

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Application Interface
//-----------------------------------------------------------------------

// Application flags
__ENUM_BEGIN(AppFlags, word_t)
	UsesSelect = BIT(0), // Disables default handling of the SELECT button
	Closed = (word_t)BIT(31), // Used internally in order to close the application
	// More to come...
__ENUM_END(AppFlags);

// Application info structure
struct AppInfo
{
	int InterfaceVersion;
	const char* Title;
	word_t Flags;
	color_t* Icon;
};

// Application cookie
typedef struct { } _tagAppCookie, *AppCookie;

#define __IFACEVER(x) static inline constexpr int __iface_ver() { return (x); }

struct IApplication
{
	__IFACEVER(1);

	// Get information about the application - the returned pointer
	// MUST be valid and constant during the lifetime of the object
	virtual const AppInfo* GetInfo() = 0;

	// Get or set the application cookie (nullptr = get, otherwise set).
	// Returns the (old in case of setting) application cookie.
	virtual AppCookie GetOrSetCookie(AppCookie cookie) = 0;

	// Called when an application is (de)activated (aka switched from/to)
	// Do your video inits in OnActivate and any necessary cleanups in OnDeactivate
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

	// Called after VBlank, intended for video and key updates (graphics loading if necessary)
	virtual void OnVBlank() = 0;

	// Called after all VBlank processing is done, even for non-active apps
	virtual void OnBgProcess() = 0;
};

#undef __IFACEVER

__COOPGUI_NAMESPACE_END
