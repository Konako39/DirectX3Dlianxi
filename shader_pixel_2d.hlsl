/* 像素着色器 (Pixel Shader)
 * 职责：对每个像素执行一次，决定最终输出到画布的颜色
 * 对应 C++：shader.cpp 加载本文件编译出的 shader_pixel_2d.cso
 * 贴图绑定：textureManager.cpp Texture_SetTexture -> PSSetShaderResources(0,...) 对应 t0
 * 采样器绑定：sprite.cpp PSSetSamplers(0,...) 对应 s0
 */

// 顶点着色器输出，经光栅化后插值到每个像素
struct PS_IN
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;     // 顶点色插值结果
    float2 uv : TEXCOORD0;     // UV 插值结果
};

SamplerState major_sampler : register(s0);  // 采样方式（点采样/WRAP 等，sprite.cpp 设置）
Texture2D major_texture : register(t0);    // 当前要画的贴图（Texture_SetTexture 绑定）

float4 main(PS_IN ps_in) : SV_TARGET
{
    // 最终色 = 贴图该 UV 处的颜色 × 顶点色（可调色、调透明度）
    return major_texture.Sample(major_sampler, ps_in.uv) * ps_in.color;
}
