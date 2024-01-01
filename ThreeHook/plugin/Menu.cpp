//By Eternal Yoshi. Thanks to ermaccer for the new hook & HKHaan and Sheep for collecting the pointers and data needed to make this possible.
#include "Menu.h"
#include "Settings.h"
#include "..\sigscan.h"
#include "../gui/notifications.h"
#include "../gui/imgui/imgui.h"
#include "../umvc3/Camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#define undefined4 int
static int64 timer = GetTickCount64();

// restartTimer = 0;

UMVC3Menu* TheMenu = new UMVC3Menu();

int runaheadcount = 0;
bool pAddrSet = false;

uintptr_t mysterytable;
uintptr_t block2;
uintptr_t ptable;
int P1Character1ID;
int P1Character2ID;
int P1Character3ID;
int P2Character1ID;
int P2Character2ID;
int P2Character3ID;
uintptr_t P1Character1Data;
uintptr_t P1Character2Data;
uintptr_t P1Character3Data;
uintptr_t P2Character1Data;
uintptr_t P2Character2Data;
uintptr_t P2Character3Data;
int MODOKLOU = 1;
int FrankLevel = 1;
float PrestigePoints = 0.0;
int WrightEvidenceA = -1;
int WrightEvidenceB = -1;
int WrightEvidenceC = -1;
int SaveStateSlot = 0;
int DormRed = 0;
int DormBlue = 0;
bool DormSpellSet = false;
bool EndlessInstalls = false;
int DeadpoolTeleportCount = 0;
bool FreezeDeadpoolTPCounter = false;
bool P1Character1Dead = false;
bool P1Character2Dead = false;
bool P1Character3Dead = false;
bool P2Character1Dead = false;
bool P2Character2Dead = false;
bool P2Character3Dead = false;
bool EndlessXFactor = false;
bool SpecialFeature = false;
bool HitboxDisplay = false;
bool SetIndividualHP = false;
bool AlsoSetRedHealth = false;
float P1Char1Health = 1.0;
float P1Char1RedHealth = 1.0;
float P1Char2Health = 1.0;
float P1Char2RedHealth = 1.0;
float P1Char3Health = 1.0;
float P1Char3RedHealth = 1.0;
float P2Char1Health = 1.0;
float P2Char1RedHealth = 1.0;
float P2Char2Health = 1.0;
float P2Char2RedHealth = 1.0;
float P2Char3Health = 1.0;
float P2Char3RedHealth = 1.0;
bool P1Char1Slow = false;
bool P1Char2Slow = false;
bool P1Char3Slow = false;
bool P2Char1Slow = false;
bool P2Char2Slow = false;
bool P2Char3Slow = false;
bool DarkPhoenix = false;
bool Turnabout = false;

#define longlong  long long
#define ulonglong  unsigned long long
#define undefined8  long long*
#define undefined7  long long
#define undefined2  int

typedef int (*code)(longlong* param_1);
typedef int (*codenoarg)();
typedef void (*method)(void);
#define undefined int
#define undefined4 int
#define CONCAT71(a,b) (a<<32 | b)
#define backupsize 10000//100000
#define prebackup 0 //100000

constexpr int backupSize = 1000;
constexpr int teamBackupSize = 0x960;

//int GameMode;
std::vector<uintptr_t> hitboxes;
bool startedHitboxViewer = false;

//uintptr_t inputRef;


struct MarvelVector {
	float x;
	float y;
	float xVelocity;
};
enum EGroundedState :uint8_t {
	NoGround = 0,
	Grounded = 0xF4,
	InAir = 0xF2,
};

enum ETagState :uint8_t {
	NoTag = 0,
	Active = 0x0D,
	Inactive = 0x09,
	Active2 = 0x68,
	TagginOut = 0xB4,
	TagginIn = 0x6c
};

