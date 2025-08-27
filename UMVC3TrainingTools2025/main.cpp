#define SOKOL_IMPL
#define SOKOL_D3D11
#define SOKOL_GLUE_IMPL
#define SOKOL_IMGUI_IMPL
#include "imgui.h"
#include "sokol_app.h"
#define SOKOL_DEBUG
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol/util/sokol_imgui.h"
#include "code/umvc3utils.cpp"
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
#include "code/proc.h"
#include "code/proc.cpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "code/Localizer.h"

/*std::ifstream inFile;
inFile.open("");*/

//For the CSV File.
//std::ifstream CSVFile;
std::string CSVPath;
LocalizationMap LocalMap;
std::string DefaultLanguage = "English";
std::string UserLangauge = "";
//LocalizationManager localMan;
//std::unordered_map<int, std::string> localizedStrings;
//static std::string getLocalizedString(int id) {
//	return localizedStrings[id];
//}

//#define LOCSTR(token) hash(#token)

//static constexpr int hash(const char* string) {
//	return string % 0xFFFF; // replace with a real hash function though
//}

static HINSTANCE ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));

bool ShowAboutWindow = false;
bool Hooked = false;
bool InMatch = false;
int GameModeRef = 0;
int RecordingSlot = 1;

static struct {
	uint64_t last_time;
} state;
static sg_pass default_pass;

static void init(void) {

	sg_desc desc = {};
	desc.environment = sglue_environment();

	sg_setup(&desc);
	if (!sg_isvalid()) {
		fprintf(stderr, "Failed to create Sokol GFX context!\n");
		exit(-1);
	}
	stm_setup();
	simgui_desc_t simgui_desc = {};
	simgui_desc.no_default_font = true;
	simgui_setup(&simgui_desc);

	ImGuiIO& io = ImGui::GetIO();

	//Gets the base project path, the adds the rest of the font path to load it properly. Thanks Remy Lebeau.
	std::filesystem::path BaseProjectPath = std::filesystem::current_path();
	std::string MainFontPath = BaseProjectPath.string() + "\\Resources\\FontToUse.ttf";
	ImFont* font1 = io.Fonts->AddFontFromFileTTF(MainFontPath.c_str(), 17.0f);

	//For the.csv file.
	CSVPath = BaseProjectPath.string() + "\\Resources\\3TTAppText.csv";
	//Check if it exists.
	if(std::filesystem::exists(CSVPath))
	{
		LocalMap = loadLocalizationCSV(CSVPath);

		//Gets the User's Language.
		std::string UIUserLangauge = getUserUILanguage();
		std::string ShortLang = UIUserLangauge.substr(0, 2);
		//switch(ShortLang)
		//{
		//default:
		//	break;
		//}

	}
	else{
		fprintf(stderr, "Unable to open 3TTAppText.csv!\n");
		exit(-1);
	}

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	//colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	//colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


}

#pragma region MenuTabs
//ImGui menu stuff begins here.

static void CheckIfInMatch()
{

	if (Player1CharNodeTree == 0 && Player2CharNodeTree == 0)
	{
		InMatch = false;
	}
	else
	{
		InMatch = true;
	}
}

void TheAboutWindow(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(0, 25));
	ImGui::SetNextWindowSize(ImVec2((float)sapp_width(), ((float)sapp_height() - 100)));
	if (!ImGui::Begin("UMVC3 Training Tools Mod V0.8 Beta", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("2023 - 2025 By Eternal Yoshi");
	ImGui::Separator();
	ImGui::Text("Thanks To SanHKHaan, Sheep, Techs, anotak, & Gneiss\n for finding the pointers and memeory offsets that\nmade this possible, and Ermmaccer for the\noriginal UMVC3Hook.\n");
	ImGui::Separator();
	ImGui::Text("In case it isn't obvious, this tool is NOT designed to\nenable cheating in online multiplayer and\nis intended to only function in Training Mode.");

	if (ImGui::Button("OK"))
		ShowAboutWindow = false;

	ImGui::End();

}

//My attempt to port hooking code starts here.
static void CastTheHook()
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
		if (ValidVer)Hooked = true;
	}
	else
	{
		Hooked = false;
	}
	HProc = hProcess;
}

