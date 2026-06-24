/* 顶点着色器 (Vertex Shader)
 * 职责：对每个顶点执行一次，输出屏幕上的位置 + 传给像素着色器的颜色/UV
 * 对应 C++：shader.cpp 加载本文件编译出的 shader_vertex_2d.cso
 */

// [常量缓冲 b0] CPU 每帧写入，见 shader.cpp Shader_SetMatrix + VSSetConstantBuffers(0,...)
// mtx = 缩放矩阵 × 正交投影矩阵，把本地坐标变到 1600x900 屏幕空间
cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 mtx;
};

// [常量缓冲 b1] CPU 写入精灵屏幕位置，见 sprite.cpp g_pTestBuffer + VSSetConstantBuffers(1,...)
cbuffer TestBuffer : register(b1)
{
    float2 position;  // 精灵左上角偏移 (x, y)
    float2 dummy;     // 补齐到 16 字节对齐（cbuffer 规则）
}

// 输入：来自 sprite.cpp 顶点缓冲，字段顺序由 shader.cpp 的 InputLayout 定义
struct VS_IN
{
    float4 posL : POSITION0;   // 本地空间位置（单位矩形 0~1 或屏幕坐标）
    float4 color : COLOR0;     // 顶点色（含透明度，会传到像素着色器相乘）
    float2 ux : TEXCOORD0;     // 纹理 UV 坐标
};

// 输出：传给像素着色器（以及光栅化阶段）
struct VS_OUT
{
    float4 posH : SV_Position; // 屏幕/裁剪空间位置（GPU 用来决定画在哪）
    float4 color : COLOR0;
    float2 ux : TEXCOORD0;
};

VS_OUT main(VS_IN vs_in)
{
    VS_OUT vs_out;

    float4 pos = vs_in.posL;
    vs_out.posH = mul(pos, mtx);     // 缩放、平移、投影已在 C++ 的 mtx 里完成
    vs_out.color = vs_in.color;
    vs_out.ux = vs_in.ux;
    return vs_out;
}
