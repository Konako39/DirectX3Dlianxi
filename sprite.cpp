#include <iostream>

#include"sprite.h"
#include "direct3d.h"
#include"shader.h"
#include"WICTextureLoader11.h"
#include <DirectXmath.h>
#include <d3d11.h>
#include"textureManager.h"
using namespace DirectX;


static ID3D11Buffer* g_pVertexBuffer = nullptr;
static ID3D11SamplerState* g_pSamplerState{ nullptr };
static ID3D11BlendState* g_pBlendState{ nullptr };
static ID3D11DepthStencilState* g_pDepthStencilState{ nullptr };
static ID3D11Buffer* g_pTestBuffer{ nullptr };


struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 texooord;
};
static_assert(sizeof(Vertex) == 36, "Vertex stride must match shader InputLayout (36 bytes)");

static constexpr int NUM_VERTEX{ 4 };

bool Sprite_Initialize()
{

	// vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Vertex v[NUM_VERTEX]{};

	v[0].position = { -0.5f,-0.5f, 0.0f };
	v[1].position = { 0.5f,-0.5f, 0.0f };
	v[2].position = { -0.5f,0.5f, 0.0f };
	v[3].position = { 0.5f,0.5f, 0.0f };

	v[0].texooord = { 0.0f,0.0f };
	v[1].texooord = { 1.0f,0.0f };
	v[2].texooord = { 0.0f,1.0f };
	v[3].texooord = { 1.0f,1.0f };

	HRESULT hr = Direct3D_GetDevice()->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
	if (FAILED(hr)) {
		return false;
	}

	D3D11_BUFFER_DESC cb_desc{};
	cb_desc.ByteWidth = sizeof(XMFLOAT4);
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// constant buffer
	hr = Direct3D_GetDevice()->CreateBuffer(&cb_desc, nullptr, &g_pTestBuffer);




	if (FAILED(hr))
	{
		MessageBox(NULL, "Constant buffer create failed", "Error", MB_OK);
		return false;
	}


	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampler_desc.MaxAnisotropy = 16;

	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	Direct3D_GetDevice()->CreateSamplerState(&sampler_desc, &g_pSamplerState);


	// blend state
	D3D11_BLEND_DESC blend_desc{};
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// alpha blend: src * a + dest * (1-a) blend这里都是半透明混合

	Direct3D_GetDevice()->CreateBlendState(&blend_desc, &g_pBlendState);

	// depth stencil
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
	depth_stencil_desc.DepthEnable = FALSE;
	//深度测试关闭 

	hr = Direct3D_GetDevice()->CreateDepthStencilState(&depth_stencil_desc, &g_pDepthStencilState);

	return true;
}

