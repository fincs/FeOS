#pragma once

// Main application class
class MainApp : public CApplication
{
	color_t* buf;
	FontPtr font;
	int fontHeight;

public:
	MainApp();
	void OnActivate();
	void OnVBlank();
};
