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
#include <fstream>
#include <iomanip>
#include <ctime>
#include "..\utils\Trampoline.h"
#include "..\utils\addr.h"
#include "..\utils\MemoryMgr.h"
#include "..\MemoryMgr.h"
#include "..\minhook\include\MinHook.h"


#define IMGUI_ENABLE_FREETYPE

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

	
	io.Fonts->AddFontDefault();
	mainfont = io.Fonts->AddFontFromFileTTF("Resources\\fonts\\FOT-NewRodinPro-DB.otf",11.5f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(mainfont != NULL);
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
	HProc = hProcess;
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
	ImGui::Text("Thanks To SanHKHaan, Sheep, & Gneiss for finding\nthe pointers and memeory offsets that made this \npossible,and Ermmaccer for the original UMVC3Hook\nthis version is based on.");
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
	if (ImGui::BeginTabItem("Extra Settings"))
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
		
		ImGui::SeparatorText("Incomplete/Experimental Stuff");

		ImGui::Separator();

		//ImGui::Text("Game Speed");
		if (ImGui::SliderFloat("Character Speed", &CharacterSpeed, 0.01666667f, 2.0f))
		{
			SetGlobalPlayerSpeed(CharacterSpeed);

		}
		
		if (ImGui::Checkbox("Enable Game Speed Change(Use at own risk!)", &ModifyGameSpeed))
		{

		}


		if(ModifyGameSpeed)
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

		ImGui::Text("More features coming soon!");

		ImGui::EndTabItem();
	}
}

