#pragma once
#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <Windows.h>
#include <d3d11.h>

#define SAFE_RELEASE(o) do { if (o) { (o)->Release(); (o) = NULL; } } while(0)

bool Direct3D_Initialize(HWND hWnd);
void Direct3D_Finalize();
void Direct3D_DrawBegin();
void Direct3D_Present();

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

ID3D11Device* Direct3D_GetDevice();
ID3D11DeviceContext* Direct3D_GetDeviceContext();
void releaseBackBuffer();
bool createBackBuffer();

#endif
