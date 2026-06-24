/*==============================================================================

    [shader.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "debug_ostream.h"
#include <fstream>
#include "shader.h"

static ID3D11VertexShader* g_pVertexShader = nullptr;   // 顶点着色器：GPU 上算每个顶点最终屏幕位置
static ID3D11InputLayout* g_pInputLayout = nullptr;     // 输入布局：告诉 GPU 顶点缓冲里各字段怎么解读
static ID3D11Buffer* g_pVSConstantBuffer = nullptr;     // 常量缓冲 b0：CPU 每帧上传矩阵给顶点着色器
static ID3D11PixelShader* g_pPixelShader = nullptr;     // 像素着色器：GPU 上算每个像素最终颜色

// Direct3D设备和设备上下文
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    HRESULT hr;

    // 参数检查
    if (!pDevice || !pContext) {
        MessageBox(nullptr, "无法打开文件\n\nshader_vertex_2d.cso", "", MB_OK);
        return false;
    }

    // 保存设备和上下文
    g_pDevice = pDevice;
    g_pContext = pContext;

    // 读取预编译顶点着色器（.hlsl 编译产物 .cso，GPU 可执行的小程序）
    std::ifstream ifs_vs("shader_vertex_2d.cso", std::ios::binary);

    if (!ifs_vs) {
        MessageBox(nullptr, "无法打开文件\n\nshader_vertex_2d.cso", "", MB_OK);
        return false;
    }

    // 获取文件大小
    ifs_vs.seekg(0, std::ios::end);      // 移动到文件末尾
    std::streamsize filesize = ifs_vs.tellg(); // 获取文件大小
    ifs_vs.seekg(0, std::ios::beg);      // 回到文件开头

    // 分配用于存放二进制数据的缓冲区
    unsigned char* vsbinary_pointer = new unsigned char[filesize];

    // 读取二进制数据
    ifs_vs.read((char*)vsbinary_pointer, filesize);

    // 关闭文件
    ifs_vs.close();

    // 创建顶点着色器对象（加载 shader_vertex_2d.hlsl 编译后的程序）
    hr = g_pDevice->CreateVertexShader(
        vsbinary_pointer,
        filesize,
        nullptr,
        &g_pVertexShader);

    if (FAILED(hr)) {
        hal::dout << "Shader_Initialize() : 创建顶点着色器失败" << std::endl;

        // 防止内存泄漏
        delete[] vsbinary_pointer;

        return false;
    }

    // [输入布局] 偏移必须和 sizeof(Vertex)==36 一致（无 padding）
    // XMFLOAT3(12) + XMFLOAT4(16) + XMFLOAT2(8) = 36 字节/顶点
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // 获取元素数量
    UINT num_elements = ARRAYSIZE(layout);

    // 创建输入布局（必须传入 VS 的 .cso，D3D 要对照着色器里的语义 POSITION/COLOR/TEXCOORD）
    hr = g_pDevice->CreateInputLayout(
        layout,
        num_elements,
        vsbinary_pointer,
        filesize,
        &g_pInputLayout);

    // 释放二进制数据缓冲区
    delete[] vsbinary_pointer;

    if (FAILED(hr)) {
        hal::dout << "Shader_Initialize() : 创建输入布局失败" << std::endl;
        return false;
    }

    // [矩阵常量缓冲 b0] CPU→GPU 的小数据通道，对应 hlsl 里 cbuffer VS_CONSTANT_BUFFER : register(b0)
    // 每帧由 Shader_SetMatrix 写入缩放/投影矩阵，顶点着色器用 mtx 把顶点变到屏幕坐标
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(XMFLOAT4X4);
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    g_pDevice->CreateBuffer(
        &buffer_desc,
        nullptr,
        &g_pVSConstantBuffer);

    // 读取预编译像素着色器
    std::ifstream ifs_ps("shader_pixel_2d.cso", std::ios::binary);

    if (!ifs_ps) {
        MessageBox(
            nullptr,
            "读取像素着色器失败\n\nshader_pixel_2d.cso",
            "错误",
            MB_OK);

        return false;
    }

    // 获取文件大小
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);

    // 分配缓冲区
    unsigned char* psbinary_pointer = new unsigned char[filesize];

    // 读取二进制数据
    ifs_ps.read((char*)psbinary_pointer, filesize);

    // 关闭文件
    ifs_ps.close();

    // 创建像素着色器对象（加载 shader_pixel_2d.cso，负责每个像素的最终颜色）
    hr = g_pDevice->CreatePixelShader(
        psbinary_pointer,
        filesize,
        nullptr,
        &g_pPixelShader);

    // 释放缓冲区
    delete[] psbinary_pointer;

    if (FAILED(hr)) {
        hal::dout << "Shader_Initialize() : 创建像素着色器失败" << std::endl;
        return false;
    }

    return true;
}

void Shader_Finalize()
{
    SAFE_RELEASE(g_pPixelShader);
    SAFE_RELEASE(g_pVSConstantBuffer);
    SAFE_RELEASE(g_pInputLayout);
    SAFE_RELEASE(g_pVertexShader);
}

void Shader_SetMatrix(const DirectX::XMMATRIX& matrix)
{
    XMFLOAT4X4 transpose;

    // 上传矩阵到常量缓冲 b0（HLSL 矩阵是列主序，C++ 行主序，所以要转置）
    XMStoreFloat4x4(
        &transpose,
        XMMatrixTranspose(matrix));

    g_pContext->UpdateSubresource(
        g_pVSConstantBuffer,
        0,
        nullptr,
        &transpose,
        0,
        0);
    // 注意：数据写入缓冲后，还需 Shader_Begin 里 VSSetConstantBuffers(0,...) 绑定到 b0 才生效
}

void Shader_Begin()
{
    // 每次 Sprite_Draw 前调用：把本项目的着色器管线绑到 GPU 上

    // 绑定顶点着色器（算顶点位置，见 shader_vertex_2d.hlsl）
    g_pContext->VSSetShader(
        g_pVertexShader,
        nullptr,
        0);

    // 绑定像素着色器（算像素颜色，见 shader_pixel_2d.hlsl）
    g_pContext->PSSetShader(
        g_pPixelShader,
        nullptr,
        0);

    // 绑定输入布局（Draw 时 GPU 按此格式读 sprite 顶点缓冲）
    g_pContext->IASetInputLayout(
        g_pInputLayout);

    // 把矩阵常量缓冲挂到顶点着色器 b0 槽（对应 hlsl 的 register(b0) / mtx）
    g_pContext->VSSetConstantBuffers(
        0,
        1,
        &g_pVSConstantBuffer);
}