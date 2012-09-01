#pragma once

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Default IApplication Implementation
//-----------------------------------------------------------------------

template <typename AppInterface>
class CApplicationEx : public AppInterface
{
	AppInfo m_appInfo;
	AppCookie m_appCookie;

protected:
	inline void Close() { m_appInfo.Flags |= AppFlags::Closed; }

	inline void SetTitle(const char* newTitle)
	{
		m_appInfo.Title = newTitle;
	}

	inline void SetFlags(word_t newFlags)
	{
		m_appInfo.Flags = newFlags;
	}

	// nullptr - default icon
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

using CApplication = CApplicationEx<IApplication>;

__COOPGUI_NAMESPACE_END
