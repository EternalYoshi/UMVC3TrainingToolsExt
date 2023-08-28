#include "gui.h"
#include <thread>
//#include <stdafx.h>

bool DetectsUMVC3 = false;
bool Hooked = false;

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{

	//Creates the GUI.
	gui::CreateHWindow("UMVC3 Training Tools Port Attempt", "Take 1. Does this appear?");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::exit)
	{

		gui::BeginRender();
		gui::Render();
		gui::Endrender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	//Discards the GUI.
	gui::DestroyImgui();
	gui::DestoryDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