struct Fighter { // TODO map this out
	char offset[0x14];
	ETagState tagState;
	char offset2[0x3B];
	MarvelVector vector;
	char offset3[0x10];
	EGroundedState  groundedState;
	char offset4[0xc7];
	uint16_t specialState;//specialstate?
	char offset5[0x528];
	uint32_t anim; // anim??
};

struct ScriptableFighter {
	Fighter* fighter = 0;
	intptr_t fighterController = 0; // TODO find better name for this
	char* name = nullptr;
	uintptr_t tickPtr = 0x14004bd30;
	void Tick() {
		((void(__fastcall*)(longlong*))_addr(tickPtr))((longlong*)fighter);
	}
	const char* GetGroundedState() {
		switch (fighter->groundedState)
		{
		case Grounded:
			return "Grounded";
		case InAir:
			return "InAir";
		default:
			return "Unkown";
			break;
		}
	}
	const char* GetTagState() {
		switch (fighter->tagState)
		{
		case Active:
		case Active2:
			return "Active";
		case Inactive:
			return "Inactive";
		case TagginOut:
			return "TagginOut";
		case TagginIn:
			return "TagginIn";
		default:
			return "Unkown";
			break;
		}
	}
};

ScriptableFighter scriptableFighters[6] = { 0 };

//Checks Character ID. Gotta rewrite this function later.
bool CheckCharacterID(int CharacterID, int TargetID, uintptr_t mysterytable)
{
	if (CharacterID == TargetID)
	{
		return true;
	}
	else return false;


}


/*
//Saves Raw Replay Data
void SaveReplayP1()
{

	uintptr_t block3 = *(uintptr_t*)_addr(0x140D510A0);
	uintptr_t P1RecordingData = *(uintptr_t*)_addr(block3 + 0x90);
	//int P1RecordingFrameTotal = *(uintptr_t*)_addr(P1RecordingData + 0x40);
	uintptr_t P1RecordingRawInput = *(uintptr_t*)_addr(P1RecordingData + 0x44);

	Recording P1Recording = {
	*(uintptr_t*)_addr(P1RecordingData + 0x40),

	};
	//*P1Recording.FrameCount = *(uintptr_t*)_addr(P1RecordingData + 0x40);



}
*/
void TurnOnHitboxDisplay()
{

	//First we get the hurtbox data. To be continued....

}

/*
bool CheckTheMode()
{
	//Checks for Training Mode. If not in Training Mode, it returns false.
	uintptr_t strangetable = *(uintptr_t*)_addr(0x140d50e58);
	GameMode = *((int*)(strangetable + 0x34C));
	//printf("%d\n", GameMode);
	if (GameMode == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}
*/

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

UMVC3Menu::UMVC3Menu()
{
}

void UMVC3Menu::OnActivate()
{
	m_bIsActive ^= 1;
}

void UMVC3Menu::OnToggleFreeCamera()
{
	if (m_bIsActive)
		return;

	m_bFreeCam ^= 1;
}

void UMVC3Menu::OnToggleHUD()
{
	if (m_bIsActive)
		return;

	m_bDisableHUD ^= 1;
}

