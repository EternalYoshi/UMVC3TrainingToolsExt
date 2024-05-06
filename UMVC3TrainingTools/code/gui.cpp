#define _CRT_SECURE_NO_DEPRECATE
#include "gui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "proc.h"
#include "umvc3utils.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <list>
#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include "..\utils\Trampoline.h"
#include "..\utils\addr.h"
#include "..\utils\MemoryMgr.h"
#include "..\MemoryMgr.h"
#include "..\minhook\include\MinHook.h"
#include <thread>

HINSTANCE ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));

#define IMGUI_ENABLE_FREETYPE

//Window Process handler.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler
(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

int RecordingSlot = 1;

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


	io.Fonts->AddFontDefault();
	//mainfont = io.Fonts->AddFontFromFileTTF("Resources\\fonts\\FOT-NewRodinPro-DB.otf", 11.5f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(mainfont != NULL);
	//ImFont* secfont = io.Fonts->AddFontFromFileTTF("E:\\DLs\\Fontworks\\Japanese\\Gothic (Black)\\FOT-NewRodinPro-B.otf", 18.5f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(secfont != NULL);

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

	if (ProcID != 0)
	{
		ModuleBase = GetModuleBaseAddress(ProcID);
		ValidVer = CheckGame();
		if (ValidVer)gui::Hooked = true;
	}
	else
	{
		gui::Hooked = false;
	}
	HProc = hProcess;
}

void CheckIfInMatch()
{

	if (Player1CharNodeTree == 0 && Player2CharNodeTree == 0)
	{
		gui::InMatch = false;
	}
	else
	{
		gui::InMatch = true;
	}
}

void TrampHookPart2(HANDLE hProcess, LPVOID TargetAddress, Trampoline* tramp, unsigned int nType)
{

}

#pragma region MenuTabs
//ImGui menu stuff begins here.
static void gui::TheMenuBar()
{
	if (ShowAboutWindow) gui::TheAboutWindow(&ShowAboutWindow);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
#ifdef _DEBUG
			if (ImGui::MenuItem(("Find ThreeHook"), "CTRL+O"))
			{
				std::string ChosenFile = "";
				OPENFILENAME ofn = { sizeof(OPENFILENAME) };
				char szFile[_MAX_PATH] = "ThreeHook";
				const char szExt[] = ".asi\0"; // extra '\0' for lpstrFilter
				ofn.hwndOwner = GetConsoleWindow();
				ofn.lpstrFile = szFile; // <--------------------- initial file name
				ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
				ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
				ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
				if (GetOpenFileName(&ofn))
				{
					ChosenFile = ofn.lpstrFile;
					ThreeHookPath = ChosenFile;
					ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));
					if (ccheck)
					{
						typedef void(*DeployTheHooks)();
						DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");
						dhook();
					}



				}
			}
#endif
			if (ImGui::MenuItem(("Reset Settings To Default")))
			{
				ResetSettings();
			}

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
	if (!ImGui::Begin("UMVC3 Training Tools Mod V0.6", p_open))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("2023, 2024 By Eternal Yoshi");
	ImGui::Separator();
	ImGui::Text("Thanks To SanHKHaan, Sheep, Techs, & Gneiss for finding\nthe pointers and memeory offsets that made this \npossible,and Ermmaccer for the original UMVC3Hook.\n");
	ImGui::Separator();
	ImGui::Text("In case it isn't obvious, this tool is NOT designed to\nenable cheating in online multiplayer and\nis intended to only function in Training Mode.");

	ImGui::End();

}