void Sprite_Finalize()
{
	SAFE_RELEASE(g_pDepthStencilState);
	SAFE_RELEASE(g_pBlendState);
	SAFE_RELEASE(g_pSamplerState);
	//SAFE_RELEASE(g_pShaderResourceView);
	SAFE_RELEASE(g_pTestBuffer);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Draw(int texture_id, float position_x, float position_y)
{
	static const DirectX::XMFLOAT4 kDefaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Sprite_Draw(texture_id, position_x, position_y, kDefaultColor);
}

void Sprite_Draw(int texture_id, float position_x, float position_y, const DirectX::XMFLOAT4& color) {
	Sprite_Draw(texture_id, position_x, position_y, (float)Texture_GetWidth(texture_id),
		(float)Texture_GetHeight(texture_id),color);
}

void Sprite_Draw(int texture_id, float position_x, float position_y, float width, float height)
{
	static const DirectX::XMFLOAT4 kDefaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Sprite_Draw(texture_id, position_x, position_y, width, height, kDefaultColor);
}

void Sprite_Draw(int texture_id, float position_x, float position_y, float width, float height, const DirectX::XMFLOAT4& color)
{
	Shader_Begin();

	XMFLOAT4 zeroPos{ 0.0f, 0.0f, 0.0f, 0.0f };
	Direct3D_GetDeviceContext()->UpdateSubresource(g_pTestBuffer, 0, nullptr, &zeroPos, 0, 0);
	Direct3D_GetDeviceContext()->VSSetConstantBuffers(1, 1, &g_pTestBuffer);

	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	D3D11_MAPPED_SUBRESOURCE msr;
	Direct3D_GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* vert = (Vertex*)msr.pData;

	vert[0].position = { position_x,		 position_y, 0.0f };
	vert[1].position = { position_x + width, position_y, 0.0f };
	vert[2].position = { position_x,		 position_y + height, 0.0f };
	vert[3].position = { position_x + width, position_y + height, 0.0f };

	vert[0].color = color;
	vert[1].color = color;
	vert[2].color = color;
	vert[3].color = color;

	vert[0].texooord = { 0.0f,0.0f };
	vert[1].texooord = { 1.0f ,0.0f };
	vert[2].texooord = { 0.0f,1.0f };
	vert[3].texooord = { 1.0f ,1.0f };

	Direct3D_GetDeviceContext()->Unmap(g_pVertexBuffer, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Direct3D_GetDeviceContext()->PSSetSamplers(0, 1, &g_pSamplerState);

	Texture_SetTexture(texture_id);

	Direct3D_GetDeviceContext()->OMSetBlendState(g_pBlendState, nullptr, 0xffffffff);

	Direct3D_GetDeviceContext()->OMSetDepthStencilState(g_pDepthStencilState, 0);
	Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);

}

void Sprite_Draw(int texture_id, float position_x, float position_y, int texture_x, int texture_y, int texture_width, int texture_height)
{
	static const DirectX::XMFLOAT4 kDefaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Sprite_Draw(texture_id, position_x, position_y, texture_x, texture_y, texture_width, texture_height, kDefaultColor);
}

void Sprite_Draw(int texture_id, float position_x, float position_y, int texture_x, int texture_y, int texture_width, int texture_height, const DirectX::XMFLOAT4& color)
{
	Sprite_Draw(texture_id, position_x, position_y, (float)texture_width, (float)texture_height, texture_x, texture_y, texture_width, texture_height, color);
}

void Sprite_Draw(int texture_id, float position_x, float position_y, float width, float height, int texture_x, int texture_y, int texture_width, int texture_height)
{
	static const DirectX::XMFLOAT4 kDefaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Sprite_Draw(texture_id, position_x, position_y, width, height, texture_x, texture_y, texture_width, texture_height, kDefaultColor);
}

//下面这个重载是有切图的
void Sprite_Draw(int texture_id, float position_x, float position_y, float width, float height, int texture_x, int texture_y, int texture_width, int texture_height, const DirectX::XMFLOAT4& color)
{
	XMFLOAT4 zeroPos{ 0.0f, 0.0f, 0.0f, 0.0f };

	Shader_Begin();

	Direct3D_GetDeviceContext()->UpdateSubresource(g_pTestBuffer, 0, nullptr, &zeroPos, 0, 0);
	Direct3D_GetDeviceContext()->VSSetConstantBuffers(1, 1, &g_pTestBuffer);

	XMMATRIX mtxS = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX mtxT = XMMatrixTranslation(position_x, position_y, 0.0f);
	XMMATRIX mtxP = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	XMMATRIX mtx = mtxS * mtxT * mtxP;

	Shader_SetMatrix(mtx);

	D3D11_MAPPED_SUBRESOURCE msr;
	Direct3D_GetDeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* vert = (Vertex*)msr.pData;

	vert[0].position = { 0.0f, 0.0f, 0.0f };
	vert[1].position = { 1.0f, 0.0f, 0.0f };
	vert[2].position = { 0.0f, 1.0f, 0.0f };
	vert[3].position = { 1.0f, 1.0f, 0.0f };

	vert[0].color = color;
	vert[1].color = color;
	vert[2].color = color;
	vert[3].color = color;

	float u0 = texture_x / (float)Texture_GetWidth(texture_id);
	float tv0 = texture_y / (float)Texture_GetHeight(texture_id);
	float u1 = (texture_x + texture_width) / (float)Texture_GetWidth(texture_id);
	float tv1 = (texture_y + texture_height) / (float)Texture_GetHeight(texture_id);

	vert[0].texooord = { u0, tv0 };
	vert[1].texooord = { u1, tv0 };
	vert[2].texooord = { u0, tv1 };
	vert[3].texooord = { u1, tv1 };

	Direct3D_GetDeviceContext()->Unmap(g_pVertexBuffer, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3D_GetDeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	Direct3D_GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Direct3D_GetDeviceContext()->PSSetSamplers(0, 1, &g_pSamplerState);

	Texture_SetTexture(texture_id);

	Direct3D_GetDeviceContext()->OMSetBlendState(g_pBlendState, nullptr, 0xffffffff);

	Direct3D_GetDeviceContext()->OMSetDepthStencilState(g_pDepthStencilState, 0);
	Direct3D_GetDeviceContext()->Draw(NUM_VERTEX, 0);
}
