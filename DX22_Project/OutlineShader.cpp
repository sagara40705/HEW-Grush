// OutlineShader.cpp
//======================================================
//  モデルアウトライン用シェーダークラスの中身
//  ・VS : モデルを普通に描画（WVP で変換）
//  ・PS : 単色で発光色を出す（光るふち用）
//======================================================
#include "OutlineShader.h"
#include <assert.h>

//=======================
// HLSL : VS
//=======================
static const char* g_OutlineVSCode = R"(
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

struct VS_IN
{
    float3 pos    : POSITION;
    float3 normal : NORMAL0;
};

struct VS_OUT
{
    float4 pos   : SV_POSITION;
    float3 normal : NORMAL0;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // 法線方向へ膨らませる（アウトライン太さ）
    float3 expandedPos = vin.pos + vin.normal * 0.0004f;

    float4 wpos = mul(float4(expandedPos,1.0f), world);
    float4 vpos = mul(wpos, view);
    vout.pos = mul(vpos, proj);

    vout.normal = normalize(mul(vin.normal, (float3x3)world));
    return vout;
}
)";

//=======================
// HLSL : PS
//=======================
static const char* g_OutlinePSCode = R"(
float4 main(float3 normal : NORMAL0) : SV_TARGET
{
    // はっきり分かるネオングリーン
    float3 outlineColor   = float3(1.0f, 0.0f, 0.6f);
    float  outlineStrength = 1.0f;

    return float4(outlineColor * outlineStrength, 1.0f);
}
)";

OutlineShader::OutlineShader()
    : m_pVS(nullptr), m_pPS(nullptr)
{
}

OutlineShader::~OutlineShader()
{
    delete m_pVS;
    delete m_pPS;
}

bool OutlineShader::Init()
{
    m_pVS = new VertexShader();
    m_pPS = new PixelShader();

    HRESULT hr;
    hr = m_pVS->Compile(g_OutlineVSCode);
    if (FAILED(hr)) return false;

    hr = m_pPS->Compile(g_OutlinePSCode);
    if (FAILED(hr)) return false;

    WVPParam initParam{};
    m_pVS->WriteBuffer(0, &initParam);

    return true;
}

void OutlineShader::SetWVP(
    const DirectX::XMFLOAT4X4& w,
    const DirectX::XMFLOAT4X4& v,
    const DirectX::XMFLOAT4X4& p)
{
    WVPParam param{};
    param.world = w;
    param.view = v;
    param.proj = p;
    m_pVS->WriteBuffer(0, &param);
}

void OutlineShader::Bind()
{
    m_pVS->Bind();
    m_pPS->Bind();
}