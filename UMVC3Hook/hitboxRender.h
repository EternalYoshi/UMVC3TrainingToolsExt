#pragma once
#include <d3d9.h>
class HitboxRender
{
public:
	void circleFilled(LPDIRECT3DDEVICE9 pDevice, float x, float y, float rad, int resolution, DWORD color);
	void renderHitboxes(LPDIRECT3DDEVICE9 pDevice);
	void renderStart(IDirect3DDevice9* device);
	void renderEnd(IDirect3DDevice9* device);
};


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};