void GetTheImportantAddresses()
{

	auto sigger = sigscan::get();

	if (pAddrSet == false)
	{
		pAddrSet = true;
		auto mysterytable = *(uintptr_t*)_addr(0x140d533e0);
		auto block2 = *(uintptr_t*)_addr(0x140d47e68);
		for (size_t i = 0; i < 6; i++)
		{
			scriptableFighters[i].fighter = *(Fighter**)(mysterytable + 0xaa0 + (0x438 * i));
		}
	}
	else {
		for (size_t i = 0; i < 6; i++)
		{
			if (scriptableFighters[i].name == nullptr) {
				ImGui::Text("p%d:%llx", i, scriptableFighters[i].fighter);
			}
			else {
				ImGui::Text("p%d:%llx %llx %s", i, scriptableFighters[i].fighter, scriptableFighters[i].tickPtr, scriptableFighters[i].name);
			}
			if (scriptableFighters[i].fighter->tagState == ETagState::Active || scriptableFighters[i].fighter->tagState == ETagState::Active2)
			{
				ImGui::Text("  X:%d", (int)scriptableFighters[i].fighter->vector.x);
				ImGui::SameLine();
				ImGui::Text("- Y:%d", (int)scriptableFighters[i].fighter->vector.y);
				ImGui::SameLine();
				ImGui::Text("- Vel:%d", (int)scriptableFighters[i].fighter->vector.xVelocity);

				ImGui::Text("  Anim:%x", scriptableFighters[i].fighter->anim);
				ImGui::Text("  Special:%x", scriptableFighters[i].fighter->specialState);

				ImGui::Text("  Grounded:%s", scriptableFighters[i].GetGroundedState());
				ImGui::Text("  TagState:%s", scriptableFighters[i].GetTagState());
			}
		}


		auto block2 = *(uintptr_t*)_addr(0x140d47e68);
		auto team1 = block2 + 0x350;
		auto team2 = block2 + 0x610;
		ImGui::Text("Team1:%llx ActiveFighter %x", team1, team1 + 0x48);
		ImGui::Text("Team2:%llx ActiveFighter %x", team2, team2 + 0x48);
	}

}

void UMVC3Menu::Draw()
{
	if (!m_bIsActive)
		return;
	FILE* pRec;
	Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
	//InjectHook(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL);
	auto block2 = *(uintptr_t*)_addr(0x140d47e68);
	auto val = *(unsigned char*)(block2 + 0x118);
	/*
	if (val != 0)
	{
		restarted = true;
		restartTimer = 0;
	}
	else if (restarted)
	{
		GetTheImportantAddresses();
		hitboxes.clear();
		restartTimer += 1;
		if (restartTimer > 5)//0.166667 sec wait
		{
			//RestartWithChanges();

			{
				auto ptr = ((uintptr_t)scriptableFighters[0].fighter) + 0x50;
				*((float*)ptr) = p1Pos;
			}
			{
				auto ptr = ((uintptr_t)scriptableFighters[3].fighter) + 0x50;
				*((float*)ptr) = p2Pos;
			}
			restarted = false;
			//SetIndividualCharacterHealth();

		}


	}
	*/
	ImGui::GetIO().MouseDrawCursor = true;

}

void UMVC3Menu::Process()
{

	Memory::VP::Patch<char>(_addr(0x140532D80), m_bFreeCam ? 0xC3 : 0x48);

	Memory::VP::Patch<char>(_addr(0x140326A70), (m_bDisableHUD || m_bDisableHUDTotal) ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x140324240), (m_bDisableHUD || m_bDisableHUDTotal) ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x1404537B0), m_bDisableHUDTotal ? 0xC3 : 0x48);

	Memory::VP::Patch<char>(_addr(0x14077EA40), m_bDisableEffects ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x14077D330), m_bDisableEffects ? 0xC3 : 0x48);

	UpdateControls();
	UpdateFreecam();
}

void UMVC3Menu::UpdateControls()
{

}

