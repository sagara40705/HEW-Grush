#include "Sprite.h"
#include "ShaderList.h"
#include <cstring> // memset
using namespace DirectX;

// static メンバ定義
std::unique_ptr<MeshBuffer> Sprite::s_quad;
std::unique_ptr<VertexShader> Sprite::s_3dVS;
std::unique_ptr<PixelShader> Sprite::s_3dPS;

void Sprite::Init()
{
    // ===== 共通 Quad メッシュ生成 =====
    // ローカル座標: (0,0)〜(1,1) を想定
    struct Vertex
    {
        XMFLOAT3 pos;
        XMFLOAT2 uv;
    };

    Vertex vtx[4] =
    {
    { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 左上
    { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, // 左下
    { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }, // 右上
    { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, // 右下
    };

    MeshBuffer::Description desc{};
    desc.pVtx = vtx;
    desc.vtxSize = sizeof(Vertex);
    desc.vtxCount = _countof(vtx);
    desc.isWrite = false;
    desc.pIdx = nullptr;
    desc.idxSize = 0;
    desc.idxCount = 0;
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

    s_quad = std::make_unique<MeshBuffer>();
    s_quad->Create(desc);

    Create3dVS();
    Create3dPS();
}

void Sprite::Uninit()
{
    s_quad.reset();
}

void Sprite::Draw()
{
    s_quad->Draw();
}

void Sprite::Create3dVS()
{
    const char* code = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};
struct VS_OUT {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer SpriteParam : register(b1)
{
    float4 offset_size;   // xy=offset, zw=size（※単位はあなたの運用に合わせる）
    float4 uvPos_uvScale; // xy=uvPos, zw=uvScale
    float4 color;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT o;

    // Quad(0..1) を中心基準にしてサイズ/オフセット適用（ローカル平面上）
    float2 local = (vin.pos.xy - float2(0.5f, 0.5f)) * offset_size.zw + offset_size.xy;

    // ローカル平面( z=0 )を world に乗せる（Transform の回転/スケールも効く）
    float4 wpos = mul(float4(local.xy, 0.0f, 1.0f), world);
    float4 vpos = mul(wpos, view);
    o.pos = mul(vpos, proj);

    o.uv = vin.uv * uvPos_uvScale.zw + uvPos_uvScale.xy;
    o.color = color;
    return o;
}
)EOT";
    s_3dVS = std::make_unique<VertexShader>();
    s_3dVS->Compile(code);
}

void Sprite::Create3dPS()
{
    const char* code = R"EOT(
struct PS_IN {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex     : register(t0);
SamplerState samp : register(s0);

cbuffer SpriteParam : register(b1)
{
    float4 offset_size;
    float4 uvPos_uvScale;
    float4 color;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 t = tex.Sample(samp, pin.uv);
    return t * pin.color;
}
)EOT";

    s_3dPS = std::make_unique<PixelShader>();
    s_3dPS->Compile(code);
}
