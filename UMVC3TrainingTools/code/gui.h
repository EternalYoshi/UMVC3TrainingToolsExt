#pragma once


#include <d3d9.h>

#pragma region Take1
namespace gui
{
	//Window Sizes.
	constexpr int WIDTH = 500;
	constexpr int HEIGHT = 800;

	inline bool exit = true;

	//winapir Window Variables.
	inline HWND window = nullptr;
	inline WNDCLASSEXA windowClass = {};

	//points for Window Movement.
	inline POINTS position = {};

	//DirectX State Variables.
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = {};

	//Window Creation & Deleteion Handler.
	void CreateHWindow(const char* windowName, const char* className) noexcept;
	void DestroyHWindow() noexcept;

	//Device Creation & Deletion Handler.
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestoryDevice() noexcept;

	//Imgui Creation & Deletion.
	void CreateImGui() noexcept;
	void DestroyImgui() noexcept;

	void BeginRender() noexcept;
	void Endrender() noexcept;
	void Render() noexcept;

	//ImGui Menu Stuff.
	static void TheMenuBar();
	static bool ShowAboutWindow = false;
	static void TheAboutWindow(bool*);
	static void TheExtraOptionsTab();
	static void TheStatusOptionsTab();
	static void TheCharacterOptionsTab();
	static void TheRecordPlaybackTab();
	static bool Hooked = false;
	static int GameModeRef = 0;

}

#pragma endregion


