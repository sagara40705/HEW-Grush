#include "UIFrostRenderer.h"

std::unique_ptr<VertexShader>  UIFrostRenderer::vs_ = nullptr;
std::unique_ptr<PixelShader>   UIFrostRenderer::ps_ = nullptr;
Texture* UIFrostRenderer::tex_ = nullptr;


void UIFrostRenderer::Start()
{
	if (!vs_) CreateVS();
	if (!ps_) CreatePS();
}

void UIFrostRenderer::OnDestroy()
{
	vs_.reset();
	ps_.reset();
}

void UIFrostRenderer::Render()
{
}

void UIFrostRenderer::CreateVS()
{
    const char* code = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;  // クリップ空間の -1..+1 を入れる
    float2 uv  : TEXCOORD0;
};
struct VS_OUT {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};
VS_OUT main(VS_IN vin)
{
    VS_OUT o;
    o.pos = float4(vin.pos.xy, 0.0f, 1.0f);
    o.uv  = vin.uv;
    return o;
}
)EOT";
	vs_ = std::make_unique<VertexShader>();
    vs_->Compile(code);
}

void UIFrostRenderer::CreatePS()
{
    const char* code = R"EOT(
struct PS_IN {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

cbuffer UIFrostParam : register(b0)
{
    float4 tint_opacity; // rgb=tint, a=opacity(0..1)
    float4 vignette;     // x=strength(0..1), y=radius(0..1), z=softness(0..1), w=unused
    float4 noise;        // x=strength(0..1), y=tileScale, z=speed, w=timeSec
    float4 chroma;       // x=amount(0..1), yzw unused
};

Texture2D noiseTex : register(t0);
SamplerState samp0 : register(s0); // エンジンの Sampler(slot0) を使用（今はWRAP想定）

float3 ApplyVignette(float3 col, float2 uv)
{
    float2 d = uv - 0.5f;
    float r = length(d) * 2.0f; // 0..~1.4
    float radius   = saturate(vignette.y);
    float softness = max(1e-3, vignette.z);
    float v = smoothstep(radius, radius + softness, r);      // 外側ほど1
    float k = 1.0f - v * saturate(vignette.x);               // 端ほど暗く
    return col * k;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float2 uv = pin.uv;

    // ---- 0) ベース色（暗幕＋色味） ----
    float opacity = saturate(tint_opacity.a);
    float3 base = tint_opacity.rgb;

    // ---- 1) 画像ノイズ（すりガラス粒状） ----
    // tileScale は 64〜512 くらいで調整
    float tileScale = max(1.0f, noise.y);
    float t = noise.w;

    // ゆっくり流す（speed=0なら固定）
    float2 nUV = uv * tileScale + float2(t * noise.z, t * noise.z * 0.37);

    // 0..1 -> -0.5..+0.5
    float n = noiseTex.Sample(samp0, nUV).r - 0.5f;
    base += n * noise.x;

    // ---- 2) 疑似色ズレ（背景をサンプルできないので “板感” だけ作る） ----
    float ca = saturate(chroma.x);
    base.r += ca * 0.04f;
    base.b -= ca * 0.04f;

    // ---- 3) ビネット ----
    base = ApplyVignette(base, uv);

    return float4(base, opacity);
}
)EOT";
	ps_ = std::make_unique<PixelShader>();
	ps_->Compile(code);
}
