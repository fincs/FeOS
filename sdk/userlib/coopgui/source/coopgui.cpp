#include <coopgui.h>

using namespace FeOS::UI;

static IGuiManager* pMon;

FEOS_EXPORT IGuiManager* FeOS::UI::GetGuiManagerChecked()
{
	if (pMon) return pMon;
	fprintf(stderr, "This app requires b0rkshell\n");
	exit(1);
}

FEOS_EXPORT IGuiManager* FeOS::UI::GetGuiManager()
{
	return pMon;
}

// Internal function - DO NOT USE
extern "C" FEOS_EXPORT void __setguimon(IGuiManager* newMon)
{
	pMon = newMon;
}
