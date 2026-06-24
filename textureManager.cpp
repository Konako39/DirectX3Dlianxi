#include "textureManager.h"
#include"direct3d.h"
#include"WICTextureLoader11.h"

#include <string>
#include <DirectXMath.h>
using namespace DirectX;

static constexpr int TEXTURE_MAX = 1024;
//管理するテクスチャの最大数

struct Texture {
	std::wstring filename;
	unsigned int width = 0;
	unsigned int height = 0;
	ID3D11Resource* pTexture = nullptr; //テクスチャリソース
	ID3D11ShaderResourceView* pTextureView = nullptr;//シェーダーリソースビュー
	//pTextureView是给着色器用的试图 所谓View视图的解释
	//GPU不能擅自摸显存，View就是告诉GPU该用什么方式去解读这个显存
	//同一块显存，可以说当画布用，也可以当深度表用，取决于GPU的访问方式
};

static Texture g_Textures[TEXTURE_MAX];

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

void Texture_Finalize()
{
	Texture_AllRelease();
}


int Texture_Load(const wchar_t* pFileName, bool bMipMap)
{
	if (!pFileName || pFileName[0] == L'\0') {
		return TEXTURE_INVALID_ID;
	}//安全检查 没有返回-1 h里定义了

	for (int i = 0; i < TEXTURE_MAX; i++) {
		if (!g_Textures[i].pTexture) continue;
		// 同じファイル名が見つかったら、そのIDを返す（新しく作らない）
		if (g_Textures[i].filename == pFileName) {
			return i;
		}
	}//找到了就返回，没找到continue	都没找到↓


	for (int i = 0; i < TEXTURE_MAX; i++) {
		// 空いている場所を探す
		if (g_Textures[i].pTexture) continue;
		HRESULT hr;
		// ミップマップの有無で読み込み関数を分ける mipmap是多级渐进纹理，就是放大缩小后该怎么取色
		//上课老师讲的黑白格子那个例子
		if (bMipMap) {
			hr = CreateWICTextureFromFile(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), pFileName,
				&g_Textures[i].pTexture, &g_Textures[i].pTextureView);
		}
		else {
			hr = CreateWICTextureFromFile(Direct3D_GetDevice(), pFileName,
				&g_Textures[i].pTexture, &g_Textures[i].pTextureView);
		}
			//这里就是加个GetDeviceContext会自动生成Mipmap链的
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"テクスチャの読み込みに失敗しました、textureManagerテクスチャロードのところ", pFileName,
				MB_OK | MB_ICONERROR);
			break;
		}

		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)g_Textures[i].pTexture;
		D3D11_TEXTURE2D_DESC t2desc;
		pTexture->GetDesc(&t2desc);
		g_Textures[i].width = t2desc.Width;
		g_Textures[i].height = t2desc.Height;
		g_Textures[i].filename = pFileName;
		return i;
		//找到后传给pTexture 然后拿到宽高和文件名，赋值给g_Textures[i]
	}

	return TEXTURE_INVALID_ID;
}

void Texture_Release(int texture_id)
{
	if (texture_id < 0 || texture_id >= TEXTURE_MAX) {
		return;
	}
	Texture& t = g_Textures[texture_id];
	SAFE_RELEASE(t.pTextureView);
	SAFE_RELEASE(t.pTexture);
	t.filename.clear();
	t.width = 0;
	t.height = 0;
}

void Texture_Release(const int* pTextureIDs, int count)
{
	if (!pTextureIDs || count <= 0) {
		return;
	}
	for (int i = 0; i < count; i++) {
		if (pTextureIDs[i] < 0 || pTextureIDs[i] >= TEXTURE_MAX) {
			continue;
		}
		Texture_Release(pTextureIDs[i]);
	}
}

void Texture_AllRelease()
{
	for (int i = 0; i < TEXTURE_MAX; i++) {
		Texture_Release(i);
	}
}

void Texture_SetTexture(int texture_id)
{
	if (texture_id < 0 || texture_id >= TEXTURE_MAX) {
		return;
	}
	if (!g_Textures[texture_id].pTextureView) {
		return;
	}
	ID3D11ShaderResourceView* pSRV = g_Textures[texture_id].pTextureView;
	Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &pSRV);
	//把这张图的视图绑定到像素着色器的t0寄存器 0号槽开始第一个资源是pSRV
}

DirectX::XMUINT2 Texture_GetSize(int texture_id)
{
	if (texture_id < 0 || texture_id >= TEXTURE_MAX) {
		return { 0, 0 };
	}
	return { g_Textures[texture_id].width, g_Textures[texture_id].height };
}


unsigned int Texture_GetHeight(int texture_id)
{
	if (texture_id < 0 || texture_id >= TEXTURE_MAX) {
		return 0;
	}
	return g_Textures[texture_id].height;
}
unsigned int Texture_GetWidth(int texture_id)
{
	if (texture_id < 0 || texture_id >= TEXTURE_MAX) {
		return 0;
	}
	return g_Textures[texture_id].width;

}
