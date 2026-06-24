#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <d3d11.h>
#include <DirectXMath.h>

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize();

int Texture_Load(const wchar_t* pFileName, bool bMipMap = true);
static constexpr int TEXTURE_INVALID_ID = -1;

void Texture_Release(int texture_id);
void Texture_Release(const int* pTextureIDs, int count);
void Texture_AllRelease();

void Texture_SetTexture(int texture_id);

DirectX::XMUINT2 Texture_GetSize(int texture_id);
unsigned int Texture_GetWidth(int texture_id);
unsigned int Texture_GetHeight(int texture_id);

#endif