static void TheMenuBar()
{
	if (ShowAboutWindow) TheAboutWindow(&ShowAboutWindow);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

#ifdef _DEBUG
			//if (ImGui::MenuItem(("Find ThreeHook"), "CTRL+O"))
			//{
			//    std::string ChosenFile = "";
			//    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
			//    char szFile[_MAX_PATH] = "ThreeHook";
			//    const char szExt[] = ".asi\0"; // extra '\0' for lpstrFilter
			//    ofn.hwndOwner = GetConsoleWindow();
			//    ofn.lpstrFile = szFile; // <--------------------- initial file name
			//    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
			//    ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
			//    ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
			//    if (GetOpenFileName(&ofn))
			//    {
			//        ChosenFile = ofn.lpstrFile;
			//        ThreeHookPath = ChosenFile;
			//        ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));
			//        if (ccheck)
			//        {
			//            typedef void(*DeployTheHooks)();
			//            DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");
			//            dhook();
			//        }



			//    }
			//}
#endif

			if (ImGui::MenuItem(("Reset Settings To Default")))
			{
				if (Hooked)
				{
					ResetSettings();
				}
			}

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

static void TheCharacterOptionsTab()
{
	GetMainPointers();
	if (ImGui::BeginTabItem((getLocalizedString(LocalMap, "P3_CHARACTER", DefaultLanguage)).c_str()))
	{

		ImGui::Text((getLocalizedString(LocalMap, "P3_REMEMBER", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_FRANK", DefaultLanguage)).c_str());

		if (FrankLevel)
		{
			ImGui::Text((getLocalizedString(LocalMap, "P3_FRANKLEVEL", DefaultLanguage)).c_str());
			if (ImGui::SliderInt("Lv", &FrankLevel, 1, 5))
			{
				ChangeFrankLevel(FrankLevel);
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
				ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_FRANKLEVELD", DefaultLanguage)).c_str());
		}


#ifdef _DEBUG

		//ImGui::SeparatorText("Vergil");
		//if (ImGui::Checkbox("Endless Spiral Swords", &VergilSpiralSwordsForever))
		//{

		//}
#endif


		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_MRWRIGHT", DefaultLanguage)).c_str());

		ImGui::Text((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCE", DefaultLanguage)).c_str());
		if (WrightEvidenceA || WrightEvidenceB || WrightEvidenceC)
		{
			ImGui::Text((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCE1", DefaultLanguage)).c_str());

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
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
				ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCED", DefaultLanguage)).c_str());

			ImGui::Text((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCE2", DefaultLanguage)).c_str());

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
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
				ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCED", DefaultLanguage)).c_str());

			ImGui::Text((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCE3", DefaultLanguage)).c_str());

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
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
				ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MRWRIGHTEVIDENCED", DefaultLanguage)).c_str());
		}

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_MRWRIGHTLOCKEVIDENCE", DefaultLanguage)).c_str(), &LockEvidence))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MRWRIGHTLOCKEVIDENCED", DefaultLanguage)).c_str());


		//ImGui::Text("Turnabout Toggle");
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_MRWRIGHTTURNABOUTT", DefaultLanguage)).c_str(), &Turnabout))
		{
			if (CheckTheMode() == true)
			{
				Objection();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MRWRIGHTTURNABOUTTD", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_JEAN", DefaultLanguage)).c_str());
		//ImGui::Text("Dark Phoenix Toggle");
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_JEANDARKPHOENIX", DefaultLanguage)).c_str(), &DarkPhoenix))
		{
			if (CheckTheMode() == true)
			{
				PopTheBird();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_JEANDARKPHOENIXD", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_MODOK", DefaultLanguage)).c_str());
		if (MODOKLOU)
		{
			ImGui::Text((getLocalizedString(LocalMap, "P3_MODOKLOU", DefaultLanguage)).c_str());
			if (ImGui::SliderInt("LOU", &MODOKLOU, 1, 10))
			{
				if (CheckTheMode() == true)
				{
					ChangeMODOKUnderstanding(MODOKLOU);
				}
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MODOKLOUD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_MODOKLOCKU", DefaultLanguage)).c_str(), &LockMODOKLOU))
		{
			if (CheckTheMode() == true)
			{
				ChangeMODOKUnderstanding(MODOKLOU);
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_MODOKLOCKUD", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_DORMAMMU", DefaultLanguage)).c_str());
		ImGui::Text((getLocalizedString(LocalMap, "P3_DORMAMMUSPELL", DefaultLanguage)).c_str());

		ImGui::Text((getLocalizedString(LocalMap, "P3_DORMAMMUSPELLRED", DefaultLanguage)).c_str());
		if (ImGui::SliderInt((getLocalizedString(LocalMap, "P3_DORMRED", DefaultLanguage)).c_str(), &DormRed, 0, 3))
		{
			if (CheckTheMode() == true)
			{
				DormSpellSet = true;
				SetDormSpellLevels();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_DORMAMMUSPELLREDD", DefaultLanguage)).c_str());

		ImGui::Text((getLocalizedString(LocalMap, "P3_DORMAMMUSPELLBLUE", DefaultLanguage)).c_str());
		if (ImGui::SliderInt((getLocalizedString(LocalMap, "P3_DORMBLUE", DefaultLanguage)).c_str(), &DormBlue, 0, 3))
		{
			if (CheckTheMode() == true)
			{
				DormSpellSet = true;
				SetDormSpellLevels();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_DORMAMMUSPELLBLUED", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_DEADPOOL", DefaultLanguage)).c_str());
		//ImGui::Text("Deadpool Teleport");
		if (ImGui::SliderInt((getLocalizedString(LocalMap, "P3_DEADPOOLTELEPORTCOUNT", DefaultLanguage)).c_str(), &DeadpoolTeleportCount, 0, 2))
		{
			if (CheckTheMode() == true)
			{
				SetDeadpoolTeleport();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_DEADPOOLTELEPORTCOUNTD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_DEADPOOLTELEPORTLOCK", DefaultLanguage)).c_str(), &FreezeDeadpoolTPCounter))
		{
			if (CheckTheMode() == true)
			{
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_DEADPOOLTELEPORTLOCKD", DefaultLanguage)).c_str());


		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_WESKER", DefaultLanguage)).c_str());

		if (ImGui::Button((getLocalizedString(LocalMap, "P3_WESKERBREAKGLASSES", DefaultLanguage)).c_str()))
		{
			if (CheckTheMode() == true)
			{
				BreakWeskerGlasses();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_WESKERBREAKGLASSESD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_WESKERBREAKATSTART", DefaultLanguage)).c_str(), &BreakGlassesAtRestart))
		{

		}


		ImGui::SeparatorText((getLocalizedString(LocalMap, "P3_ETC", DefaultLanguage)).c_str());
		//ImGui::Text("Endless Install Toggle");
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P3_ENDLESSINSTALLS", DefaultLanguage)).c_str(), &EndlessInstalls))
		{
			if (CheckTheMode() == true)
			{
				EndlessInstallBoolUpdate();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P3_ENDLESSINSTALLSD", DefaultLanguage)).c_str());

		ImGui::Separator();
		ImGui::EndTabItem();

	}

}

static void TheExtraOptionsTab()
{
	GetMainPointers();
	//if (ImGui::BeginTabItem("Extra"))
	if (ImGui::BeginTabItem((getLocalizedString(LocalMap, "P1_EXTRA", DefaultLanguage)).c_str()))
	{
		ImGui::Separator();

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_RESTARTINTRO", DefaultLanguage)).c_str()))
		{
			if (CheckTheMode() == true)
			{
				//*(unsigned char*)(block2 + 0x118) = 3;
				int RestartValue = 1;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(sAction + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else
				{
					val = 3;
				}
			}
		}

		ImGui::Text((getLocalizedString(LocalMap, "P1_RESTARTINTROD", DefaultLanguage)).c_str());

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_RESTARTREADY", DefaultLanguage)).c_str()))
		{
			if (CheckTheMode() == true)
			{
				//*(unsigned char*)(block2 + 0x118) = 3;
				int RestartValue = 2;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(sAction + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else
				{
					val = 3;
				}
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_RESTARTREADYD", DefaultLanguage)).c_str());

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_RESTARTTRAINING", DefaultLanguage)).c_str()))
		{
			if (CheckTheMode() == true)
			{
				int RestartValue = 3;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(sAction + 0x118), &RestartValue, sizeof(int), NULL))
				{

				}
				else
				{
					val = 3;
				}
			}
		}

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P1_POSITIONING", DefaultLanguage)).c_str());

		ImGui::Text((getLocalizedString(LocalMap, "P1_PLAYER1POSITION", DefaultLanguage)).c_str());
		if (ImGui::DragFloat(((getLocalizedString(LocalMap, "P1_PLAYER1XPOSITION", DefaultLanguage)).c_str()), &p1Pos, 1.0f, -800.0f, 800.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
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

		ImGui::Text((getLocalizedString(LocalMap, "P1_PLAYER2POSITION", DefaultLanguage)).c_str());
		if (ImGui::DragFloat(((getLocalizedString(LocalMap, "P1_PLAYER2XPOSITION", DefaultLanguage)).c_str()), &p2Pos, 1.0f, -800.0f, 800.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
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

		ImGui::Text((getLocalizedString(LocalMap, "P1_POSITIONPRESETS", DefaultLanguage)).c_str());
		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSDEFAULT", DefaultLanguage)).c_str()))
		{

			p1Pos = -125.0f;
			p2Pos = 125.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSDEFAULT2P", DefaultLanguage)).c_str()))
		{

			p1Pos = 125.0f;
			p2Pos = -125.0f;


		}
		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSLEFT", DefaultLanguage)).c_str()))
		{

			p1Pos = -750.0f;
			p2Pos = -800.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSLEFT2P", DefaultLanguage)).c_str()))
		{

			p1Pos = -800.0f;
			p2Pos = -750.0f;


		}

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSRIGHT", DefaultLanguage)).c_str()))
		{

			p1Pos = 750.0f;
			p2Pos = 800.0f;


		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P1_POSRIGHT2P", DefaultLanguage)).c_str()))
		{

			p1Pos = 800.0f;
			p2Pos = 750.0f;


		}

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P1_ETC", DefaultLanguage)).c_str());

		if (ImGui::Checkbox(((getLocalizedString(LocalMap, "P1_ENDLESSX1P", DefaultLanguage)).c_str()), &EndlessXFactorP1))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_ENDLESSX1PD", DefaultLanguage)).c_str());

		ImGui::SameLine();

		if (ImGui::Checkbox(((getLocalizedString(LocalMap, "P1_ENDLESSX2P", DefaultLanguage)).c_str()), &EndlessXFactorP2))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_ENDLESSX2PD", DefaultLanguage)).c_str());
		ImGui::Separator();

		//ImGui::SeparatorText("Etc.");

		if (ImGui::Checkbox(((getLocalizedString(LocalMap, "P1_MOVEINPUTSTOLEFT", DefaultLanguage)).c_str()), &MoveInputDisplay))
		{
			LeftSideInputDisplay();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_MOVEINPUTSTOLEFTD", DefaultLanguage)).c_str());

		ImGui::Separator();

		//ImGui::Text("Game Speed");
		if (ImGui::DragFloat(((getLocalizedString(LocalMap, "P1_CHARACTERSPEEDP1", DefaultLanguage)).c_str()), &P1Char1Speed, 0.01f, 0.01666667f, 2.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_CHARACTERSPEEDP1D", DefaultLanguage)).c_str());

		if (ImGui::DragFloat(((getLocalizedString(LocalMap, "P1_CHARACTERSPEEDP2", DefaultLanguage)).c_str()), &P2Char1Speed, 0.01f, 0.01666667f, 2.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P1_CHARACTERSPEEDP2D", DefaultLanguage)).c_str());

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_NORMALSPEEDP1", DefaultLanguage)).c_str()))
		{
			P1Char1Speed = 1;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		ImGui::SameLine();

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_NORMALSPEEDP2", DefaultLanguage)).c_str()))
		{
			P2Char1Speed = 1;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_VJOESLOWSPEEDP1", DefaultLanguage)).c_str()))
		{
			P1Char1Speed = 0.75;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		ImGui::SameLine();

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_VJOESLOWSPEEDP2", DefaultLanguage)).c_str()))
		{
			P2Char1Speed = 0.75;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_VALEOFMISTSPEEDP1", DefaultLanguage)).c_str()))
		{
			P1Char1Speed = 0.66666666667;
			SetGlobalPlayerSpeed(P1Char1Speed);
		}

		ImGui::SameLine();

		if (ImGui::Button((getLocalizedString(LocalMap, "P1_VALEOFMISTSPEEDP2", DefaultLanguage)).c_str()))
		{
			P2Char1Speed = 0.66666666667;
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

		ImGui::Text((getLocalizedString(LocalMap, "P1_MORESOON", DefaultLanguage)).c_str());

		ImGui::EndTabItem();
	}
}

static void TheStatusOptionsTab()
{
	GetMainPointers();
	//Player and Status Related  Settings such as Health.
	if (ImGui::BeginTabItem((getLocalizedString(LocalMap,"P2_STATUS", DefaultLanguage)).c_str()))
	{
		ImGui::SeparatorText((getLocalizedString(LocalMap, "P2_CHARACTERHP", DefaultLanguage)).c_str());

		//Toggle for using individual character health.
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_SETINDIVIDUALHP", DefaultLanguage)).c_str(), &SetIndividualHP))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_SETINDIVIDUALHPD", DefaultLanguage)).c_str());

		//Toggle for Applying Health Settings to Red Health.
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_APPLYREDHPP1", DefaultLanguage)).c_str(), &AlsoSetRedHealthP1))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_APPLYREDHPP1D", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_APPLYREDHPP2", DefaultLanguage)).c_str(), &AlsoSetRedHealthP2))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_APPLYREDHPP2D", DefaultLanguage)).c_str());

		if (P1Char1Health)
		{
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP1C1", DefaultLanguage)).c_str());
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
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP1C2", DefaultLanguage)).c_str());
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
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP1C3", DefaultLanguage)).c_str());
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
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP2C1", DefaultLanguage)).c_str());
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
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP2C2", DefaultLanguage)).c_str());
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
			ImGui::Text((getLocalizedString(LocalMap, "P2_HEALTHP2C3", DefaultLanguage)).c_str());
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

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P2_HYPERMETER", DefaultLanguage)).c_str());

		//if (P1Meter)
		//{
		ImGui::Text((getLocalizedString(LocalMap, "P2_PLAYER1METER", DefaultLanguage)).c_str());
		if (ImGui::DragInt((getLocalizedString(LocalMap, "P2_P1METER", DefaultLanguage)).c_str(), &P1Meter, 100.0f, 0, 50000, "%d"))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_P1METERD", DefaultLanguage)).c_str());
		//}

		//if (P2Meter)
		//{
		ImGui::Text((getLocalizedString(LocalMap, "P2_PLAYER2METER", DefaultLanguage)).c_str());
		if (ImGui::DragInt((getLocalizedString(LocalMap, "P2_P2METER", DefaultLanguage)).c_str(), &P2Meter, 100.0f, 0, 50000, "%d"))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_P2METERD", DefaultLanguage)).c_str());
		//}

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_METERLOCKP1", DefaultLanguage)).c_str(), &LockP1Meter))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_METERLOCKP1D", DefaultLanguage)).c_str());

		ImGui::SameLine();

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_METERLOCKP2", DefaultLanguage)).c_str(), &LockP2Meter))
		{

		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_METERLOCKP2D", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P2_JAMMINGBOMB", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP1C1", DefaultLanguage)).c_str(), &P1C1Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP1C2", DefaultLanguage)).c_str(), &P1C2Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP1C3", DefaultLanguage)).c_str(), &P1C3Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP2C1", DefaultLanguage)).c_str(), &P2C1Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP2C2", DefaultLanguage)).c_str(), &P2C2Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P2_JAMP2C3", DefaultLanguage)).c_str(), &P2C3Jammed))
		{
			if (CheckTheMode() == true)
			{
				JammingToggle();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P2_JAMD", DefaultLanguage)).c_str());

		ImGui::Separator();

		ImGui::Text((getLocalizedString(LocalMap, "P1_MORESOON", DefaultLanguage)).c_str());

		ImGui::EndTabItem();

	}
}

static void TheIncomingStuffTab()
{
	GetMainPointers();
	if (ImGui::BeginTabItem((getLocalizedString(LocalMap, "P5_INCOMING", DefaultLanguage)).c_str()))
	{
		ImGui::Text((getLocalizedString(LocalMap, "P5_INCOMINGW", DefaultLanguage)).c_str());
		ImGui::Text((getLocalizedString(LocalMap, "P5_INCOMINGD", DefaultLanguage)).c_str());
		ImGui::Separator();

		ImGui::Text((getLocalizedString(LocalMap, "P5_INCOMINGTD", DefaultLanguage)).c_str());
		if (ImGui::SliderInt((getLocalizedString(LocalMap, "P5_INCOMINGFRAMES", DefaultLanguage)).c_str(), &FrameDelayofDeath, 0, 300))
		{
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P5_INCOMINGFRAMESD", DefaultLanguage)).c_str());

		if (ImGui::DragFloat((getLocalizedString(LocalMap, "P5_INCOMINGKOPOSX", DefaultLanguage)).c_str(), &DeathSiteX, 1.0f, -800.0f, 800.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{
			if (DeathSiteX < -800) { DeathSiteX = -800; }
			if (DeathSiteX > 800) { DeathSiteX = 800; }
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P5_INCOMINGKOPOSXD", DefaultLanguage)).c_str());

		if (ImGui::DragFloat((getLocalizedString(LocalMap, "P5_INCOMINGKOPOSY", DefaultLanguage)).c_str(), &DeathSiteY, 1.0f, 0.0f, 3000.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal))
		{
			if (DeathSiteY < 0) { DeathSiteY = 0; }
			if (DeathSiteY > 3000) { DeathSiteY = 3000; }
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P5_INCOMINGKOPOSYD", DefaultLanguage)).c_str());
		ImGui::Separator();

		if (ImGui::Button((getLocalizedString(LocalMap, "P5_INCOMINGSYNCP1SPOS", DefaultLanguage)).c_str()))
		{
			DeathSiteX = p1Pos;
			DeathSiteY = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P5_INCOMINGSYNCP2SPOS", DefaultLanguage)).c_str()))
		{
			DeathSiteX = p2Pos;
			DeathSiteY = 0;
		}
		ImGui::Separator();
		ImGui::Text((getLocalizedString(LocalMap, "P5_INCOMINGDEATHPOSPRESETS", DefaultLanguage)).c_str());
		if (ImGui::Button((getLocalizedString(LocalMap, "P5_INCOMINGPOSCENTER", DefaultLanguage)).c_str()))
		{
			DeathSiteX = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P5_INCOMINGPOSLCORNER", DefaultLanguage)).c_str()))
		{
			DeathSiteX = -800;
		}
		ImGui::SameLine();
		if (ImGui::Button((getLocalizedString(LocalMap, "P5_INCOMINGPOSRCORNER", DefaultLanguage)).c_str()))
		{
			DeathSiteX = 800;
		}
		ImGui::Separator();
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOACTIVECHARACTERP1", DefaultLanguage)).c_str(), &KOActiveCharacterAtStart))
		{
			if (CheckTheMode() == true)
			{
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P5_KOACTIVECHARACTERP1D", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOACTIVECHARACTERP2", DefaultLanguage)).c_str(), &KOActiveOpponentAtStart))
		{
			if (CheckTheMode() == true)
			{
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
			ImGui::SetTooltip((getLocalizedString(LocalMap, "P5_KOACTIVECHARACTERP2D", DefaultLanguage)).c_str());

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P5_KOTOGGLES", DefaultLanguage)).c_str());

		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP1C1", DefaultLanguage)).c_str(), &Player1Character1Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP1C2", DefaultLanguage)).c_str(), &Player1Character2Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP1C3", DefaultLanguage)).c_str(), &Player1Character3Dead))
		{
			KOToggles();
		}
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP2C1", DefaultLanguage)).c_str(), &Player2Character1Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP2C2", DefaultLanguage)).c_str(), &Player2Character2Dead))
		{
			KOToggles();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox((getLocalizedString(LocalMap, "P5_KOTOGGLEP2C3", DefaultLanguage)).c_str(), &Player2Character3Dead))
		{
			KOToggles();
		}
		ImGui::Text((getLocalizedString(LocalMap, "P1_MORESOON", DefaultLanguage)).c_str());

		ImGui::EndTabItem();

	}

}

static void TheRecordPlaybackTab()
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
		typedef void(*DeployTheHooks)();
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
			DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");

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

static void TheDebugStuffTab()
{
	//For Debug Stuff and keeping records.
	if (ImGui::BeginTabItem("Debug Stuff"))
	{
		GetMainPointers();
		GetPlayerData();
		GetHurtboxData();
		GetHitboxDataPart1();
		GetHitstunTimers();

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
		ImGui::SeparatorText("Module Base");
		uintptr_t moduleBaseInternal = (uintptr_t)GetModuleHandle(_T("umvc3.exe"));
		ImGui::Text("ModuleBase External = : %x", moduleBaseInternal);
		ImGui::Separator();
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
		if (ImGui::TreeNode("Hitstun Stuff"))
		{
#pragma region Hitstun Stuff

			ImGui::SeparatorText("Hitstun Timers");
			ImGui::Text("Player 1 Character 1");
			ImGui::Text("GroundHitstunTimer: %f", P1C1GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P1C1AirHitstunTimer);

			ImGui::Text("Player 1 Character 2");
			ImGui::Text("GroundHitstunTimer: %f", P1C2GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P1C2AirHitstunTimer);

			ImGui::Text("Player 1 Character 3");
			ImGui::Text("GroundHitstunTimer: %f", P1C3GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P1C3AirHitstunTimer);

			ImGui::Text("Player 2 Character 1");
			ImGui::Text("GroundHitstunTimer: %f", P2C1GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P2C1AirHitstunTimer);

			ImGui::Text("Player 2 Character 2");
			ImGui::Text("GroundHitstunTimer: %f", P2C2GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P2C2AirHitstunTimer);

			ImGui::Text("Player 2 Character 3");
			ImGui::Text("GroundHitstunTimer: %f", P2C3GroundHitstunTimer);
			ImGui::Text("AirHitstunTimer: %f", P2C3AirHitstunTimer);


#pragma endregion
			ImGui::TreePop();
		}

		ImGui::Separator();

		//if (ImGui::TreeNode("Collision Stuff"))
		//{

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
		//140D47F98

		ImGui::Separator();

		ImGui::Text("Shot List Stuff");

		ImGui::Text("sShotList: %x", sShotList);
		ImGui::Text("P1ShotList: %x", P1Shots);
		ImGui::Text("P2ShotList: %x", P2Shots);
		ImGui::Text("P1ShotList: %x", P1ProjectileCount);
		ImGui::Text("P1ShotList: %x", P2ProjectileCount);

		int ShotTotal = P1ProjectileCount + P2ProjectileCount;
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
		if (ImGui::BeginTable("Shot List", 12, flags, outer_size))
		{
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
			ImGui::TableSetupColumn("VTable");
			ImGui::TableSetupColumn("Pos X");
			ImGui::TableSetupColumn("Pos Y");
			ImGui::TableSetupColumn("Pos Z");
			ImGui::TableSetupColumn("Quat X");
			ImGui::TableSetupColumn("Quat Y");
			ImGui::TableSetupColumn("Quat Z");
			ImGui::TableSetupColumn("Quat W");
			ImGui::TableSetupColumn("Size X");
			ImGui::TableSetupColumn("Size Y");
			ImGui::TableSetupColumn("Size Z");

			ImGui::TableHeadersRow();
			ImGuiListClipper clipper;
			clipper.Begin(ShotTotal);
			while (clipper.Step())
			{
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < 12; column++)
					{

						if (!ImGui::TableSetColumnIndex(column) && column > 0)
							continue;
						if (column == 0)
							ImGui::Text("%s", ActiveShots[row].ProjName.c_str());
						else if (column == 1)
							ImGui::Text("%x", ActiveShots[row].ShotVTAddress);
						else if (column == 2)
							ImGui::Text("%f", ActiveShots[row].PossibleAbsPos.X);
						else if (column == 3)
							ImGui::Text("%f", ActiveShots[row].PossibleAbsPos.Y);
						else if (column == 4)
							ImGui::Text("%f", ActiveShots[row].PossibleAbsPos.Z);
						else if (column == 5)
							ImGui::Text("%f", ActiveShots[row].PossibleQuat.X);
						else if (column == 6)
							ImGui::Text("%f", ActiveShots[row].PossibleQuat.Y);
						else if (column == 7)
							ImGui::Text("%f", ActiveShots[row].PossibleQuat.Z);
						else if (column == 8)
							ImGui::Text("%f", ActiveShots[row].PossibleQuat.W);
						else if (column == 9)
							ImGui::Text("%f", ActiveShots[row].PossibleScale.X);
						else if (column == 10)
							ImGui::Text("%f", ActiveShots[row].PossibleScale.Y);
						else if (column == 11)
							ImGui::Text("%f", ActiveShots[row].PossibleScale.Z);
						else continue;
					}
				}
			}
			ImGui::EndTable();


		}
		//ImGui::Text("Shot Count: %i", ShotCount);
		/*
				if (ShotTotal > 0) {
				for (int n = 0; n < ShotTotal; n++)
				{
					ImGui::Text("Shot File: %x", ActiveShots[n].ShotVTAddress);
					ImGui::Text("Shot Name %s", ActiveShots[n].ProjName.c_str());
					ImGui::Text("Possible Position");
					ImGui::SameLine();
					ImGui::Text("X: %f", ActiveShots[n].PossibleAbsPos.X);
					ImGui::SameLine();
					ImGui::Text("Y: %f", ActiveShots[n].PossibleAbsPos.Y);
					ImGui::SameLine();
					ImGui::Text("Z: %f", ActiveShots[n].PossibleAbsPos.Z);
					ImGui::Text("Rotation (Quaternion)");
					ImGui::SameLine();
					ImGui::Text("X: %f", ActiveShots[n].PossibleQuat.X);
					ImGui::SameLine();
					ImGui::Text("Y: %f", ActiveShots[n].PossibleQuat.Y);
					ImGui::SameLine();
					ImGui::Text("Z: %f", ActiveShots[n].PossibleQuat.Z);
					ImGui::SameLine();
					ImGui::Text("W: %f", ActiveShots[n].PossibleQuat.W);
					ImGui::Text("Possible Scale");
					ImGui::SameLine();
					ImGui::Text("X: %f", ActiveShots[n].PossibleScale.X);
					ImGui::SameLine();
					ImGui::Text("Y: %f", ActiveShots[n].PossibleScale.Y);
					ImGui::SameLine();
					ImGui::Text("Z: %f", ActiveShots[n].PossibleScale.Z);
				}
			}

			*/

		ImGui::Separator();



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

		//	ImGui::TreePop();
		//}

		if (ImGui::TreeNode("Active installs"))
		{
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
			ImGui::TreePop();
		}
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

static void TheExtraDataTab()
{
	if (ImGui::BeginTabItem((getLocalizedString(LocalMap, "P4_DATA", DefaultLanguage)).c_str()))
	{
		GetMainPointers();
		GetPlayerData();
		GetHurtboxData();
		GetHitboxDataPart1();
		GetHitstunTimers();

		ImGui::SeparatorText((getLocalizedString(LocalMap, "P4_HITSTUNTIMERS", DefaultLanguage)).c_str());

		if (ImGui::TreeNode((getLocalizedString(LocalMap, "P4_HITSTUNTIMERSSH", DefaultLanguage)).c_str()))
		{
#pragma region Hitstun Stuff

			//Makes the text larger.
			ImGui::PushFont(NULL, 20.0f);



			static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
			ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));

			ImGui::PopStyleVar(2);

			if (ImGui::BeginTable("table1", 5, flags))
			{
				ImGui::TableSetupColumn((getLocalizedString(LocalMap, "P4_HITSTUNPLAYER", DefaultLanguage)).c_str(), ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn((getLocalizedString(LocalMap, "P4_HITSTUNGROUND", DefaultLanguage)).c_str(), ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn((getLocalizedString(LocalMap, "P4_HITSTUNAIR", DefaultLanguage)).c_str(), ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn((getLocalizedString(LocalMap, "P4_HITSTUNMAXGROUND", DefaultLanguage)).c_str(), ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn((getLocalizedString(LocalMap, "P4_HITSTUNMAXAIR", DefaultLanguage)).c_str(), ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP1C1", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P1C1GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P1C1AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P1C1GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P1C1AirHitstunTimerMAX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP1C2", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P1C2GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P1C2AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P1C2GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P1C2AirHitstunTimerMAX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP1C3", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P1C3GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P1C3AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P1C3GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P1C3AirHitstunTimerMAX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP2C1", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P2C1GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P2C1AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P2C1GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P2C1AirHitstunTimerMAX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP2C2", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P2C2GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P2C2AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P2C2GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P2C2AirHitstunTimerMAX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((getLocalizedString(LocalMap, "P4_HITSTUNP2C3", DefaultLanguage)).c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", P2C3GroundHitstunTimer);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", P2C3AirHitstunTimer);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%f", P2C3GroundHitstunTimerMAX);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%f", P2C3AirHitstunTimerMAX);
				ImGui::EndTable();
			}

			ImGui::PopFont();

#pragma endregion
			ImGui::TreePop();
		}


		ImGui::EndTabItem();
	}
}

#pragma endregion

static void frame(void) {
	// Gets the current window size.
	const int width = sapp_width();
	const int height = sapp_height();
	float ratio = width / (float)height;

	const float dpi_scale = sapp_dpi_scale();
	const float delta_time = (float)stm_sec(stm_diff(stm_now(), state.last_time));
	state.last_time = stm_now();

	// New ImGui frame
	simgui_frame_desc_t frame_desc = {};
	frame_desc.width = width;
	frame_desc.height = height;
	frame_desc.delta_time = delta_time;
	frame_desc.dpi_scale = dpi_scale;
	simgui_new_frame(&frame_desc);


	// === ImGui UI Starts Here ===

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)sapp_width(), (float)sapp_height()));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::Begin("ImGui + Sokol", 0, flags);
	//ImGui::PushFont(font1, 17.0f);
	//ImGui::Text("Window size: %d x %d", width, height);
	//ImGui::Text("Delta time: %.3f ms", delta_time * 1000.0f);

	//The Menu Bar.
	TheMenuBar();

	//Tries to hook.
	CastTheHook();

	if (Hooked)
	{

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(70, 255, 70, 255));

		//ImGui::Text("If you can read this then the tool recognizes Marvel 3 is open.");
		//ImGui::Text((localMan.get("ACTIVATED_TRUE")).c_str());
		ImGui::Text((getLocalizedString(LocalMap, "ACTIVATED_TRUE", DefaultLanguage)).c_str());

		//ImGui::Text("The version open is valid too.");
		//ImGui::Text((localMan.get("ACTIVATED_TRUE2")).c_str());
		ImGui::Text((getLocalizedString(LocalMap, "ACTIVATED_TRUE2", DefaultLanguage)).c_str());
		
		ImGui::PopStyleColor();
		WackyStuff();

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
					ReadProcessMemory(hProcess, (LPVOID*)(sAction + 0x118), &val, sizeof(val), 0);

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

					//Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

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
					TheExtraDataTab();
					TheIncomingStuffTab();
#ifdef _DEBUG

					TheRecordPlaybackTab();

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
				//ImGui::Text("Choose the characters and get into a match.");
				ImGui::Text((getLocalizedString(LocalMap, "ACTIVATED_NOTINMATCH", DefaultLanguage)).c_str());
				ImGui::PopStyleColor();

			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 70, 255));
			//ImGui::Text("Get to Training Mode so this tool can do its thing.");
			ImGui::Text((getLocalizedString(LocalMap, "ACTIVATED_INMAINMENU", DefaultLanguage)).c_str());
			ImGui::PopStyleColor();
		}


	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 70, 70, 255));

		//ImGui::Text("Nothing Doing until you open the game.");		
		ImGui::Text((getLocalizedString(LocalMap, "NOT_ACTIVATED1", DefaultLanguage)).c_str());
		//ImGui::Text((localMan.get("NOT_ACTIVATED1")).c_str());

		//ImGui::Text("If the game is open and nothing is happening,\nthen it could be possible\nyou are using an invalid version.");
		ImGui::Text((getLocalizedString(LocalMap, "NOT_ACTIVATED2", DefaultLanguage)).c_str());
		//ImGui::Text((localMan.get("NOT_ACTIVATED2")).c_str());

		ImGui::PopStyleColor();

	}


	ImGui::End();
	ImGui::Render();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(100, 100, 100, 217));

	ImGui::PopStyleColor();

	sg_pass pass = { .swapchain = sglue_swapchain() };

	sg_begin_pass(&pass);

	simgui_render();
	sg_end_pass();
	sg_commit();
}

static void input(const sapp_event* event) {
	simgui_handle_event(event);
}

static void cleanup(void) {
	simgui_shutdown();
	sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
	return {
		.init_cb = init,
		.frame_cb = frame,
		.cleanup_cb = cleanup,
		.event_cb = input,
		.width = 480,
		.height = 720,
		.window_title = "UMVC3 Training Tools V0.8 Beta",
	};
}
