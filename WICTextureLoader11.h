//--------------------------------------------------------------------------------------
// 文件：WICTextureLoader11.h
//
// 用于加载 WIC（Windows Imaging Component）图像，
// 并为其创建 Direct3D 运行时纹理的函数
// （如果支持的话，会自动生成 Mipmap）
//
// 注意：假设应用程序已经调用过 CoInitializeEx
//`
// 警告：如果向 CreateWICTexture* 函数传入 d3dContext 实例
// 以支持自动生成 Mipmap，则这些函数不是线程安全的。
//
// 注意：这些函数适用于作为简单二维纹理（2D Texture）创建的图像。
//
// 对于更复杂的资源，DDSTextureLoader 是一个优秀的轻量级运行时加载器。
//
// 如果需要功能完整的 DDS 文件读取、写入以及纹理处理流程，
// 请参考 'Texconv' 示例程序和 'DirectXTex' 库。
//
// 版权所有 (c) Microsoft Corporation.
// 根据 MIT 许可证授权使用。
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#include <d3d11_1.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"windowscodecs.lib")

#include <cstddef>
#include <cstdint>

namespace DirectX
{
#ifndef WIC_LOADER_FLAGS_DEFINED
#define WIC_LOADER_FLAGS_DEFINED
    enum WIC_LOADER_FLAGS : uint32_t
    {
        WIC_LOADER_DEFAULT = 0,
        WIC_LOADER_FORCE_SRGB = 0x1,
        WIC_LOADER_IGNORE_SRGB = 0x2,
        WIC_LOADER_SRGB_DEFAULT = 0x4,
        WIC_LOADER_FIT_POW2 = 0x20,
        WIC_LOADER_MAKE_SQUARE = 0x40,
        WIC_LOADER_FORCE_RGBA32 = 0x80,
    };

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
#endif

    DEFINE_ENUM_FLAG_OPERATORS(WIC_LOADER_FLAGS);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

    // Standard version
    HRESULT CreateWICTextureFromMemory(
        _In_ ID3D11Device* d3dDevice,
        _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
        _In_ size_t wicDataSize,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView,
        _In_ size_t maxsize = 0) noexcept;

    HRESULT CreateWICTextureFromFile(
        _In_ ID3D11Device* d3dDevice,
        _In_z_ const wchar_t* szFileName,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView,
        _In_ size_t maxsize = 0) noexcept;

    // Standard version with optional auto-gen mipmap support
    HRESULT CreateWICTextureFromMemory(
        _In_ ID3D11Device* d3dDevice,
        _In_opt_ ID3D11DeviceContext* d3dContext,
        _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
        _In_ size_t wicDataSize,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView,
        _In_ size_t maxsize = 0) noexcept;

    HRESULT CreateWICTextureFromFile(
        _In_ ID3D11Device* d3dDevice,
        _In_opt_ ID3D11DeviceContext* d3dContext,
        _In_z_ const wchar_t* szFileName,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView,
        _In_ size_t maxsize = 0) noexcept;

    // Extended version
    HRESULT CreateWICTextureFromMemoryEx(
        _In_ ID3D11Device* d3dDevice,
        _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
        _In_ size_t wicDataSize,
        _In_ size_t maxsize,
        _In_ D3D11_USAGE usage,
        _In_ unsigned int bindFlags,
        _In_ unsigned int cpuAccessFlags,
        _In_ unsigned int miscFlags,
        _In_ WIC_LOADER_FLAGS loadFlags,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

    HRESULT CreateWICTextureFromFileEx(
        _In_ ID3D11Device* d3dDevice,
        _In_z_ const wchar_t* szFileName,
        _In_ size_t maxsize,
        _In_ D3D11_USAGE usage,
        _In_ unsigned int bindFlags,
        _In_ unsigned int cpuAccessFlags,
        _In_ unsigned int miscFlags,
        _In_ WIC_LOADER_FLAGS loadFlags,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

    // Extended version with optional auto-gen mipmap support
    HRESULT CreateWICTextureFromMemoryEx(
        _In_ ID3D11Device* d3dDevice,
        _In_opt_ ID3D11DeviceContext* d3dContext,
        _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
        _In_ size_t wicDataSize,
        _In_ size_t maxsize,
        _In_ D3D11_USAGE usage,
        _In_ unsigned int bindFlags,
        _In_ unsigned int cpuAccessFlags,
        _In_ unsigned int miscFlags,
        _In_ WIC_LOADER_FLAGS loadFlags,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

    HRESULT CreateWICTextureFromFileEx(
        _In_ ID3D11Device* d3dDevice,
        _In_opt_ ID3D11DeviceContext* d3dContext,
        _In_z_ const wchar_t* szFileName,
        _In_ size_t maxsize,
        _In_ D3D11_USAGE usage,
        _In_ unsigned int bindFlags,
        _In_ unsigned int cpuAccessFlags,
        _In_ unsigned int miscFlags,
        _In_ WIC_LOADER_FLAGS loadFlags,
        _Outptr_opt_ ID3D11Resource** texture,
        _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;
}
