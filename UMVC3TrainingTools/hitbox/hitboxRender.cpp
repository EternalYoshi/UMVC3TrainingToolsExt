#include "hitboxRender.h"
#include <vector>
//#include "./code/umvc3menu.h"
#include <functional>
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
#define D3DX_PI 3.1415926535897932384626

static float Lerp(float start_value, float end_value, float pct)
{
    return (start_value + (end_value - start_value) * pct);
}
std::function<void(IDirect3DDevice9*)> render_end;
// Set up the device for rendering.
void HitboxRender::renderEnd(IDirect3DDevice9* device) {
	render_end(device);
}
void HitboxRender::renderStart(IDirect3DDevice9* device) //stolen from Altimor unisthitboxes source
{
	// Get the resolution scale.
	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);

	// Back up device state.
	DWORD alpha_blend_enable, dest_blend, src_blend, dest_blend_alpha, src_blend_alpha, fvf;
	IDirect3DPixelShader9* pixel_shader;
	IDirect3DBaseTexture9* texture;
	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &alpha_blend_enable);
	device->GetRenderState(D3DRS_DESTBLEND, &dest_blend);
	device->GetRenderState(D3DRS_SRCBLEND, &src_blend);
	device->GetRenderState(D3DRS_DESTBLENDALPHA, &dest_blend_alpha);
	device->GetRenderState(D3DRS_SRCBLENDALPHA, &src_blend_alpha);
	device->GetPixelShader(&pixel_shader);
	device->GetFVF(&fvf);
	device->GetTexture(0, &texture);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
	device->SetPixelShader(nullptr);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetTexture(0, nullptr);

	render_end = [=](IDirect3DDevice9* device)
	{
		// Restore device state.
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, alpha_blend_enable);
		device->SetRenderState(D3DRS_DESTBLEND, dest_blend);
		device->SetRenderState(D3DRS_SRCBLEND, src_blend);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, dest_blend_alpha);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, src_blend_alpha);
		device->SetPixelShader(pixel_shader);
		device->SetFVF(fvf);
		device->SetTexture(0, texture);

		if (texture != nullptr)
			texture->Release();
	};
}
void HitboxRender::circleFilled(LPDIRECT3DDEVICE9 pDevice, float x, float y, float rad, int resolution, DWORD color)
{
	std::vector<CUSTOMVERTEX> circle(resolution + 2);
	float pi = D3DX_PI;        // Full circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(0) * (circle[i].x - x) - sin(0) * (circle[i].y - y);
		circle[i].y = y + sin(0) * (circle[i].x - x) + cos(0) * (circle[i].y - y);
	}

	pDevice->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB->Unlock();

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);

	if (g_pVB != NULL) g_pVB->Release();
}

uintptr_t activefighteraddresses[2]{ 0,0 };
void HitboxRender::renderHitboxes(LPDIRECT3DDEVICE9 pDevice)
{
	auto block2 = *(uintptr_t*)_addr(0x140d47e68);
	auto team1 = block2 + 0x350;
	auto team2 = block2 + 0x610;
	activefighteraddresses[0] = *((uintptr_t*)(team1 + 0x48));
	activefighteraddresses[1] = *((uintptr_t*)(team2 + 0x48));
	for (size_t x = 0; x < 2; x++)
	{
		int fighterIndex = UMVC3Menu::getFighterIndex(activefighteraddresses[x]);;
		int hboxcnt = UMVC3Menu::getHBoxCount(fighterIndex);
		for (size_t i = 0; i < hboxcnt; i++)
		{
			auto hboxpos = UMVC3Menu::getHBoxPos(i, fighterIndex);
			float xMul = Lerp(0.9, 1.2, 1 - (hboxpos.pos.X / 1600));
			//CircleFilled(pDevice, (1600 * 1.1 - hboxpos.pos.X), (900 * 1.1 - hboxpos.pos.Y), 35, 16, 0x00ff00a0);
			if (hboxpos.enabled)
				circleFilled(pDevice, (1600 * xMul - hboxpos.pos.X), (900 * UMVC3Menu::getYMultiplier() - hboxpos.pos.Y), hboxpos.size * 2, 16, hboxpos.color);
		}
	}
	for (size_t i = 0; i < UMVC3Menu::getHitboxCount(); i++)
	{
		auto hboxpos = UMVC3Menu::getHitBoxPos(i);
		float xMul = Lerp(0.95, 1.17, 1 - (hboxpos.pos.X / 1600));
		//CircleFilled(pDevice, (1600 * 1.1 - hboxpos.pos.X), (900 * 1.1 - hboxpos.pos.Y), 35, 16, 0x00ff00a0);
		if (hboxpos.enabled)
			circleFilled(pDevice, (1600 * xMul - hboxpos.pos.X), (900 * UMVC3Menu::getYMultiplier() - hboxpos.pos.Y), hboxpos.size * 2, 16, hboxpos.color);
	}

}