void UMVC3Menu::UpdateFreecam()
{

	UMVC3MainCamera* cam = GetMainCamera();

	if (!cam)
		return;

	Vector fwd = cam->mat.forward;
	Vector up = cam->mat.up;
	Vector right = cam->mat.right;

	static XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	float delta = GUIImplementationDX9::GetDeltaTime();

	if (m_bFreeCam)
	{
		// forward

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyXPlus))
			TheMenu->camMat.pos.X += TheMenu->m_fFreeCameraSpeed * delta * 1;


		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyXMinus))
			TheMenu->camMat.pos.X += TheMenu->m_fFreeCameraSpeed * delta * -1;

		// right

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYPlus))
			TheMenu->camMat.pos.Y += TheMenu->m_fFreeCameraSpeed * delta * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYMinus))
			TheMenu->camMat.pos.Y += TheMenu->m_fFreeCameraSpeed * delta * -1;

		// up

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyZPlus))
			TheMenu->camMat.pos.Z += TheMenu->m_fFreeCameraSpeed * delta * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyZMinus))
			TheMenu->camMat.pos.Z += TheMenu->m_fFreeCameraSpeed * delta * -1;


		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchPlus))
			camRot.X += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchMinus))
			camRot.X -= TheMenu->m_fFreeCameraRotationSpeed * delta;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawPlus))
			camRot.Y += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawMinus))
			camRot.Y -= TheMenu->m_fFreeCameraRotationSpeed * delta;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollPlus))
			camRot.Z += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollMinus))
			camRot.Z -= TheMenu->m_fFreeCameraRotationSpeed * delta;
	}

	if (m_bFreeCam || m_bCustomCameraFOV)
	{
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
			camFov -= 2.5f * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
			camFov += 2.5f * delta;
	}


	if (m_bFreeCam)
	{
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(camRot.X, camRot.Y, camRot.Z);

		XMVECTOR camLookAt = XMVector3TransformCoord(defaultForward, rotMat);
		XMVECTOR camUp = XMVector3TransformCoord(defaultUp, rotMat);
		XMVECTOR camRight = XMVector3TransformCoord(defaultRight, rotMat);
		XMVector3Normalize(camLookAt);
		XMVector3Normalize(camUp);
		XMVector3Normalize(camRight);
		XMFLOAT3 cL, cU, cR;
		XMStoreFloat3(&cL, camLookAt);
		XMStoreFloat3(&cU, camUp);
		XMStoreFloat3(&cR, camRight);

		camMat.forward = { cL.x, cL.y, cL.z };
		camMat.up = { cU.x, cU.y, cU.z };
		camMat.right = { cR.x, cR.y, cR.z };
	}
	if (m_bFreeCam)
	{
		cam->mat.pos = cam->mat2.pos = camMat.pos;
		cam->mat.right = cam->mat2.right = camMat.right;
		cam->mat.forward = cam->mat2.forward = camMat.forward;
		cam->mat.up = cam->mat2.up = camMat.up;

	}
	else
	{
		camMat.pos = cam->mat.pos;
	}

	if (TheCamera)
	{
		if (m_bCustomCameraFOV)
			TheCamera->mFOV = camFov;
		else
			camFov = TheCamera->mFOV;
	}
}

void UMVC3Menu::DrawCameraTab()
{
	UMVC3MainCamera* cam = GetMainCamera();

	if (!cam)
		return;
#ifdef _DEBUG
	ImGui::Text("%p", cam);


	ImGui::InputFloat4("Pr", &cam->mat.M[0][0]);
	ImGui::InputFloat4("Pu", &cam->mat.M[1][0]);
	ImGui::InputFloat4("Pf", &cam->mat.M[2][0]);
	ImGui::InputFloat4("Pp", &cam->mat.M[3][0]);
	ImGui::Separator();
	ImGui::Separator();
	ImGui::InputFloat4("Sr", &cam->mat2.M[0][0]);
	ImGui::InputFloat4("Su", &cam->mat2.M[1][0]);
	ImGui::InputFloat4("Sf", &cam->mat2.M[2][0]);
	ImGui::InputFloat4("Sp", &cam->mat2.M[3][0]);


	ImGui::Separator();
#endif

	ImGui::InputFloat3("X | Y | Z", &camMat.pos.X);
	ImGui::InputFloat3("Pitch | Yaw | Roll", &camRot.X);


	ImGui::Separator();
	ImGui::Checkbox("Set FOV", &m_bCustomCameraFOV);
	ImGui::SameLine();
	ShowHelpMarker("FOV won't update while free camera is active!");
	ImGui::InputFloat("FOV", &camFov);


	ImGui::Separator();
	ImGui::Checkbox("Enable Free Camera", &m_bFreeCam);
	ImGui::SameLine(); ShowHelpMarker("Allows to move camera with keyboard. Keys can be changed in the Settings menu or in the .ini file. Hotkey can be assigned in Settings menu.");

	if (m_bFreeCam)
	{
		ImGui::InputFloat("Free Camera Speed", &m_fFreeCameraSpeed);
		ImGui::InputFloat("Free Camera Rotation Speed", &m_fFreeCameraRotationSpeed);

	}

}

