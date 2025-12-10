// SkyDomeShader.cpp
//======================================================
//  スカイドーム（宇宙風の空）シェーダークラスの実装
//  ・VS : 球の頂点を WVP で変換しつつ、「ローカル方向ベクトル」を PS に渡す
//  ・PS : 方向ベクトルから宇宙っぽいグラデーションと星を生成する
//======================================================

#include "SkyDomeShader.h"
#include <assert.h>

//=======================
// HLSL コード（頂点シェーダー）
//=======================
// ここでは、球メッシュのローカル座標 vin.pos を正規化して
// 「どの方向を向いているか」というベクトルを PS に渡す。
// スカイドームの中心をカメラ位置に合わせて配置する前提なので、
// ローカル座標＝カメラから見た方向、とみなしてよい。
static const char* g_SkyVSCode = R"(
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// ← 新しく追加する定数バッファ（b1 を使用）
cbuffer SkyParam : register(b1)
{
    int   gUseTexture;   // 0: プロシージャル星空, 1: テクスチャ
    float3 _padding;     // 16バイト境界を合わせるためのダミー
};

// 頂点シェーダーの入力構造体
// POSITION と TEXCOORD0 は Geometory で作った頂点フォーマットに合わせる
struct VS_IN
{
  float3 pos : POSITION;   // Geometory の頂点レイアウトと一致
    float3 nor : NORMAL;     // ★必須：頂点フォーマットに合わせる
    float4 col : COLOR;      // ★必須
    float2 uv  : TEXCOORD0;  // ★必須
};

// 頂点シェーダーの出力構造体
struct VS_OUT
{
    float4 pos  : SV_POSITION;  // ラスタライザに渡す座標
    float3 dir  : TEXCOORD0;    // ローカル方向ベクトル（宇宙表現に使用）
    float2 uv   : TEXCOORD1;    // 予備（必要なら将来テクスチャ用に使う）
};

VS_OUT main(VS_IN vin)
{
     VS_OUT vout;

    float4 lpos = float4(vin.pos, 1.0f);

    // スカイドーム用の方向
    vout.dir = normalize(vin.pos);

    // ワールド変換
    float4 wpos = mul(lpos, world);
    float4 vpos = mul(wpos, view);
    float4 ppos = mul(vpos, proj);

    vout.pos = ppos;
    vout.uv  = vin.uv;

    return vout;
}
)";

//=======================
// HLSL コード（ピクセルシェーダー）
//=======================
// 宇宙風のグラデーション＋簡易的な星ノイズを生成する。
static const char* g_SkyPSCode = R"(
cbuffer SkyParam : register(b1)
{
    int   gUseTexture;   // 0: プロシージャル星空, 1: テクスチャ
    float3 _padding;
};

Texture2D    gSkyTex  : register(t0);
SamplerState gSkySamp : register(s0);

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
    float2 uv  : TEXCOORD1;
};

float rand2d(float2 p)
{
    float n = dot(p, float2(12.9898, 78.233));
    return frac(sin(n) * 43758.5453);
}

float4 main(PS_IN pin) : SV_TARGET
{
    float3 d = normalize(pin.dir);

    float t = saturate(d.y * 0.5f + 0.5f);
    float3 bottomColor = float3(0.02f, 0.02f, 0.08f);
    float3 topColor    = float3(0.0f,  0.0f,  0.02f);
    float3 skyColor    = lerp(bottomColor, topColor, t);

    float2 starCoord = d.xz * 50.0f;
    float n = rand2d(starCoord);
    float starMask      = step(0.985f, n);
    float starIntensity = lerp(0.4f, 1.0f, n);
    float3 starColor    = starMask * starIntensity * float3(1.0f, 1.0f, 1.0f);

    float3 procedural = saturate(skyColor + starColor);

    float4 col;
    if (gUseTexture != 0)
    {
        col = gSkyTex.Sample(gSkySamp, pin.uv);
    }
    else
    {
        col = float4(procedural, 1.0f);
    }

    return col;
}
)";

//------------------------------------------------------
// コンストラクタ
//------------------------------------------------------
SkyDomeShader::SkyDomeShader()
    : m_pVS(nullptr)
    , m_pPS(nullptr)
    , m_pSkyParamCB(nullptr)
    , m_useTexture(false)
{
}