static void gui::TheStatusOptionsTab()
{
	//Player and Status Related  Settings such as Health.
	if (ImGui::BeginTabItem("Status Options"))
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
			if (ImGui::SliderFloat("P1C1 HP %", &P1Char1Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P1Char2Health)
		{
			ImGui::Text("Player 1 Character 2 Health");
			if (ImGui::SliderFloat("P1C2 HP %", &P1Char2Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P1Char3Health)
		{
			ImGui::Text("Player 1 Character 3 Health");
			if (ImGui::SliderFloat("P1C3 HP %", &P1Char3Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char1Health)
		{
			ImGui::Text("Player 2 Character 1 Health");
			if (ImGui::SliderFloat("P2C1 HP %", &P2Char1Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char2Health)
		{
			ImGui::Text("Player 2 Character 2 Health");
			if (ImGui::SliderFloat("P2C2 HP %", &P2Char2Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		if (P2Char3Health)
		{
			ImGui::Text("Player 2 Character 3 Health");
			if (ImGui::SliderFloat("P2C3 HP %", &P2Char3Health, 0.01f, 1.0f))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		ImGui::SeparatorText("Hyper Meter");

		if (P1Meter)
		{
			ImGui::Text("Player 1 Meter");
			if (ImGui::SliderInt("P1 Meter", &P1Meter, 1, 50000))
			{
				if (CheckTheMode() == true)
				{
					SetMeters();
				}
			}
		}

		if (P2Meter)
		{
			ImGui::Text("Player 2 Meter");
			if (ImGui::SliderInt("P2 Meter", &P2Meter, 1, 50000))
			{
				if (CheckTheMode() == true)
				{
					SetMeters();
				}
			}
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

static void gui::TheRecordPlaybackTab()
{
	//For Recording And Playback Stuff.
	if (ImGui::BeginTabItem("Record & Playback"))
	{

		ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");


		switch (RecordingSlot)
		{

		case 2:

			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						//recording = true;
						//replaying = false;
						recordingP1 = true;
						replayingP1 = false;
						recordingP2 = true;
						replayingP2 = false;
						recordReplayIndex2P1 = 0;
						recordReplayIndex2P2 = 0;

					}
				}
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P1")) {
					if (CheckTheMode() == true)
					{
						recordingP1 = true;
						replayingP1 = false;
						//recording = false;
						//replaying = false;
						recordingP2 = false;
						replayingP2 = false;
						recordReplayIndex2P1 = 0;
					}
				}
				ImGui::SameLine();
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P2")) {
					if (CheckTheMode() == true)
					{
						recordingP2 = true;
						replayingP2 = false;
						//recording = false;
						//replaying = false;
						recordingP1 = false;
						replayingP1 = false;
						recordReplayIndex2P2 = 0;
					}
				}
			}

			else if (recordReplayIndex2P1 > 90 || recordReplayIndex2P2 > 90) {
				if (ImGui::Button("Stop Recording")) {
					if (CheckTheMode() == true)
					{
						//recording = false;
						recordingP1 = false;
						recordingP2 = false;
						recordedLength2P1 = recordReplayIndex2P1 - 1;
						recordedLength2P2 = recordReplayIndex2P2 - 1;
					}
				}
			}

			//Playback Code Below.
			ImGui::Separator();

			if (replayAvailable2P1 && replayAvailable2P2 && !recordingP1 && !recordingP2) {
				if (!replayingP1 && !replayingP2) {
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{

							replayingP1 = true;
							replayingP2 = true;
							recordReplayIndex2 = 0;
							recordReplayIndex2P1 = 0;
							recordReplayIndex2P2 = 0;

						}

					}
				}
				else if (recordReplayIndex2P1 > 90 || recordReplayIndex2P2 > 90) {
					if (ImGui::Button("Stop Playback"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
							replayingP2 = false;

						}
					}
				}
			}

			if (replayAvailable2P1 && !recordingP1) {
				if (!replayingP1) {
					if (ImGui::Button("Playback P1")) {
						if (CheckTheMode() == true)
						{
							replayingP1 = true;
							recordReplayIndex2P1 = 0;
						}

					}
				}
				else if (recordReplayIndex2P1 > 90) {
					if (ImGui::Button("Stop Playback P1"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
						}
					}
				}
				ImGui::SameLine();
			}

			if (replayAvailable2P2 && !recordingP2) {
				if (!replayingP2) {
					if (ImGui::Button("Playback P2")) {
						if (CheckTheMode() == true)
						{
							replayingP2 = true;
							recordReplayIndex2P2 = 0;
						}

					}
				}
				else if (recordReplayIndex2P2 > 90) {
					if (ImGui::Button("Stop Playback P2"))
					{
						if (CheckTheMode() == true)
						{
							replayingP2 = false;
						}
					}
				}
			}


			ImGui::Separator();

			//Saves Recordings. Needs to be adjusted.
			if (ImGui::Button("Save P1 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable2P1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer3, 550, recordedLength2P1, pRec);
							fclose(pRec);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save P2 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable2P2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP2";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer4, 550, recordedLength2P2, pRec);
							fclose(pRec);

						}
					}
				}
			}

			if (ImGui::Button("Load Recording in P1"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP1 && !replayingP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable2P1 = true;
									recordedLength2P1 = length;
									recordReplayIndex2P1 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									//Need to fix this.
									for (int i = 0; i < recordedLength2P1; i++)
									{
										memcpy(&replayBuffer3[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Recording in P2"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP2 && !replayingP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable2P2 = true;
									recordedLength2P2 = length;
									recordReplayIndex2P2 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									for (int i = 0; i < recordedLength2P2; i++)
									{
										memcpy(&replayBuffer4[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}


			break;

		case 3:

			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						//recording = true;
						//replaying = false;
						recordingP1 = true;
						replayingP1 = false;
						recordingP2 = true;
						replayingP2 = false;
						recordReplayIndex3P1 = 0;
						recordReplayIndex3P2 = 0;

					}
				}
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P1")) {
					if (CheckTheMode() == true)
					{
						recordingP1 = true;
						replayingP1 = false;
						//recording = false;
						//replaying = false;
						recordingP2 = false;
						replayingP2 = false;
						recordReplayIndex3P1 = 0;
					}
				}
				ImGui::SameLine();
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P2")) {
					if (CheckTheMode() == true)
					{
						recordingP2 = true;
						replayingP2 = false;
						//recording = false;
						//replaying = false;
						recordingP1 = false;
						replayingP1 = false;
						recordReplayIndex3P2 = 0;
					}
				}
			}

			else if (recordReplayIndex3P1 > 90 || recordReplayIndex3P2 > 90) {
				if (ImGui::Button("Stop Recording")) {
					if (CheckTheMode() == true)
					{
						//recording = false;
						recordingP1 = false;
						recordingP2 = false;
						recordedLength3P1 = recordReplayIndex3P1 - 1;
						recordedLength3P2 = recordReplayIndex3P2 - 1;
					}
				}
			}

			//Playback Code Below.
			ImGui::Separator();

			if (replayAvailable3P1 && replayAvailable3P2 && !recordingP1 && !recordingP2) {
				if (!replayingP1 && !replayingP2) {
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{

							replayingP1 = true;
							replayingP2 = true;
							recordReplayIndex3 = 0;
							recordReplayIndex3P1 = 0;
							recordReplayIndex3P2 = 0;

						}

					}
				}
				else if (recordReplayIndex3P1 > 90 || recordReplayIndex3P2 > 90) {
					if (ImGui::Button("Stop Playback"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
							replayingP2 = false;

						}
					}
				}
			}

			if (replayAvailable3P1 && !recordingP1) {
				if (!replayingP1) {
					if (ImGui::Button("Playback P1")) {
						if (CheckTheMode() == true)
						{
							replayingP1 = true;
							recordReplayIndex3P1 = 0;
						}

					}
				}
				else if (recordReplayIndex3P1 > 90) {
					if (ImGui::Button("Stop Playback P1"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
						}
					}
				}
				ImGui::SameLine();
			}

			if (replayAvailable3P2 && !recordingP2) {
				if (!replayingP2) {
					if (ImGui::Button("Playback P2")) {
						if (CheckTheMode() == true)
						{
							replayingP2 = true;
							recordReplayIndex3P2 = 0;
						}

					}
				}
				else if (recordReplayIndex3P2 > 90) {
					if (ImGui::Button("Stop Playback P2"))
					{
						if (CheckTheMode() == true)
						{
							replayingP2 = false;
						}
					}
				}
			}


			ImGui::Separator();

			//Saves Recordings. Needs to be adjusted.
			if (ImGui::Button("Save P1 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable3P1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer5, 550, recordedLength3P1, pRec);
							fclose(pRec);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save P2 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable3P2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP2";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer6, 550, recordedLength3P2, pRec);
							fclose(pRec);

						}
					}
				}
			}

			if (ImGui::Button("Load Recording in P1"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP1 && !replayingP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable3P1 = true;
									recordedLength3P1 = length;
									recordReplayIndex3P1 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									//Need to fix this.
									for (int i = 0; i < recordedLength3P1; i++)
									{
										memcpy(&replayBuffer5[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Recording in P2"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP2 && !replayingP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable3P2 = true;
									recordedLength3P2 = length;
									recordReplayIndex3P2 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									for (int i = 0; i < recordedLength3P2; i++)
									{
										memcpy(&replayBuffer6[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}


			break;

		case 4:

			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						//recording = true;
						//replaying = false;
						recordingP1 = true;
						replayingP1 = false;
						recordingP2 = true;
						replayingP2 = false;
						recordReplayIndex4P1 = 0;
						recordReplayIndex4P2 = 0;

					}
				}
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P1")) {
					if (CheckTheMode() == true)
					{
						recordingP1 = true;
						replayingP1 = false;
						//recording = false;
						//replaying = false;
						recordingP2 = false;
						replayingP2 = false;
						recordReplayIndex4P1 = 0;
					}
				}
				ImGui::SameLine();
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P2")) {
					if (CheckTheMode() == true)
					{
						recordingP2 = true;
						replayingP2 = false;
						//recording = false;
						//replaying = false;
						recordingP1 = false;
						replayingP1 = false;
						recordReplayIndex4P2 = 0;
					}
				}
			}

			else if (recordReplayIndex4P1 > 90 || recordReplayIndex4P2 > 90) {
				if (ImGui::Button("Stop Recording")) {
					if (CheckTheMode() == true)
					{
						//recording = false;
						recordingP1 = false;
						recordingP2 = false;
						recordedLength4P1 = recordReplayIndex4P1 - 1;
						recordedLength4P2 = recordReplayIndex4P2 - 1;
					}
				}
			}

			//Playback Code Below.
			ImGui::Separator();

			if (replayAvailable4P1 && replayAvailable4P2 && !recordingP1 && !recordingP2) {
				if (!replayingP1 && !replayingP2) {
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{

							replayingP1 = true;
							replayingP2 = true;
							recordReplayIndex4 = 0;
							recordReplayIndex4P1 = 0;
							recordReplayIndex4P2 = 0;

						}

					}
				}
				else if (recordReplayIndex4P1 > 90 || recordReplayIndex4P2 > 90) {
					if (ImGui::Button("Stop Playback"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
							replayingP2 = false;

						}
					}
				}
			}

			if (replayAvailable4P1 && !recordingP1) {
				if (!replayingP1) {
					if (ImGui::Button("Playback P1")) {
						if (CheckTheMode() == true)
						{
							replayingP1 = true;
							recordReplayIndex4P1 = 0;
						}

					}
				}
				else if (recordReplayIndex4P1 > 90) {
					if (ImGui::Button("Stop Playback P1"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
						}
					}
				}
				ImGui::SameLine();
			}

			if (replayAvailable4P2 && !recordingP2) {
				if (!replayingP2) {
					if (ImGui::Button("Playback P2")) {
						if (CheckTheMode() == true)
						{
							replayingP2 = true;
							recordReplayIndex4P2 = 0;
						}

					}
				}
				else if (recordReplayIndex4P2 > 90) {
					if (ImGui::Button("Stop Playback P2"))
					{
						if (CheckTheMode() == true)
						{
							replayingP2 = false;
						}
					}
				}
			}


			ImGui::Separator();

			//Saves Recordings. Needs to be adjusted.
			if (ImGui::Button("Save P1 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable4P1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer7, 550, recordedLength4P1, pRec);
							fclose(pRec);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save P2 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable4P2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP2";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer8, 550, recordedLength4P2, pRec);
							fclose(pRec);

						}
					}
				}
			}

			if (ImGui::Button("Load Recording in P1"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP1 && !replayingP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable4P1 = true;
									recordedLength4P1 = length;
									recordReplayIndex4P1 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									//Need to fix this.
									for (int i = 0; i < recordedLength4P1; i++)
									{
										memcpy(&replayBuffer7[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Recording in P2"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP2 && !replayingP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable4P2 = true;
									recordedLength4P2 = length;
									recordReplayIndex4P2 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									for (int i = 0; i < recordedLength4P2; i++)
									{
										memcpy(&replayBuffer8[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}


			break;

		case 5:

			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						//recording = true;
						//replaying = false;
						recordingP1 = true;
						replayingP1 = false;
						recordingP2 = true;
						replayingP2 = false;
						recordReplayIndex5P1 = 0;
						recordReplayIndex5P2 = 0;

					}
				}
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P1")) {
					if (CheckTheMode() == true)
					{
						recordingP1 = true;
						replayingP1 = false;
						//recording = false;
						//replaying = false;
						recordingP2 = false;
						replayingP2 = false;
						recordReplayIndex5P1 = 0;
					}
				}
				ImGui::SameLine();
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P2")) {
					if (CheckTheMode() == true)
					{
						recordingP2 = true;
						replayingP2 = false;
						//recording = false;
						//replaying = false;
						recordingP1 = false;
						replayingP1 = false;
						recordReplayIndex5P2 = 0;
					}
				}
			}

			else if (recordReplayIndex5P1 > 90 || recordReplayIndex5P2 > 90) {
				if (ImGui::Button("Stop Recording")) {
					if (CheckTheMode() == true)
					{
						//recording = false;
						recordingP1 = false;
						recordingP2 = false;
						recordedLength5P1 = recordReplayIndex5P1 - 1;
						recordedLength5P2 = recordReplayIndex5P2 - 1;
					}
				}
			}

			//Playback Code Below.
			ImGui::Separator();

			if (replayAvailable5P1 && replayAvailable5P2 && !recordingP1 && !recordingP2) {
				if (!replayingP1 && !replayingP2) {
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{

							replayingP1 = true;
							replayingP2 = true;
							recordReplayIndex5 = 0;
							recordReplayIndex5P1 = 0;
							recordReplayIndex5P2 = 0;

						}

					}
				}
				else if (recordReplayIndex5P1 > 90 || recordReplayIndex5P2 > 90) {
					if (ImGui::Button("Stop Playback"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
							replayingP2 = false;

						}
					}
				}
			}

			if (replayAvailable5P1 && !recordingP1) {
				if (!replayingP1) {
					if (ImGui::Button("Playback P1")) {
						if (CheckTheMode() == true)
						{
							replayingP1 = true;
							recordReplayIndex5P1 = 0;
						}

					}
				}
				else if (recordReplayIndex5P1 > 90) {
					if (ImGui::Button("Stop Playback P1"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
						}
					}
				}
				ImGui::SameLine();
			}

			if (replayAvailable5P2 && !recordingP2) {
				if (!replayingP2) {
					if (ImGui::Button("Playback P2")) {
						if (CheckTheMode() == true)
						{
							replayingP2 = true;
							recordReplayIndex5P2 = 0;
						}

					}
				}
				else if (recordReplayIndex5P2 > 90) {
					if (ImGui::Button("Stop Playback P2"))
					{
						if (CheckTheMode() == true)
						{
							replayingP2 = false;
						}
					}
				}
			}


			ImGui::Separator();

			//Saves Recordings. Needs to be adjusted.
			if (ImGui::Button("Save P1 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable5P1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer9, 550, recordedLength5P1, pRec);
							fclose(pRec);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save P2 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailable5P2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP2";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer10, 550, recordedLength5P2, pRec);
							fclose(pRec);

						}
					}
				}
			}

			if (ImGui::Button("Load Recording in P1"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP1 && !replayingP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable5P1 = true;
									recordedLength5P1 = length;
									recordReplayIndex5P1 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									//Need to fix this.
									for (int i = 0; i < recordedLength5P1; i++)
									{
										memcpy(&replayBuffer9[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Recording in P2"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP2 && !replayingP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailable5P2 = true;
									recordedLength5P2 = length;
									recordReplayIndex5P2 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									for (int i = 0; i < recordedLength5P2; i++)
									{
										memcpy(&replayBuffer10[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}

			break;

		case 1:
		default:

			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record Both")) {
					if (CheckTheMode() == true)
					{
						//recording = true;
						//replaying = false;
						recordingP1 = true;
						replayingP1 = false;
						recordingP2 = true;
						replayingP2 = false;
						//recordReplayIndex = 0;
						recordReplayIndexP1 = 0;
						recordReplayIndexP2 = 0;

					}
				}
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P1")) {
					if (CheckTheMode() == true)
					{
						recordingP1 = true;
						replayingP1 = false;
						//recording = false;
						//replaying = false;
						recordingP2 = false;
						replayingP2 = false;
						recordReplayIndexP1 = 0;
					}
				}
				ImGui::SameLine();
			}
			//ImGui::SameLine();
			if (!recordingP1 && !recordingP2) {
				if (ImGui::Button("Record P2")) {
					if (CheckTheMode() == true)
					{
						recordingP2 = true;
						replayingP2 = false;
						//recording = false;
						//replaying = false;
						recordingP1 = false;
						replayingP1 = false;
						recordReplayIndexP2 = 0;
					}
				}
			}

			else if (recordReplayIndexP1 > 90 || recordReplayIndexP2 > 90) {
				if (ImGui::Button("Stop Recording")) {
					if (CheckTheMode() == true)
					{
						//recording = false;
						recordingP1 = false;
						recordingP2 = false;
						recordedLengthP1 = recordReplayIndexP1 - 1;
						recordedLengthP2 = recordReplayIndexP2 - 1;
					}
				}
			}

			//Playback Code Below.
			ImGui::Separator();

			if (replayAvailableP1 && replayAvailableP2 && !recordingP1 && !recordingP2) {
				if (!replayingP1 && !replayingP2) {
					if (ImGui::Button("Playback Both")) {
						if (CheckTheMode() == true)
						{

							replayingP1 = true;
							replayingP2 = true;
							recordReplayIndex = 0;
							recordReplayIndexP1 = 0;
							recordReplayIndexP2 = 0;

						}

					}
				}
				else if (recordReplayIndexP1 > 90 || recordReplayIndexP2 > 90) {
					if (ImGui::Button("Stop Playback"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
							replayingP2 = false;

						}
					}
				}
			}

			if (replayAvailableP1 && !recordingP1) {
				if (!replayingP1) {
					if (ImGui::Button("Playback P1")) {
						if (CheckTheMode() == true)
						{
							replayingP1 = true;
							recordReplayIndexP1 = 0;
						}

					}
				}
				else if (recordReplayIndexP1 > 90) {
					if (ImGui::Button("Stop Playback P1"))
					{
						if (CheckTheMode() == true)
						{
							replayingP1 = false;
						}
					}
				}
				ImGui::SameLine();
			}

			if (replayAvailableP2 && !recordingP2) {
				if (!replayingP2) {
					if (ImGui::Button("Playback P2")) {
						if (CheckTheMode() == true)
						{
							replayingP2 = true;
							recordReplayIndexP2 = 0;
						}

					}
				}
				else if (recordReplayIndexP2 > 90) {
					if (ImGui::Button("Stop Playback P2"))
					{
						if (CheckTheMode() == true)
						{
							replayingP2 = false;
						}
					}
				}
			}


			ImGui::Separator();

			//Saves Recordings. Needs to be adjusted.
			if (ImGui::Button("Save P1 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailableP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer, 550, recordedLengthP1, pRec);
							fclose(pRec);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save P2 Recording"))
			{
				if (CheckTheMode() == true)
				{
					if (!recording && replayAvailableP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };

						char szFile[_MAX_PATH] = "RecP2";
						const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetSaveFileName(&ofn))
						{
							pRec = fopen(ofn.lpstrFile, "wb");
							fwrite(replayBuffer2, 550, recordedLengthP2, pRec);
							fclose(pRec);

						}
					}
				}
			}

			//Loads Recordings. To Be Continued....
			if (ImGui::Button("Load Recording in P1"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP1 && !replayingP1)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailableP1 = true;
									recordedLengthP1 = length;
									recordReplayIndexP1 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									//Need to fix this.
									for (int i = 0; i < recordedLengthP1; i++)
									{

										memcpy(&replayBuffer[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Recording in P2"))
			{
				if (CheckTheMode() == true)
				{
					if (!recordingP2 && !replayingP2)
					{
						OPENFILENAME ofn = { sizeof(OPENFILENAME) };
						char szFile[_MAX_PATH] = "RecP1";
						const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
						ofn.hwndOwner = GetConsoleWindow();
						ofn.lpstrFile = szFile; // <--------------------- initial file name
						ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
						ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
						ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

						if (GetOpenFileName(&ofn))
						{
							if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
							{
								MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

							}
							else
							{
								//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
								std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
								mRP.unsetf(std::ios::skipws);
								std::ifstream::pos_type pos = mRP.tellg();
								int filesize = pos;

								if (filesize % 550 != 0)
								{
									MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
								}
								else
								{
									//Gets the file size, sets the needed variables to play the replay when loaded, 
									//and then copies the file into an array in memory.
									int length = pos / 550;
									replayAvailableP2 = true;
									recordedLengthP2 = length;
									recordReplayIndexP2 = 0;

									mRP.seekg(0, std::ios::beg);

									std::vector<BYTE> pChars;
									pChars.reserve(filesize);

									pChars.insert(pChars.begin(),
										std::istream_iterator<BYTE>(mRP),
										std::istream_iterator<BYTE>());

									//For loop for inserting the frames in the proper place. Based on code from recording.
									for (int i = 0; i < recordedLengthP2; i++)
									{

										memcpy(&replayBuffer2[i], &pChars[(i * 550)], ReplayBufferSize);
									}
								}


							}

						}


					}
				}
			}

			break;
		}

		if (RecordingSlot)
		{
			ImGui::Text("Recording Slot");
			if (ImGui::SliderInt("Slot", &RecordingSlot, 1, 5))
			{
				if (CheckTheMode() == true)
				{

				}
			}
		}

		//auto sigger = sigscan::get();

		ImGui::EndTabItem();

	}

}

static void gui::TheDebugStuffTab()
{
	//For Debug Stuff and keeping records.
	if (ImGui::BeginTabItem("Debug Stuff"))
	{
		GetPlayerData();
		GetHurtboxData();
		GetHitboxDataPart1();
		ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");
		ImGui::Separator();
		
		std::string Sentence = "";
		int TestEx = 0;
		DWORD Thing = 0;
		typedef int(*CheckConnectionTwo)();

		auto ccheck = LoadLibrary("E:\\ULTIMATE MARVEL VS. CAPCOM 3\\Scripts\\ThreeHook.asi");

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

		if (ccheck != NULL) 
		{
			CheckConnectionTwo lpcc = (CheckConnectionTwo)GetProcAddress((HMODULE)ccheck, "CheckConnectionTwo");
			//ImGui::Text(lpcc);
			if(!lpcc)
			{
				std::cout << "could not locate the function" << std::endl;
			}
			TestEx = lpcc();
			ImGui::Text("%i", TestEx);
		}
		else
		{
			SetLastError(Thing);
			ImGui::Text("Nothing Doing!");
		}
		
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
		"UMVC3 Training Tools Mod V0.4X By Eternal Yoshi",
		&exit,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::PushFont(mainfont);

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
					}


				}
				
				Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

				char TempByte = 0;
				ReadProcessMemory(hProcess, (LPVOID*)(0x140289c5a), &TempByte, sizeof(TempByte), 0);

				//Memory::InjectHookEx(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL, hProcess);
				//TrampHookPart2( hProcess,(LPVOID*)0x140289c5a,tramp, PATCH_CALL);

				TickUpdates();
				TheExtraOptionsTab();
				TheStatusOptionsTab();
				TheCharacterOptionsTab();
				TheRecordPlaybackTab();
				TheDebugStuffTab();

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

	ImGui::PopFont();

	ImGui::End();

}
#pragma endregion
