/*ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

 Direct3D勉強

 
													Author : SongLi
													Date : 2026/06/03
													
ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー*/

#include <d3d11.h> 
#include "direct3d.h"
#include<iostream>
#pragma comment(lib, "d3d11.lib")

using namespace std;
// Direct3D接口
static ID3D11Device* g_pDevice{ nullptr };
static ID3D11DeviceContext* g_pDeviceContext{ nullptr };
static IDXGISwapChain* g_pSwapChain {nullptr};

//Direct3Dのバックバッファ関連
static ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
//渲染RGB颜色

static ID3D11Texture2D* g_pDepthStencilBuffer = nullptr;
//渲染深度

static ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
//深度Buffer的View 这块内存当作深度表用

//bool creatRenderTarget() {


//}


bool Direct3D_Initialize(HWND window_handle)
{
    /*
    デバイス、スワップチェーン、コンテキスト⽣成
    先创建说明书swap_chain_desc
    */
    DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
    //这块告诉Windows我要这样的画布，上面这个swap_chain_desc就是说明书
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.BufferCount = 2;
    //2页画布
    // swap_chain_desc.BufferDesc.Width = 0; 
    // swap_chain_desc.BufferDesc.Height = 0; 
    // ⇒ ウィンドウサイズに合わせて⾃動的に設定される
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //红绿蓝都是8字节透明度也是，所以一块32字节
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.SampleDesc.Count = 1;
    //不做抗锯齿
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swap_chain_desc.OutputWindow = window_handle;
    //画在这个窗口上
    UINT device_flags = 0;

    #if defined(DEBUG) || defined(_DEBUG) 
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
    //开Debug模式
    #endif 

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    //然后创建D3D设备和交换链 里面会用到上面的说明书
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        device_flags,
        levels,
        ARRAYSIZE(levels),
        D3D11_SDK_VERSION,
        &swap_chain_desc,//上面的说明书
        &g_pSwapChain,//在这里就填上了Device和Context和SwapChain的指针了
        &g_pDevice,
        &feature_level,
        &g_pDeviceContext
    );

    if (FAILED(hr)) {
        MessageBox(window_handle,"Direct3D初期化に失敗しました", "エラー", MB_OK|MB_ICONERROR);
        return false;
    }

    if (!createBackBuffer()) {
        Direct3D_Finalize();
        return false;
    }

    static D3D11_VIEWPORT g_Viewport{};

    g_Viewport.TopLeftX = 0.0f;
    g_Viewport.TopLeftY = 0.0f;
    g_Viewport.Width = (float)SCREEN_WIDTH;
    g_Viewport.Height = (float)SCREEN_HEIGHT;
    g_Viewport.MinDepth = 0.0f;
    g_Viewport.MaxDepth = 1.0f;
    //最后创建一个view 有效绘制区域
    //MinDepth和MaxDepth是深度范围

    // ビューポートの設定
    g_pDeviceContext->RSSetViewports(1, &g_Viewport);

    return true;
}
//这一切初始化完，我就有
//g_pDevice：创建 纹理/缓冲
//g_pDeviceContext：发绘制命令
//g_pSwapChain：有 2 块画布，但还没显示内容
//g_pRenderTargetView：知道 往哪画颜色
//g_pDepthStencilView：知道 往哪写深度
//g_Viewport：知道 1920x1080 坐标范围



void Direct3D_Finalize() {
    releaseBackBuffer();
    SAFE_RELEASE(g_pSwapChain);
    SAFE_RELEASE(g_pDeviceContext);
    SAFE_RELEASE(g_pDevice);
}

void Direct3D_DrawBegin()//清空颜色和深度缓冲 然后告诉GPU接下来画到哪里
{
    //  本帧清屏用的颜色：RGBA 四个分量，范围 0.0~1.0（黑底，A=1 不透明）
    float clear_color[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

    //  把颜色画布整块涂成 clear_color（擦掉上一帧的画面，避免残影）
    //  g_pRenderTargetView = 后台缓冲的颜色视图，GPU 最终 RGB 画在这里
    g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clear_color);

    //  重置深度缓冲：每个像素深度设为 1.0（最远），模板设为 0
    g_pDeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    //  告诉 GPU接下来 Draw 往哪输出：颜色 → 后台画布，深度 → 深度缓冲
    //   参数 1 = 只绑定 1 个颜色目标（RenderTargetView）
    g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
}

void Direct3D_Present()//翻页显示 显示到屏幕
{
    g_pSwapChain->Present(1, 0);
}


ID3D11Device* Direct3D_GetDevice()
{
    return g_pDevice;
}

ID3D11DeviceContext* Direct3D_GetDeviceContext()
{
    return g_pDeviceContext;
}


//第二张后备画布的Buffer创建
bool createBackBuffer() {
    HRESULT hr;
    ID3D11Texture2D* back_buffer_pointer = nullptr;
    // バックバッファの取得 
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (void**)&back_buffer_pointer);
        //这里hr是失败码，如果失败了就返回false
        //g_pSwapChain拿到后备画布的指针
    if (FAILED(hr)) {
        MessageBox(NULL, "バックバッファの取得に失敗しました", "エラー", MB_OK|MB_ICONERROR);
        return false;
    }
    // バックバッファのレンダーターゲットビューの⽣成 
    hr = g_pDevice->CreateRenderTargetView(back_buffer_pointer, nullptr,
        &g_pRenderTargetView);
    //Device创建RenderTargetView 这个View就是告诉GPU往哪里画颜色

    if (FAILED(hr)) {
        back_buffer_pointer->Release();
        MessageBox(NULL, "バックバッファのレンダーターゲットビューの⽣成に失敗しました", "エラー", MB_OK|MB_ICONERROR);
        return false;
    }
    // バックバッファの状態（情報）を取得 深度缓冲
    D3D11_TEXTURE2D_DESC backBufferDesc{};
    back_buffer_pointer->GetDesc(&backBufferDesc);//先拿到大小，然后释放之前的指针
    back_buffer_pointer->Release(); // バックバッファのポインタは不要なので解放 
    // デプスステンシルバッファの⽣成 
    D3D11_TEXTURE2D_DESC depth_stencil_desc{};//按照后备画布的大小创建深度缓冲
    depth_stencil_desc.Width = backBufferDesc.Width;
    depth_stencil_desc.Height = backBufferDesc.Height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;
    hr = g_pDevice->CreateTexture2D(&depth_stencil_desc, nullptr,
        &g_pDepthStencilBuffer);
    if (FAILED(hr)) {
        cout << "デプスステンシルバッファの⽣成に失敗しました" << std::endl;
        SAFE_RELEASE(g_pRenderTargetView);
        return false;
    }
    // デプスステンシルビューの⽣成 

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
    depth_stencil_view_desc.Format = depth_stencil_desc.Format;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;
    depth_stencil_view_desc.Flags = 0;
    hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilBuffer,
        &depth_stencil_view_desc, &g_pDepthStencilView);
    if (FAILED(hr)) {
        SAFE_RELEASE(g_pDepthStencilBuffer);
        SAFE_RELEASE(g_pRenderTargetView);
        cout << "デプスステンシルビューの⽣成に失敗しました" << std::endl;
        return false;
    }
    return true;
}

void releaseBackBuffer() {
    SAFE_RELEASE(g_pRenderTargetView);
    SAFE_RELEASE(g_pDepthStencilBuffer);
    SAFE_RELEASE(g_pDepthStencilView);
}