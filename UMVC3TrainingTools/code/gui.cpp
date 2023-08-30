#include "gui.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "proc.h"
#include "umvc3utils.h"

#define IMGUI_ENABLE_FREETYPE

#pragma region Take1

//Window Process handler.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler
(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)

{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) return true;


	switch (message)
	{
		//Called when resizing Window I think.
	case WM_SIZE:
	{
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
		return 0;

		//Disallows ALT Application Menu.
	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU)
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

		//Sets Click Points.
	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;

		//Mouse movements.
	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{};

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 && gui::position.x <= gui::WIDTH && gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);



		};

	}


	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);

}

void gui::CreateHWindow(const char* windowName, const char* className) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA
	(
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;

}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

}

void gui::DestoryDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}
	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	/*
	io.Fonts->AddFontDefault();
	ImFont* mainfont = io.Fonts->AddFontFromFileTTF("E:\\DLs\\Fontworks\\Japanese\\Gothic (Black)\\FOT-NewRodinPro-B.otf",18.5f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(mainfont != NULL);
	*/

}

void gui::DestroyImgui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//Starts the Dear Imgui frame.
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::Endrender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	//Handles loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();

}

//My attempt to port hooking code starts here.
void CastTheHook()
{
	ProcID = GetProcId(L"umvc3.exe");
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcID);
	if (!hProcess)
	{
		DWORD ex = GetLastError();
	}

	if(ProcID != 0)
	{
		ModuleBase = GetModuleBaseAddress(ProcID);
		ValidVer = CheckGame();
		if(ValidVer)gui::Hooked = true;
	}
	else
	{
		gui::Hooked = false;
	}

}

//ImGui menu stuff begins here.
static void gui::TheMenuBar()
{
	if (ShowAboutWindow) gui::TheAboutWindow(&ShowAboutWindow);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			ImGui::MenuItem(("Exit"), NULL, &exit);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Misc"))
		{


			ImGui::MenuItem("About", NULL, &ShowAboutWindow);


			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

}

void gui::TheAboutWindow(bool* p_open)
{
	ImGui::SetNextWindowSize({ 400, 400 }, ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("UMVC3 Training Tools Mod V0.4X", p_open))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("2023 By Eternal Yoshi");
	ImGui::Separator();
	ImGui::Text("Thanks To SanHKHaan & Sheep for finding the pointers\nand memeory offsets that made this possible,\nand Ermmaccer for the original UMVC3Hook this\nversion is based on.");
	ImGui::Separator();
	ImGui::Text("In case it isn't obvious, this tool is NOT designed to\nenable cheating in netplay and\nis intended to only function in training mode.");
	
	ImGui::End();

}

static void gui::TheCharacterOptionsTab() 
{

	if (ImGui::BeginTabItem("Character Settings"))
	{

		ImGui::Text("Remember! These Parameters will only take\neffect in training mode.");

		ImGui::SeparatorText("Frank West");

		if (FrankLevel)
		{
			ImGui::Text("Frank West's Level");
			if (ImGui::SliderInt("Lv", &FrankLevel, 1, 5))
			{
				ChangeFrankLevel(FrankLevel);				
			}
		}

		ImGui::Separator();

		ImGui::Text("Gotta port the rest if the above works.");

		ImGui::EndTabItem();

	}

}

static void gui::TheExtraOptionsTab()
{
	if (ImGui::BeginTabItem("Extra Settings"))
	{
		ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");

		ImGui::Separator();

		if (ImGui::Button("Restart Training Mode"))
		{
			if (CheckTheMode() == true)
			{
				//*(unsigned char*)(block2 + 0x118) = 3;
				int RestartValue = 3;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(block2 + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else 
				{
					val = 3;
				}
			}
		}

		ImGui::SeparatorText("Positioning");

		ImGui::Text("Player 1 Position");
		if (ImGui::SliderFloat("P1 X Position", &p1Pos, -800.0f, 800.0f))
		{

		}

		ImGui::Text("Player 2 Position");
		if (ImGui::SliderFloat("P2 X Position", &p2Pos, -800.0f, 800.0f))
		{

		}

		ImGui::Text("Position Presets");
		if (ImGui::Button("Default"))
		{

			p1Pos = -125.0f;
			p2Pos = 125.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button("Default P2 Side"))
		{

			p1Pos = 125.0f;
			p2Pos = -125.0f;


		}
		if (ImGui::Button("Left Corner"))
		{

			p1Pos = -750.0f;
			p2Pos = -800.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button("Left Corner P2 Side"))
		{

			p1Pos = -800.0f;
			p2Pos = -750.0f;


		}

		if (ImGui::Button("Right Corner"))
		{

			p1Pos = 750.0f;
			p2Pos = 800.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button("Right Corner P2 Side"))
		{

			p1Pos = 800.0f;
			p2Pos = 750.0f;


		}

		ImGui::SeparatorText("Etc");

		if (ImGui::Checkbox("Endless X-Factor P1", &EndlessXFactorP1))
		{

		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Endless X-Factor P2", &EndlessXFactorP2))
		{

		}

		ImGui::Separator();

		ImGui::Text("More features coming soon!");

		ImGui::EndTabItem();
	}
}

void gui::Render() noexcept
{
	ImGui::SetWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);

	ImGui::Begin(
		"UMVC3 Training Tools Mod V0.4X By Eternal Yoshi",
		&exit,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	//The Menu Bar.
	TheMenuBar();

	//Tries to hook.
	CastTheHook();

	//Hides all the options if the game isn't open.
	if(Hooked)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(70, 255, 70, 255));
		ImGui::Text("If you can read this then the tool recognizes Marvel 3 is open.");
		ImGui::Text("The version open is valid too.");
		ImGui::PopStyleColor();

		if(CheckTheMode() == true)
		{
			GetMainPointers();
			TickUpdates();
			if (ImGui::BeginTabBar("##tabs"))
			{
				ReadProcessMemory(hProcess, (LPVOID*)(block2 + 0x118), &val, sizeof(val), 0);

				if (val != 0)
				{
					restarted = true;
					restartTimer = 0;
				}
				else if (restarted)
				{
					restartTimer += 1;
					if (restartTimer > 5)//0.166667 sec wait
					{
						RestartWithChanges();

						{
							//auto ptr = (P1Character1Data + 0x50);
							//*((float*)ptr) = p1Pos;
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x50), &p1Pos, sizeof(float), NULL))
							{

							}
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x50), &p1Pos, sizeof(float), NULL))
							{

							}
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x50), &p1Pos, sizeof(float), NULL))
							{

							}
						}
						{
							//auto ptr = (P1Character1Data + 0x50);
							//*((float*)ptr) = p2Pos;
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x50), &p2Pos, sizeof(float), NULL))
							{

							}
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x50), &p2Pos, sizeof(float), NULL))
							{

							}
							if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x50), &p2Pos, sizeof(float), NULL))
							{

							}
						}
						restarted = false;
						//SetIndividualCharacterHealth();

					}


				}
				
				TickUpdates();
				TheExtraOptionsTab();
				TheCharacterOptionsTab();
			
				ImGui::EndTabBar();

			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 70, 255));
			ImGui::Text("Get to Training Mode so this tool can do its thing.");
			ImGui::PopStyleColor();
		
		}

	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 70, 70, 255));
		ImGui::Text("Nothing Doing until you open the game.");
		ImGui::Text("If the game is open and nothing is happening,\nthen it could be possible\nyou are using an invalid version.");
		ImGui::PopStyleColor();

	}


	ImGui::End();

}
#pragma endregion