static void gui::TheCharacterOptionsTab()
{
	GetMainPointers();
	if (ImGui::BeginTabItem("Character"))
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

		ImGui::SeparatorText("Phoenix Wright");

		ImGui::Text("Mr. Wright's Evidence");
		if (WrightEvidenceA || WrightEvidenceB || WrightEvidenceC)
		{
			ImGui::Text("Evidence Slot 1");

			if (ImGui::BeginCombo("##WrightEvidencecombo", selected_item))
			{
				if (CheckTheMode() == true)
				{
					for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypes); n++)
					{
						bool is_selected = (selected_item == EvidenceTypes[n]);
						if (ImGui::Selectable(EvidenceTypes[n], is_selected))
						{
							selected_item = EvidenceTypes[n];
							WrightEvidenceA = EvidenceNameToID(selected_item);
							ChangeWrightEvidence();

						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Evidence Slot 2");

			if (ImGui::BeginCombo("##WrightEvidencecombo2", selected_itemTwo))
			{
				if (CheckTheMode() == true)
				{
					for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypesTwo); n++)
					{
						bool is_selected = (selected_itemTwo == EvidenceTypesTwo[n]);
						if (ImGui::Selectable(EvidenceTypesTwo[n], is_selected))
						{
							selected_itemTwo = EvidenceTypesTwo[n];
							WrightEvidenceB = EvidenceNameToID(selected_itemTwo);
							ChangeWrightEvidence();

						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Evidence Slot 3");

			if (ImGui::BeginCombo("##WrightEvidencecombo3", selected_itemThree))
			{
				if (CheckTheMode() == true)
				{
					for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypesThree); n++)
					{
						bool is_selected = (selected_itemThree == EvidenceTypesThree[n]);
						if (ImGui::Selectable(EvidenceTypesThree[n], is_selected))
						{
							selected_itemThree = EvidenceTypesThree[n];
							WrightEvidenceC = EvidenceNameToID(selected_itemThree);
							ChangeWrightEvidence();

						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

		}

		//ImGui::Text("Turnabout Toggle");
		if (ImGui::Checkbox("Turnabout Toggle", &Turnabout))
		{
			if (CheckTheMode() == true)
			{
				Objection();
			}
		}

		ImGui::SeparatorText("Phoenix/Jean");
		//ImGui::Text("Dark Phoenix Toggle");
		if (ImGui::Checkbox("Dark Phoenix Toggle", &DarkPhoenix))
		{
			if (CheckTheMode() == true)
			{
				PopTheBird();
			}
		}

		ImGui::SeparatorText("MODOK");
		if (MODOKLOU)
		{
			ImGui::Text("MODOK Level of Understanding");
			if (ImGui::SliderInt("LOU", &MODOKLOU, 1, 10))
			{
				if (CheckTheMode() == true)
				{
					ChangeMODOKUnderstanding(MODOKLOU);
				}
			}
		}
		//ImGui::Text("Lock Understanding");
		if (ImGui::Checkbox("Lock Understanding", &LockMODOKLOU))
		{
			if (CheckTheMode() == true)
			{
				ChangeMODOKUnderstanding(MODOKLOU);
			}
		}

		ImGui::SeparatorText("Dormammu");
		ImGui::Text("Dormammu's Spell Charges");

		ImGui::Text("Power Of The Destructor");
		if (ImGui::SliderInt("Red", &DormRed, 0, 3))
		{
			if (CheckTheMode() == true)
			{
				DormSpellSet = true;
				SetDormSpellLevels();
			}
		}

		ImGui::Text("Power Of The Creator");
		if (ImGui::SliderInt("Blue", &DormBlue, 0, 3))
		{
			if (CheckTheMode() == true)
			{
				DormSpellSet = true;
				SetDormSpellLevels();
			}
		}


		ImGui::SeparatorText("Deadpool");
		//ImGui::Text("Deadpool Teleport");
		if (ImGui::SliderInt("Teleport Count", &DeadpoolTeleportCount, 0, 2))
		{
			if (CheckTheMode() == true)
			{
				SetDeadpoolTeleport();
			}
		}

		if (ImGui::Checkbox("Lock Teleport Count", &FreezeDeadpoolTPCounter))
		{
			if (CheckTheMode() == true)
			{
			}
		}

		ImGui::SeparatorText("Etc.");
		//ImGui::Text("Endless Install Toggle");
		if (ImGui::Checkbox("Endless Install Toggle", &EndlessInstalls))
		{
			if (CheckTheMode() == true)
			{
				EndlessInstallBoolUpdate();
			}
		}

		ImGui::Separator();
		ImGui::EndTabItem();

	}

}

static void gui::TheExtraOptionsTab()
{
	GetMainPointers();
	if (ImGui::BeginTabItem("Extra"))
	{
		ImGui::Separator();

		if (ImGui::Button("Restart To Intros"))
		{
			if (CheckTheMode() == true)
			{
				//*(unsigned char*)(block2 + 0x118) = 3;
				int RestartValue = 1;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(block2 + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else
				{
					val = 3;
				}
			}
		}

		ImGui::Text("Note: Pressing the above button multiple times will result\n in the intro replaying after FIGHT!!!");

		if (ImGui::Button("Restart To Ready"))
		{
			if (CheckTheMode() == true)
			{
				//*(unsigned char*)(block2 + 0x118) = 3;
				int RestartValue = 2;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(block2 + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else
				{
					val = 3;
				}
			}
		}

		if (ImGui::Button("Restart Training Mode"))
		{
			if (CheckTheMode() == true)
			{
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
		if (ImGui::DragFloat("P1 X Position", &p1Pos, 1.0f, -800.0f, 800.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{
			if (p1Pos > 800)
			{
				p1Pos = 800;
			}
			else if (p1Pos < -800)
			{
				p1Pos = -800;
			}
		}

		ImGui::Text("Player 2 Position");
		if (ImGui::DragFloat("P2 X Position", &p2Pos, 1.0f, -800.0f, 800.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{
			if (p2Pos > 800)
			{
				p2Pos = 800;
			}
			else if (p2Pos < -800)
			{
				p2Pos = -800;
			}
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

		ImGui::SeparatorText("Etc.");

		ImGui::Separator();

		//ImGui::Text("Game Speed");
		if (ImGui::DragFloat("Character Speed P1", &P1Char1Speed, 0.01f, 0.01666667f, 2.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{

			if (P1Char1Speed > 2.0)
			{
				P1Char1Speed = 2.0;
			}
			else if (P1Char1Speed < 0.01666667)
			{
				P1Char1Speed = 0.01666667;
			}

			SetGlobalPlayerSpeed(P1Char1Speed);
		}
		
		if (ImGui::DragFloat("Character Speed P2", &P2Char1Speed, 0.01f, 0.01666667f, 2.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{

			if (P2Char1Speed > 2.0)
			{
				P2Char1Speed = 2.0;
			}
			else if (P2Char1Speed < 0.01666667)
			{
				P2Char1Speed = 0.01666667;
			}

			SetGlobalPlayerSpeed(P2Char1Speed);
		}

		if (ImGui::Button("Normal Speed P1"))
		{
			P1Char1Speed = 1;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		ImGui::SameLine();

		if (ImGui::Button("Normal Speed P2"))
		{
			P2Char1Speed = 1;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		if (ImGui::Button("VJoe Slow Speed P1"))
		{
			P1Char1Speed = 0.75;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		ImGui::SameLine();

		if (ImGui::Button("VJoe Slow Speed P2"))
		{
			P2Char1Speed = 0.75;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		/*
		if (ImGui::Checkbox("Enable Game Speed Change(Use at own risk!)", &ModifyGameSpeed))
		{

		}


		if (ModifyGameSpeed)
		{
			if (ImGui::SliderFloat("Game Speed", &GameSpeed, 0.01666667f, 5.0f))
			{
				SetGameSpeed(GameSpeed);

			}
		}
		else
		{
			ResetGameSpeed();
		}
		*/

		ImGui::Separator();

#ifdef _DEBUG

		if (ImGui::Checkbox("Move Inputs to Left Side of Screen", &MoveInputDisplay))
		{
			LeftSideInputDisplay();
		}

		ImGui::Separator();
#endif
		ImGui::Text("More features coming soon!");

		ImGui::EndTabItem();
	}
}

static void gui::TheStatusOptionsTab()
{
	GetMainPointers();
	//Player and Status Related  Settings such as Health.
	if (ImGui::BeginTabItem("Status"))
	{
		ImGui::SeparatorText("Character HP");

		//Toggle for using individual character health.
		if (ImGui::Checkbox("Set Individual Character Health(Applies during restarts).", &SetIndividualHP))
		{

		}

		//Toggle for Applying Health Settings to Red Health.
		if (ImGui::Checkbox("Also Apply To Red Health", &AlsoSetRedHealth))
		{

		}

		if (P1Char1Health)
		{
			ImGui::Text("Player 1 Character 1 Health");
			if (ImGui::DragFloat("P1C1 HP %", &P1Char1Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{


				if (P1Char1Health > 1.0)
				{
					P1Char1Health = 1.0;
				}
				else if (P1Char1Health < 0.01)
				{
					P1Char1Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P1Char2Health)
		{
			ImGui::Text("Player 1 Character 2 Health");
			if (ImGui::DragFloat("P1C2 HP %", &P1Char2Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{

				if (P1Char2Health > 1.0)
				{
					P1Char2Health = 1.0;
				}
				else if (P1Char2Health < 0.01)
				{
					P1Char2Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P1Char3Health)
		{
			ImGui::Text("Player 1 Character 3 Health");
			if (ImGui::DragFloat("P1C3 HP %", &P1Char3Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{

				if (P1Char3Health > 1.0)
				{
					P1Char3Health = 1.0;
				}
				else if (P1Char3Health < 0.01)
				{
					P1Char3Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char1Health)
		{
			ImGui::Text("Player 2 Character 1 Health");
			if (ImGui::DragFloat("P2C1 HP %", &P2Char1Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{

				if (P2Char1Health > 1.0)
				{
					P2Char1Health = 1.0;
				}
				else if (P2Char1Health < 0.01)
				{
					P2Char1Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char2Health)
		{
			ImGui::Text("Player 2 Character 2 Health");
			if (ImGui::DragFloat("P2C2 HP %", &P2Char2Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{

				if (P2Char2Health > 1.0)
				{
					P2Char2Health = 1.0;
				}
				else if (P2Char2Health < 0.01)
				{
					P2Char2Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char3Health)
		{
			ImGui::Text("Player 2 Character 3 Health");
			if (ImGui::DragFloat("P2C3 HP %", &P2Char3Health, 0.003f, 0.001f, 1.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
			{

				if (P2Char3Health > 1.0)
				{
					P2Char3Health = 1.0;
				}
				else if (P2Char3Health < 0.01)
				{
					P2Char3Health = 0.01;
				}

				if (CheckTheMode() == true)
				{

				}
			}
		}

		ImGui::SeparatorText("Hyper Meter");

		//if (P1Meter)
		//{
		ImGui::Text("Player 1 Meter");
		if (ImGui::DragInt("P1 Meter", &P1Meter, 100.0f, 0, 50000, "%d"))
		{
			if (CheckTheMode() == true)
			{

				if (P1Meter > 50000)
				{
					P1Meter = 50000;
				}
				else if (P1Meter < 0)
				{
					P1Meter = 0;
				}

				SetMeters();
			}
		}
		//}

		//if (P2Meter)
		//{
		ImGui::Text("Player 2 Meter");
		if (ImGui::DragInt("P2 Meter", &P2Meter, 100.0f, 0, 50000, "%d"))
		{
			if (CheckTheMode() == true)
			{


				if (P2Meter > 50000)
				{
					P2Meter = 50000;
				}
				else if (P2Meter < 0)
				{
					P2Meter = 0;
				}

				SetMeters();
			}
		}
		//}

		if (ImGui::Checkbox("Lock P1 Meter", &LockP1Meter))
		{

		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Lock P2 Meter", &LockP2Meter))
		{

		}

		ImGui::SeparatorText("Jamming Bomb/Reversed Controls");

		if (ImGui::Checkbox("Player 1 Character 1", &P1C1Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}

		if (ImGui::Checkbox("Player 1 Character 2", &P1C2Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}

		if (ImGui::Checkbox("Player 1 Character 3", &P1C3Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}

		if (ImGui::Checkbox("Player 2 Character 1", &P2C1Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}

		if (ImGui::Checkbox("Player 2 Character 2", &P2C2Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}

		if (ImGui::Checkbox("Player 2 Character 3", &P2C3Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		ImGui::Separator();

		ImGui::Text("More stuff coming soon!");

		ImGui::EndTabItem();

	}
}

static void gui::TheIncomingStuffTab()
{
	GetMainPointers();
	if (ImGui::BeginTabItem("Incoming"))
	{
		ImGui::Text("Experimental! Be Smart about using these.");
		ImGui::Separator();
		
		ImGui::Text("Time Delay(Frames)");
		if (ImGui::SliderInt("Frames Until Death", &FrameDelayofDeath, 0, 300))
		{
		}
		
		if (ImGui::SliderFloat("Position of KO", &DeathSiteX, -800.0f, 800.0f))
		{

		}

		if (ImGui::Checkbox("KO Player 1's Active Character At Training Mode Restart", &KOActiveCharacterAtStart))
		{
			if (CheckTheMode() == true)
			{
			}
		}

		if (ImGui::Checkbox("KO Player 2's Active Character At Training Mode Restart", &KOActiveOpponentAtStart))
		{
			if (CheckTheMode() == true)
			{
			}
		}

		ImGui::SeparatorText("KO Toggles");

		if (ImGui::Checkbox("Player 1 Character 1 Dead", &Player1Character1Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Player 1 Character 2 Dead", &Player1Character2Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Player 1 Character 3 Dead", &Player1Character3Dead))
		{
			KOToggles();
		}
		if (ImGui::Checkbox("Player 2 Character 1 Dead", &Player2Character1Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Player 2 Character 2 Dead", &Player2Character2Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Player 2 Character 3 Dead", &Player2Character3Dead))
		{
			KOToggles();
		}

		ImGui::Text("More stuff coming soon!");

		ImGui::EndTabItem();

	}

}

static void gui::TheRecordPlaybackTab()
{
	//For Recording And Playback Stuff.
	if (ImGui::BeginTabItem("Record & Playback"))
	{
		GetMainPointers();
		//HINSTANCE ccheck = LoadLibrary("E:\\ULTIMATE MARVEL VS. CAPCOM 3\\Scripts\\ThreeHook.asi");

		ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");

		typedef bool(*CheckIfRecordingP1)();
		typedef bool(*CheckIfRecordingP2)();
		typedef bool(*CheckIfRecordingBoth)();
		typedef bool(*CheckIfPlayback1P)(int);
		typedef bool(*CheckIfPlayback2P)(int);
		typedef void(*RecordBoth)(int);
		typedef void(*RecordP1)(int);
		typedef void(*RecordP2)(int);
		typedef void(*StopRecording)(int);
		typedef void(*PlaybackP1)(int);
		typedef void(*PlaybackP2)(int);
		typedef void(*PlaybackBoth)(int);
		typedef bool(*CheckReplayAvailableP1)(int);
		typedef bool(*CheckReplayAvailableP2)(int);
		typedef bool(*CheckReplayAvailableBoth)(int);
		typedef void(*StopPlaybackP1)(int);
		typedef void(*StopPlaybackP2)(int);
		typedef void(*StopPlaybackBoth)(int);
		//typedef void(*DeployTheHooks)();
		typedef void(*ChangeRecordingSlot)(int);

		if (ccheck != NULL)
		{
			CheckIfRecordingP1 RP1 = (CheckIfRecordingP1)GetProcAddress((HMODULE)ccheck, "CheckIfRecordingP1");
			CheckIfRecordingP2 RP2 = (CheckIfRecordingP2)GetProcAddress((HMODULE)ccheck, "CheckIfRecordingP2");
			CheckIfRecordingBoth RPB = (CheckIfRecordingBoth)GetProcAddress((HMODULE)ccheck, "CheckIfRecordingBoth");
			CheckIfPlayback1P IPB1 = (CheckIfPlayback1P)GetProcAddress((HMODULE)ccheck, "CheckIfPlayback1P");
			CheckIfPlayback2P IPB2 = (CheckIfPlayback2P)GetProcAddress((HMODULE)ccheck, "CheckIfPlayback2P");
			StopRecording SPB = (StopRecording)GetProcAddress((HMODULE)ccheck, "StopRecording");
			RecordP1 RCP1 = (RecordP1)GetProcAddress((HMODULE)ccheck, "RecordP1");
			RecordP2 RCP2 = (RecordP2)GetProcAddress((HMODULE)ccheck, "RecordP2");
			RecordBoth RCB12 = (RecordBoth)GetProcAddress((HMODULE)ccheck, "RecordBoth");
			PlaybackP1 PBP1 = (PlaybackP1)GetProcAddress((HMODULE)ccheck, "PlaybackP1");
			PlaybackP2 PBP2 = (PlaybackP2)GetProcAddress((HMODULE)ccheck, "PlaybackP2");
			PlaybackBoth PBB12 = (PlaybackBoth)GetProcAddress((HMODULE)ccheck, "PlaybackBoth");
			CheckReplayAvailableP1 IPBAP1 = (CheckReplayAvailableP1)GetProcAddress((HMODULE)ccheck, "CheckReplayAvailableP1");
			CheckReplayAvailableP2 IPBAP2 = (CheckReplayAvailableP2)GetProcAddress((HMODULE)ccheck, "CheckReplayAvailableP2");
			CheckReplayAvailableBoth IPBBA12 = (CheckReplayAvailableBoth)GetProcAddress((HMODULE)ccheck, "CheckReplayAvailableBoth");
			StopPlaybackP1 SPBP1 = (StopPlaybackP1)GetProcAddress((HMODULE)ccheck, "StopPlaybackP1");
			StopPlaybackP2 SPBP2 = (StopPlaybackP2)GetProcAddress((HMODULE)ccheck, "StopPlaybackP2");
			StopPlaybackBoth SPBP12 = (StopPlaybackBoth)GetProcAddress((HMODULE)ccheck, "StopPlaybackBoth");
			ChangeRecordingSlot CRS = (ChangeRecordingSlot)GetProcAddress((HMODULE)ccheck, "ChangeRecordingSlot");
			//DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");

			//dhook();

			if (!RP1() && !RP2())
			{
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						RCB12(RecordingSlot);
					}
				}
			}

			if (!RP1() && !RP2())
			{
				if (ImGui::Button("Record 1P")) {
					if (CheckTheMode() == true)
					{
						RCP1(RecordingSlot);
					}
				}
			}

			if (!RP1() && !RP2())
			{
				if (ImGui::Button("Record 2P")) {
					if (CheckTheMode() == true)
					{
						RCP2(RecordingSlot);
					}
				}
			}

			else if (RP1 || RP2)
			{

				if (ImGui::Button("Stop Recording"))
				{
					if (CheckTheMode() == true)
					{
						SPB(RecordingSlot);
					}
				}

			}

			if (IPBAP1(RecordingSlot) && IPBAP2(RecordingSlot) && !IPB1(RecordingSlot) && !IPB2(RecordingSlot))
			{
				if (!PBP1 && !PBP2)
				{
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{
							PBB12(RecordingSlot);
						}

					}
				}
				else if (!PBP1 && !PBP2)
				{
					if (ImGui::Button("Stop Playback"))
					{
						SPBP12(RecordingSlot);
					}
				}
			}

			if (IPBAP1(RecordingSlot) && (!IPB1(RecordingSlot)))
			{

				if (!IPB1)
				{
					if (ImGui::Button("Playback P1")) {
						{
							PBP1(RecordingSlot);
						}

					}
				}

				else if (IPB1)
				{
					if (ImGui::Button("Stop Playback P1"))
					{
						SPBP1(RecordingSlot);
					}
				}

				ImGui::SameLine();
			}

			if (IPBAP2(RecordingSlot) && (!IPB2(RecordingSlot)))
			{

				if (!IPB2)
				{
					if (ImGui::Button("Playback P2")) {
						{
							PBP2(RecordingSlot);
						}

					}
				}

				else if (IPB2)
				{
					if (ImGui::Button("Stop Playback P2"))
					{
						SPBP2(RecordingSlot);
					}
				}

				ImGui::SameLine();
			}

			if (RecordingSlot)
			{
				ImGui::Text("Recording Slot");
				if (ImGui::SliderInt("Slot", &RecordingSlot, 1, 5))
				{
					if (CheckTheMode() == true)
					{
						CRS(RecordingSlot);
					}
				}
			}

		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 70, 100, 255));
			ImGui::Text("The functions on this page will do absolutely nothing without \nThreeHook.asi. Go to File-> Open ThreeHook and open that\n file while it's loaded in the game.");
			ImGui::PopStyleColor();
		}


		ImGui::EndTabItem();

	}

}

static void gui::TheDebugStuffTab()
{
	//For Debug Stuff and keeping records.
	if (ImGui::BeginTabItem("Debug Stuff"))
	{
		GetMainPointers();
		GetPlayerData();
		GetHurtboxData();
		GetHitboxDataPart1();
		ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");
		ImGui::Separator();

		std::string Sentence = "";
		int TestEx = 0;
		DWORD Thing = 0;
		typedef int(*CheckConnectionTwo)();

		//HINSTANCE ccheck = LoadLibrary("E:\\ULTIMATE MARVEL VS. CAPCOM 3\\Scripts\\ThreeHook.asi");

		if (!ccheck)
		{
			std::cout << "could not load the dynamic library" << std::endl;
		}

		//typedef int(__stdcall* CheckCon)();

		//TestEx = 
		/*
		// resolve function address here
		CheckCon funci = (CheckCon)GetProcAddress(ccheck, "CheckConnectionTwo");
		if (!funci) {
			std::cout << "could not locate the function" << std::endl;
		}
		else
		{
			ImGui::Text("%i", TestEx);
		}
		*/
#ifdef _DEBUG

		if (ccheck != NULL)
		{
			CheckConnectionTwo lpcc = (CheckConnectionTwo)GetProcAddress((HMODULE)ccheck, "CheckConnectionTwo");
			//ImGui::Text(lpcc);
			if (!lpcc)
			{
				std::cout << "could not locate the function" << std::endl;
			}
			TestEx = lpcc();
			ImGui::Text("%i", TestEx);
		}
		else
		{
			SetLastError(Thing);

			ImGui::Text("Nothing Doing! Go to File-> Find ThreeHook and select the ThreeHook.asi.");


		}
#endif


		ImGui::Separator();

#pragma region Hurtbox Data and Coordinates

		ImGui::SeparatorText("Collision Stuff");

		//The Hurtboxes aren't really stored in the same way as the character data stuff I'm used to working with normally.
		//Have to use this funky looking method to get the characters' hurtboxes during runtime.
		uintptr_t t;
		uintptr_t tt;
		uintptr_t ttt;
		uintptr_t tttt;
		uintptr_t v;
		//std::list<uintptr_t>HurtboxPointers;
		int HurtboxCount = 0;

#pragma region P1C1

		ImGui::SeparatorText("Player 1 Character 1");
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P1C1HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P1C1Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C1Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P1C1Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P1C1Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P1C1HitboxCount; i++)
		{

			ImGui::Text("X: %f", P1C1Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C1Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P1C1Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P1C1Hitboxes[i].Radius);

		}

#pragma endregion

#pragma region P1C2

		ImGui::SeparatorText("Player 1 Character 2");
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P1C2HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P1C2Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C2Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P1C2Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P1C2Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P1C2HitboxCount; i++)
		{

			ImGui::Text("X: %f", P1C2Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C2Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P1C2Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P1C2Hitboxes[i].Radius);

		}

#pragma endregion

#pragma region P1C3

		ImGui::SeparatorText("Player 1 Character 3");
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P1C3HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P1C3Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C3Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P1C3Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P1C3Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P1C3HitboxCount; i++)
		{

			ImGui::Text("X: %f", P1C3Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P1C3Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P1C3Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P1C3Hitboxes[i].Radius);

		}

#pragma endregion

#pragma region P2C1

		ImGui::SeparatorText("Player 2 Character 1");
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P2C1HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P2C1Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C1Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P2C1Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P2C1Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P2C1HitboxCount; i++)
		{

			ImGui::Text("X: %f", P2C1Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C1Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P2C1Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P2C1Hitboxes[i].Radius);

		}

#pragma endregion

#pragma region P2C2

		ImGui::SeparatorText("Player 2 Character 2");
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P2C2HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P2C2Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C2Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P2C2Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P2C2Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P2C2HitboxCount; i++)
		{

			ImGui::Text("X: %f", P2C2Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C2Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P2C2Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P2C2Hitboxes[i].Radius);

		}

#pragma endregion

#pragma region P2C3

		ImGui::SeparatorText("Player 2 Character 3");
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x4E10), &t, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", t);
		ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
		//ImGui::Text("%x", tt);
		ImGui::Text("Hurtboxes");
		tttt = tt;

		for (int i = 0; i < P2C3HurtboxCount; i++)
		{

			ImGui::Text("X: %f", P2C3Hurtboxes[i].CollData.Coordinates.X);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C3Hurtboxes[i].CollData.Coordinates.Y);
			ImGui::SameLine();
			ImGui::Text("Z: %f", P2C3Hurtboxes[i].CollData.Coordinates.Z);
			ImGui::SameLine();
			ImGui::Text("Size: %f", P2C3Hurtboxes[i].CollData.Radius);

		}

		ImGui::Text("Hitboxes");

		for (int i = 0; i < P2C3HitboxCount; i++)
		{

			ImGui::Text("X: %f", P2C3Hitboxes[i].GlobalX);
			ImGui::SameLine();
			ImGui::Text("Y: %f", P2C3Hitboxes[i].GlobalY);
			ImGui::SameLine();
			//ImGui::Text("Z: %f", P2C3Hitboxes[i].GlobalZ);
			//ImGui::SameLine();
			ImGui::Text("Size: %f", P2C3Hitboxes[i].Radius);

		}

#pragma endregion

#pragma endregion



#pragma region GetInstallID

		GetActiveInstallData();

		//Gets install IDs and Stuff.
		ImGui::SeparatorText("Character Install State IDs");

		ImGui::Text("Player 1 Character 1:");
		ImGui::SameLine();
		ImGui::Text("%i", P1Character1ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C1InstallType5);

		ImGui::Text("Player 1 Character 2:");
		ImGui::SameLine();
		ImGui::Text("%i", P1Character2ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C2InstallType5);

		ImGui::Text("Player 1 Character 3:");
		ImGui::SameLine();
		ImGui::Text("%i", P1Character3ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P1C3InstallType5);

		ImGui::Text("Player 2 Character 1:");
		ImGui::SameLine();
		ImGui::Text("%i", P2Character1ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C1InstallType5);

		ImGui::Text("Player 2 Character 2:");
		ImGui::SameLine();
		ImGui::Text("%i", P2Character2ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C2InstallType5);

		ImGui::Text("Player 1 Character 3:");
		ImGui::SameLine();
		ImGui::Text("%i", P2Character3ID);

		ImGui::Text("Slot 1 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallID1);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallType1);

		ImGui::Text("Slot 2 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallID2);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallType2);

		ImGui::Text("Slot 3 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallID3);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallType3);

		ImGui::Text("Slot 4 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallID4);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallType4);

		ImGui::Text("Slot 5 - ");
		ImGui::SameLine();
		ImGui::Text("Install ID: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallID5);
		ImGui::SameLine();
		ImGui::Text(" Type: ");
		ImGui::SameLine();
		ImGui::Text("%x", P2C3InstallType5);


#pragma endregion

		/*
		for (size_t i = 0; i < 6; i++)
		{
			if (scriptableFighters[i].name == nullptr) {
				//ImGui::Text("p%d:%llx", i, scriptableFighters[i].fighter);
				//ImGui::Text("p%d:%llx",i, scriptableFighters[i].fighter);
			}
			else {
				//ImGui::Text("p%d:%llx", i, scriptableFighters[i].fighter);
				//ImGui::Text("p%d:%llx %llx %s", i, scriptableFighters[i].fighter, scriptableFighters[i].tickPtr, scriptableFighters[i].name);
			}


			if (scriptableFighters[i].fighter->tagState == ETagState::Active || scriptableFighters[i].fighter->tagState == ETagState::Active2)
			{
				//ImGui::Text("  X:%d", (int)scriptableFighters[i].fighter->vector.x);
				//ImGui::SameLine();
				//ImGui::Text("- Y:%d", (int)scriptableFighters[i].fighter->vector.y);
				//ImGui::SameLine();
				//ImGui::Text("- Vel:%d", (int)scriptableFighters[i].fighter->vector.xVelocity);

				//ImGui::Text("  Anim:%x", scriptableFighters[i].fighter->anim);
				//ImGui::Text("  Special:%x", scriptableFighters[i].fighter->specialState);

				//ImGui::Text("  Grounded:%s", scriptableFighters[i].GetGroundedState());
				//ImGui::Text("  TagState:%s", scriptableFighters[i].GetTagState());
			}


		}
		*/
		ImGui::EndTabItem();
	}

}

void gui::Render() noexcept
{
	ImGui::SetWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);

	ImGui::Begin(
		"UMVC3 Training Tools Mod V0.6 By Eternal Yoshi",
		&exit,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::PushFont(mainfont);

	//typedef void(*DeployTheHooks)();
	//DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");
	//dhook();


	//The Menu Bar.
	TheMenuBar();

	//Tries to hook.
	CastTheHook();

	//Hides all the options if the game isn't open.
	if (Hooked)
	{

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(70, 255, 70, 255));
		ImGui::Text("If you can read this then the tool recognizes Marvel 3 is open.");
		ImGui::Text("The version open is valid too.");
		ImGui::PopStyleColor();

		if (CheckTheMode() == true)
		{
			GetMainPointers();
			CheckIfInMatch();

			if (InMatch)
			{
				TickUpdates();
				GetActiveInstallData();

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
							SetIndividualCharacterHealth();
							SetMeters();
							GetDebugData();
							KOToggles();

							//std::thread t1(DeathDelay);
							DeathDelay();


						}


					}

					Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

					char TempByte = 0;
					ReadProcessMemory(hProcess, (LPVOID*)(0x140289c5a), &TempByte, sizeof(TempByte), 0);

					//For the Record/Playback Tick Function that's important.
					//
					// 
					//  
					//Memory::InjectHookEx(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL, hProcess);
					//TrampHookPart2( hProcess,(LPVOID*)0x140289c5a,tramp, PATCH_CALL);

					TickUpdates();
					TheExtraOptionsTab();
					TheStatusOptionsTab();
					TheCharacterOptionsTab();
#ifdef _DEBUG

					TheRecordPlaybackTab();
					TheIncomingStuffTab();
					TheDebugStuffTab();

					if (ccheck != NULL)
					{
						typedef void(*TheRecordButton)();
						TheRecordButton TRCTake1 = (TheRecordButton)GetProcAddress((HMODULE)ccheck, "TheRecordButton");
						TRCTake1();
					}

#endif

					ImGui::EndTabBar();
				}

			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 70, 255));
				ImGui::Text("Choose the characters and get into a match.");
				ImGui::PopStyleColor();

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

#ifdef _DEBUG
	if (!ccheck)
	{
		std::cout << "could not load the dynamic library" << std::endl;
		ImGui::Text("Nothing Doing! Go to File-> Find ThreeHook and select the ThreeHook.asi");
	}
#endif

	ImGui::PopFont();

	ImGui::End();

}
#pragma endregion
