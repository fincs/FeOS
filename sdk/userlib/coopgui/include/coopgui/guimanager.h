#pragma once
#pragma interface

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// Gui Manager Interface
//-----------------------------------------------------------------------

//! \brief GUI manager interface
struct IGuiManager
{
	//! \brief Retrieves the interface version the GUI manager implements.
	virtual int GetInterfaceVersion() = 0;

	// Interface version 1 functions
	// -----------------------------

	//! \brief Run an IApplication object
	virtual void RunApplication(IApplication* pApp) = 0;

	//! \brief Get the pointer to the currently running application
	virtual IApplication* GetActiveApp() = 0;

	//! \brief Set the currently running application -- pass nullptr to return to the main menu
	virtual void SwitchTo(IApplication* pApp) = 0;

	//! \brief Load a font
	virtual IFont* LoadFont(const char* face, int size) = 0;

	//! \brief Loads the system font
	virtual IFont* GetSystemFont() = 0; // Tahoma 10pt

	//! \brief Load a GRF file from a C stream
	virtual bool LoadGrf(GrfFile& grf, FILE* f) = 0;

	//! \brief Load a GRF file using a filename
	bool LoadGrf(GrfFile& grf, const char* aFileName);

	//! \brief Open a file using the file type system
	virtual void OpenFile(const char* aFileName) = 0;

	//! \brief Retrieve a file's type icon
	virtual IFileIcon* GetFileIcon(const char* aFileName) = 0;

	//! \brief Retrieve a file's description
	virtual void GetFileDescription(const char* aFileName, char* buffer, size_t size) = 0;

	//! \brief Create a keyboard.
	//! \param layout The keyboard layout to use. Pass `nullptr` in order to use the default layout.
	//!     (Currently layouts are not supported and anything else other than `nullptr` will make the call fail)
	virtual IKeyboard* CreateKeyboard(const char* layout = nullptr) = 0;
};

extern "C" IGuiManager* GetGuiManager(); //!< Retrieves the GUI manager object.
extern "C" IGuiManager* GetGuiManagerChecked(); //!< Checked version of GetGuiManager() that also prints an error message and does `exit(1)` if it fails

/** @} */

}
}
