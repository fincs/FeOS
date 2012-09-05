#include "common.h"

IGuiManager* g_guiManager;

MainApp::MainApp()
{
	// Set the title of the application
	SetTitle("Coop GUI App Template");

	// Load the stock font - there's no error checking for simplicity
	font = g_guiManager->GetSystemFont();
	fontHeight = font->GetHeight();
}

void MainApp::OnActivate()
{
	// Prepare top-screen 16-bit bitmap drawing
	videoSetMode(MODE_3_2D);
	int bmpBg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	buf = bgGetGfxPtr(bmpBg);

	dmaFillHalfWords(Colors::White, buf, 256*192*2);
	font->PrintText(buf, 8, 8, "Hello, world!");
	font->PrintText(buf, 8, 8+fontHeight, "Touch the screen to exit");
}

void MainApp::OnVBlank()
{
	// Place your VBlank code here
	word_t kDown = keysDown();
	if (kDown & KEY_TOUCH)
	{
		Close();
		return;
	}
	//...etc
}

int main()
{
	g_guiManager = GetGuiManagerChecked();

	MainApp app;
	g_guiManager->RunApplication(&app);

	return 0;
}