//------------------------------------------------------
// デストラクタ
//------------------------------------------------------
SkyDomeShader::~SkyDomeShader()
{
    // VS / PS 自体の解放（既に書いてあればそれで OK）
    delete m_pVS;  m_pVS = nullptr;
    delete m_pPS;  m_pPS = nullptr;

    // gUseTexture 用定数バッファ
    SAFE_RELEASE(m_pSkyParamCB);

    // サンプラステート
    SAFE_RELEASE(m_pSampler);

    // SRV は Texture 側が所有しているのでここでは Release しない
    m_pSkySRV = nullptr;
}

//------------------------------------------------------
// Init
// シェーダーコンパイルと定数バッファの用意を行う
//------------------------------------------------------
bool SkyDomeShader::Init()
{
    // -------------- VS / PS コンパイル --------------
    m_pVS = new VertexShader();
    if (FAILED(m_pVS->Compile(g_SkyVSCode)))
    {
        return false; // VS のコンパイル失敗
    }

    m_pPS = new PixelShader();
    if (FAILED(m_pPS->Compile(g_SkyPSCode)))
    {
        return false; // PS のコンパイル失敗
    }

    ID3D11Device* device = GetDevice();

    // -------------- gUseTexture 用定数バッファ (b1) --------------
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;          // 毎フレーム書き換えるので DEFAULT
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.ByteWidth = sizeof(SkyParam);         // struct SkyParam と同じサイズ

    HRESULT hr = device->CreateBuffer(&cbd, nullptr, &m_pSkyParamCB);
    if (FAILED(hr)) return false;

    // -------------- サンプラステート作成 (s0) --------------
    // テクスチャサンプリングの方法を指定する
    D3D11_SAMPLER_DESC sd = {};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // 線形フィルタ
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;        // U 方向にループ
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;        // V 方向にループ
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    // ★ ここが元コードでは抜けていた
    hr = device->CreateSamplerState(&sd, &m_pSampler);
    if (FAILED(hr)) return false;

    return true;
}

void SkyDomeShader::SetUseTexture(bool use)
{
    // CPU 側フラグ
    m_useTexture = use;

    if (!m_pSkyParamCB) return;

    // HLSL 側の cbuffer gSkyParam (b1) へ書き込む値
    SkyParam param = {};
    param.useTexture = use ? 1 : 0;  // int にしておくと HLSL でも扱いやすい

    ID3D11DeviceContext* ctx = GetContext();
    ctx->UpdateSubresource(m_pSkyParamCB, 0, nullptr, &param, 0, 0);
}

void SkyDomeShader::SetTexture(ID3D11ShaderResourceView* srv)
{
    m_pSkySRV = srv;
}

//------------------------------------------------------
// SetWVP
// CPU から渡された WVP 行列を VS の定数バッファへ書き込む
//------------------------------------------------------
void SkyDomeShader::SetWVP(
    const DirectX::XMFLOAT4X4& world,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& proj)
{
    // 一旦 CPU 側の構造体にコピー
    WVPParam param = {};
    param.world = world;
    param.view = view;
    param.proj = proj;

    // slot 0 の定数バッファに書き込む
    // まだバッファが無ければ作成、あれば Update される想定
    m_pVS->WriteBuffer(0, &param);
}

//------------------------------------------------------
// Bind
// このシェーダーを現在の描画パイプラインにセットする
//------------------------------------------------------
void SkyDomeShader::Bind()
{
    ID3D11DeviceContext* ctx = GetContext();

    m_pVS->Bind();
    m_pPS->Bind();

    // b1: gUseTexture
    if (m_pSkyParamCB)
    {
        ID3D11Buffer* cb = m_pSkyParamCB;
        ctx->PSSetConstantBuffers(1, 1, &cb);
    }

    // ★ t0 へ テクスチャをセット
    if (m_pSkySRV)
    {
        ctx->PSSetShaderResources(0, 1, &m_pSkySRV);
    }

    // ★ s0 へ サンプラをセット
    if (m_pSampler)
    {
        ctx->PSSetSamplers(0, 1, &m_pSampler);
    }

}