void UMVC3Menu::DrawMiscTab()
{
	ImGui::Checkbox("Disable Game HUD", &m_bDisableHUD);
	ImGui::SameLine();
	ShowHelpMarker(eKeyboardMan::KeyToString(SettingsMgr->iToggleHUDKey));
	ImGui::Checkbox("Disable All HUD", &m_bDisableHUDTotal);
	ImGui::Checkbox("Disable Effects", &m_bDisableEffects);
}

void UMVC3Menu::DrawSettings()
{
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 35 * ImGui::GetFontSize(), 35 * ImGui::GetFontSize() }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SM_Settings]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"INI",
		"Keys",
	};

	enum eSettings {
		MENU,
		INI,
		KEYS,
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to umvc3hook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::InputFloat("##", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
		break;
	case INI:
		ImGui::TextWrapped("These settings control UMVC3Hook.ini options. Any changes require game restart to take effect.");
		ImGui::Checkbox("Debug Console", &SettingsMgr->bEnableConsoleWindow);
		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");

		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
		{
			SettingsMgr->ResetKeys();
			Notifications->SetNotificationTime(5500);
			Notifications->PushNotification("Keys reset! Remember to save.");

		}
		ImGui::Separator();
		ImGui::LabelText("##", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iHookMenuOpenKey, "Open/Close Menu", "menu");
		ImGui::Separator();
		ImGui::LabelText("##", "Camera");
		ImGui::Separator();

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyYawPlus, "Yaw+", "ya_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYawMinus, "Yaw-", "ya_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchPlus, "Pitch+", "pi_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchMinus, "Pitch-", "pi_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollPlus, "Roll+", "r_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollMinus, "Roll-", "r_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyXPlus, "X+", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyXMinus, "X-", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYPlus, "Y+", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYMinus, "Y-", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZPlus, "Z+", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZMinus, "Z-", "z_minus");


		ImGui::Separator();
		ImGui::LabelText("##", "Misc");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iToggleFreeCameraKey, "Toggle Free Camera", "fcam");
		KeyBind(&SettingsMgr->iToggleHUDKey, "Toggle HUD", "thud");
		ImGui::Separator();

		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(5500);
		Notifications->PushNotification("Settings saved to UMVC3Hook.ini and umvc3hook_user.ini!");
		GUIImplementationDX9::RequestFontReload();
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();

	ImGui::End();
}

void UMVC3Menu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void UMVC3Menu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("##", bindName);
	DrawKeyBind(name, var);
}



//TODO put this in a better spot.

