#pragma once

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// Default IApplication Implementation
//-----------------------------------------------------------------------

//! \brief Application interface implementation base class.
template <typename AppInterface>
class CApplicationEx : public AppInterface
{
	AppInfo m_appInfo;
	AppCookie m_appCookie;

protected:
	//! \brief Closes the application.
	inline void Close() { m_appInfo.Flags |= AppFlags::Closed; }

	//! \brief Sets the title of the application.
	inline void SetTitle(const char* newTitle)
	{
		m_appInfo.Title = newTitle;
	}

	//! \brief Sets the behavioural flags of the application (see AppInfo::Flags).
	inline void SetFlags(word_t newFlags)
	{
		m_appInfo.Flags = newFlags;
	}

	//! \brief Sets the icon of the application. If `nullptr`, the default icon is used.
	inline void SetIcon(color_t* pIcon)
	{
		m_appInfo.Icon = pIcon;
	}

	inline CApplicationEx() : m_appCookie(nullptr)
	{
		m_appInfo.InterfaceVersion = AppInterface::__iface_ver();
		SetTitle(nullptr);
		SetFlags(0);
		SetIcon(nullptr);
	}

public:
	inline const AppInfo* GetInfo()
	{
		return &m_appInfo;
	}

	AppCookie GetOrSetCookie(AppCookie cookie)
	{
		AppCookie oldCookie = m_appCookie;
		if (cookie) m_appCookie = cookie;
		return oldCookie;
	}

	inline void OnActivate() { }
	inline void OnDeactivate() { }
	inline void OnVBlank() { }
	inline void OnBgProcess() { }
};

//! \brief Default application interface implementation. You should inherit from this class.
using CApplication = CApplicationEx<IApplication>;

/** @} */

}
}
