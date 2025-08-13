#include "gui.h"
#include "umvc3utils.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
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

//HINSTANCE ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));



//ImGui menu stuff begins here.
//static void TheMenuBar()
//{
//	if (ShowAboutWindow) TheAboutWindow(&ShowAboutWindow);
//
//	if (ImGui::BeginMenuBar())
//	{
//		if (ImGui::BeginMenu("File"))
//		{
//#ifdef _DEBUG
//			if (ImGui::MenuItem(("Find ThreeHook"), "CTRL+O"))
//			{
//				std::string ChosenFile = "";
//				OPENFILENAME ofn = { sizeof(OPENFILENAME) };
//				char szFile[_MAX_PATH] = "ThreeHook";
//				const char szExt[] = ".asi\0"; // extra '\0' for lpstrFilter
//				ofn.hwndOwner = GetConsoleWindow();
//				ofn.lpstrFile = szFile; // <--------------------- initial file name
//				ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
//				ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
//				ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
//				if (GetOpenFileName(&ofn))
//				{
//					ChosenFile = ofn.lpstrFile;
//					ThreeHookPath = ChosenFile;
//					ccheck = LoadLibrary(const_cast<char*>(ThreeHookPath.c_str()));
//					if (ccheck)
//					{
//						typedef void(*DeployTheHooks)();
//						DeployTheHooks dhook = (DeployTheHooks)GetProcAddress((HMODULE)ccheck, "DeployTheHooks");
//						dhook();
//					}
//
//
//
//				}
//			}
//#endif
//			if (ImGui::MenuItem(("Reset Settings To Default")))
//			{
//				//ResetSettings();
//			}
//
//			ImGui::MenuItem(("Exit"), NULL, &exit);
//
//			ImGui::EndMenu();
//		}
//
//		if (ImGui::BeginMenu("Misc"))
//		{
//
//
//			ImGui::MenuItem("About", NULL, &ShowAboutWindow);
//
//
//			ImGui::EndMenu();
//		}
//
//		ImGui::EndMenuBar();
//	}
//
//}

//void TheAboutWindow(bool* p_open)
//{
//	ImGui::SetNextWindowSize({ 400, 400 }, ImGuiCond_FirstUseEver);
//	if (!ImGui::Begin("UMVC3 Training Tools Mod V0.8", p_open))
//	{
//		ImGui::End();
//		return;
//	}
//	ImGui::Text("2023 - 2025 By Eternal Yoshi");
//	ImGui::Separator();
//	ImGui::Text("Thanks To SanHKHaan, Sheep, anotak, Techs, & Gneiss for finding\nthe pointers and memeory offsets that made this \npossible,and Ermmaccer for the original UMVC3Hook.\n");
//	ImGui::Separator();
//	ImGui::Text("In case it isn't obvious, this tool is NOT designed to\nenable cheating in online multiplayer and\nis intended to only function in Training Mode.");
//
//	ImGui::End();
//
//}