#include <d3d9types.h>
bool UMVC3Menu::startedHitbox() {
	return startedHitboxViewer;
}
int UMVC3Menu::getFighterIndex(uintptr_t ptr) {
	for (size_t i = 0; i < 6; i++)
	{
		if ((uintptr_t)scriptableFighters[i].fighter == ptr) return i;
	}
	return 0;
}
int UMVC3Menu::getHBoxCount(int fighterIndex) {
	if (!pAddrSet || scriptableFighters[fighterIndex].fighter == NULL)
		return 0;
	auto t = *((uintptr_t*)(((uint8_t*)scriptableFighters[fighterIndex].fighter) + 0x4E10));
	auto tt = (uintptr_t*)(*(uintptr_t*)(((uint8_t*)t) + 0x30));
	for (size_t i = 0; i < 100; i++)
	{
		if (*tt == 0) {
			return i;
		}
		tt++;
	}
	return 26;
}
HBoxInfo UMVC3Menu::getHBoxPos(int index, int fighterIndex)
{
	if (!pAddrSet || scriptableFighters[fighterIndex].fighter == NULL)
		return HBoxInfo(0, 0, 0.0f, D3DCOLOR_RGBA(0, 250, 0, 100), 60, 0);
	auto t = *((uintptr_t*)(((uint8_t*)scriptableFighters[fighterIndex].fighter) + 0x4E10));
	auto tt = (uint8_t*)*(uintptr_t*)(((uint8_t*)t) + 0x30);
	tt += index * 8;
	tt = (uint8_t*)(*(uintptr_t*)tt);
	auto valx = tt + 0x30;
	auto posx = *((float*)valx);
	auto valy = tt + 0x34;
	auto posy = *((float*)valy);
	auto r = screenPosition(glm::vec3(posx, posy, 70.0f));
	return HBoxInfo(r.x, r.y, 0.0f, D3DCOLOR_RGBA(0, 250, 0, 70, 60, 0), 30, 1);
}

int UMVC3Menu::getHitboxCount() {
	return hitboxes.size();
	return NULL;
}
HBoxInfo UMVC3Menu::getHitBoxPos(int index)
{
	auto valx = ((uint8_t*)hitboxes[index]) + 0x20;
	auto posx = *((float*)valx);
	auto valy = ((uint8_t*)hitboxes[index]) + 0x24;
	auto posy = *((float*)valy);
	auto valen = ((uint8_t*)hitboxes[index]) + 0x30 + 0x20;
	auto en = *((int*)valen);
	auto valsz = ((uint8_t*)hitboxes[index]) + 0x20 + 0x20;
	float size = *((float*)valsz);
	if(index==0)
		printf("%llx - %f\n",valx,posy);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

	auto r = screenPosition(glm::vec3(posx, posy, 70.0f));
	if (posx != 0) {
		printf("pttrtrtrt:%f - %llx %llx\n", posx, valx, valen);
	}
	return HBoxInfo(r.x, r.y, 0.0f, D3DCOLOR_RGBA(250, 0, 0, 100), size, en);
}



int64 cameraptr = 0;
float posXMultiplier = 1.15;
float posYMultiplier = 1.37;
#include "../glm/vec4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
float UMVC3Menu::getXMultiplier() { return posXMultiplier; }
float* UMVC3Menu::getXMultiplierPtr() { return &posXMultiplier; }
void UMVC3Menu::setXMultiplier(float val) { posXMultiplier = val; }
float UMVC3Menu::getYMultiplier() { return posYMultiplier; }
float* UMVC3Menu::getYMultiplierPtr() { return &posYMultiplier; }
void UMVC3Menu::setYMultiplier(float val) { posYMultiplier = val; }
void UMVC3Menu::setCameraPtr(uintptr_t camptr) {
	cameraptr = camptr;
}
glm::highp_mat4 getProjection() {

	auto res = glm::perspective<float>(*(float*)(cameraptr + 0x4C), 1600.0f / 900.0f, 0.01f, 100.0f);
	return res;
}
glm::vec4 getView() {
	return glm::vec4(0, 0, 1600, 900 * UMVC3Menu::getYMultiplier());
}
glm::vec2 UMVC3Menu::screenPosition(glm::vec3& _coord) {
	float posx = *(float*)(cameraptr + 0x50);
	float posy = *(float*)(cameraptr + 0x54);
	float posz = *(float*)(cameraptr + 0x58);
	float posx2 = *(float*)(cameraptr + 0x70);
	float posy2 = *(float*)(cameraptr + 0x74);
	float posz2 = *(float*)(cameraptr + 0x78);

	_coord.z = posz;
	int multi = 0.5f;
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(posx, posy, posz2), glm::vec3(posx2, posy2, posz), glm::vec3(0, 1, 0));
	return glm::project(_coord, viewMatrix, getProjection(), getView());
}
