#pragma once
#pragma interface

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Gui Manager Interface
//-----------------------------------------------------------------------

struct IGuiManager
{
	virtual int GetInterfaceVersion() = 0;

	// Interface version 1 functions
	// -----------------------------

	// Run an application object
	virtual void RunApplication(IApplication* pApp) = 0;

	// Get the pointer to the currently running application
	virtual IApplication* GetActiveApp() = 0;

	// Set the currently running application - pass nullptr to return to the main menu
	virtual void SwitchTo(IApplication* pApp) = 0;

	// Load a font
	virtual IFont* LoadFont(const char* face, int size) = 0;
	virtual IFont* GetSystemFont() = 0; // Tahoma 10pt

	// Load a GRF file
	virtual bool LoadGrf(GrfFile& grf, FILE* f) = 0;
	bool LoadGrf(GrfFile& grf, const char* aFileName);
};

extern "C" IGuiManager* GetGuiManager();
extern "C" IGuiManager* GetGuiManagerChecked(); // prints error message and does exit(1) if it fails

__COOPGUI_NAMESPACE_END
