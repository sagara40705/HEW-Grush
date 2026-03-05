#include "ShaderList.h"

VertexShader* ShaderList::m_pVS[VS_KIND_MAX];
PixelShader* ShaderList::m_pPS[PS_KIND_MAX];

ShaderList::ShaderList()
{
}
ShaderList::~ShaderList()
{
}

void ShaderList::Init()
{
	MakePrimVS();
	MakeWorldVS();
	MakeAnimeVS();
	MakeSpriteVS();
	MakeShardVS();

	MakePrimPS();
	MakeLambertPS();
	MakeSpecularPS();
	MakeToonPS();
	MakeFogPS();
	MakeSpritePS();
	MakeGlassPS();
	MakeGlassTitlePS();
	MakeGlassShardPS();
	MakeRainbowPS();

	DirectX::XMFLOAT4X4 mat[250];
	for (int i = 0; i < 250; ++i)
	{
		DirectX::XMStoreFloat4x4(&mat[i], DirectX::XMMatrixIdentity());
	}
	SetWVP(mat);
	SetBones(mat);

	Model::Material material = {
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		nullptr
	};
	SetMaterial(material);
	SetLight(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f));
	SetCameraPos(DirectX::XMFLOAT3(0.0f, 1.0f, -2.0f));
	SetFog(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 50.0f);
}

void ShaderList::Uninit()
{
	for (int i = 0; i < VS_KIND_MAX; ++i)
	{
		if (m_pVS[i])
		{
			delete m_pVS[i];
			m_pVS[i] = nullptr;
		}
	}
	for (int i = 0; i < PS_KIND_MAX; ++i)
	{
		if (m_pPS[i])
		{
			delete m_pPS[i];
			m_pPS[i] = nullptr;
		}
	}
}

VertexShader* ShaderList::GetVS(VSKind vs)
{
	return m_pVS[vs];
}
PixelShader* ShaderList::GetPS(PSKind ps)
{
	return m_pPS[ps];
}

void ShaderList::SetWVP(DirectX::XMFLOAT4X4* wvp)
{
	for (int i = 0; i < VS_KIND_MAX; ++i)
	{
		m_pVS[i]->WriteBuffer(0, wvp);
	}
}
void ShaderList::SetBones(DirectX::XMFLOAT4X4* bones200)
{
	m_pVS[VS_ANIME]->WriteBuffer(1, bones200);
}
void ShaderList::SetMaterial(const Model::Material& material)
{
	DirectX::XMFLOAT4 param[3] = {
		material.diffuse,
		material.ambient,
		material.specular
	};
	param[1].w = material.pTexture ? 1.0f : 0.0f;
	for (int i = 0; i < PS_KIND_MAX; ++i)
	{
		m_pPS[i]->SetTexture(0, material.pTexture);
		m_pPS[i]->WriteBuffer(0, param);
	}
}
void ShaderList::SetLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 dir)
{
	DirectX::XMFLOAT4 param[2] = {
		color, {0,0,0,0}
	};
	DirectX::XMStoreFloat4(&param[1], DirectX::XMVector3Normalize(
		DirectX::XMVectorSet(dir.x, dir.y, dir.z, 0.0f)
	));
	for (int i = 0; i < PS_KIND_MAX; ++i)
	{
		m_pPS[i]->WriteBuffer(1, param);
	}
}
void ShaderList::SetCameraPos(const DirectX::XMFLOAT3 pos)
{
	DirectX::XMFLOAT4 param[] = {
		{pos.x, pos.y, pos.z, 0.0f}
	};
	m_pPS[PS_SPECULAR]->WriteBuffer(2, param);
	m_pPS[PS_TOON]->WriteBuffer(2, param);
	m_pPS[PS_GLASS]->WriteBuffer(2, param);
	m_pPS[PS_GLASS_SHARD]->WriteBuffer(2, param);
	m_pPS[PS_GLASS_TITLE]->WriteBuffer(2, param);
}
void ShaderList::SetFog(DirectX::XMFLOAT4 color, float start, float range)
{
	DirectX::XMFLOAT4 param[] = {
		color,
		{start, range, 0.0f, 0.0f}
	};
	m_pPS[PS_FOG]->WriteBuffer(3, param);
}
void ShaderList::SetGlassParam(const DirectX::XMFLOAT4& tint, float fresnelPower, float reflectionStrength, float transmissionStrength, float v)
{
	DirectX::XMFLOAT4 param[2];

	// tint
	param[0] = tint;  // tint = { r, g, b, a } (a にベースアルファ)

	// glassParams = { fresnelPower, reflectionStrength, transmissionStrength, 0 }
	param[1] = DirectX::XMFLOAT4(fresnelPower, reflectionStrength, transmissionStrength, v);

	// b3 に書き込む
	m_pPS[PS_GLASS]->WriteBuffer(3, param);
	m_pPS[PS_GLASS_SHARD]->WriteBuffer(3, param);
	m_pPS[PS_GLASS_TITLE]->WriteBuffer(3, param);
}

void ShaderList::SetGlassSceneTexture(RenderTarget* sceneColorRT)
{
	if (!sceneColorRT) return;

	// PS_GLASS, PS_GLASS_SHARD の t1 に SceneColor をセット
	m_pPS[PS_GLASS]->SetTexture(1, sceneColorRT);
	m_pPS[PS_GLASS_SHARD]->SetTexture(1, sceneColorRT);
}

void ShaderList::SetGlassScreenParam(float screenW, float screenH, float refractStrength, float chromaStrength)
{
	struct GlassScreenCB
	{
		DirectX::XMFLOAT2 screenSize;
		float refractStrength;
		float chromaStrength;
	};

	GlassScreenCB cb;
	cb.screenSize = DirectX::XMFLOAT2(screenW, screenH);
	cb.refractStrength = refractStrength;
	cb.chromaStrength = chromaStrength;

	// b4
	m_pPS[PS_GLASS]->WriteBuffer(4, &cb);
	m_pPS[PS_GLASS_SHARD]->WriteBuffer(4, &cb);
}

void ShaderList::MakePrimVS()
{
	const char* code = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	return vout;
})EOT";
	m_pVS[VS_PRIM] = new VertexShader();
	m_pVS[VS_PRIM]->Compile(code);
}
void ShaderList::MakeWorldVS()
{
	const char* code = R"EOT(
struct VS_IN {
	float3 pos : POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 wPos : POSITION0;
};
cbuffer WVP : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.normal = mul(vin.normal, (float3x3)world);
	vout.uv = vin.uv;
	vout.color = vin.color;
	return vout;
})EOT";
	m_pVS[VS_WORLD] = new VertexShader();
	m_pVS[VS_WORLD]->Compile(code);
}
void ShaderList::MakeAnimeVS()
{
	const char* code = R"EOT(
struct VS_IN {
	float3 pos : POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 wPos : POSITION0;
};
cbuffer WVP : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Bone : register(b1) {
	float4x4 bone[200];
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	float4x4 anime;
	anime  = bone[vin.index.x] * vin.weight.x;
	anime += bone[vin.index.y] * vin.weight.y;
	anime += bone[vin.index.z] * vin.weight.z;
	anime += bone[vin.index.w] * vin.weight.w;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, anime);
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.normal = vin.normal;
	vout.normal = mul(vout.normal, (float3x3)anime);
	vout.normal = mul(vout.normal, (float3x3)world);
	vout.uv = vin.uv;
	vout.color = vin.color;
	return vout;
})EOT";
	m_pVS[VS_ANIME] = new VertexShader();
	m_pVS[VS_ANIME]->Compile(code);
}
void ShaderList::MakeSpriteVS()
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

// 既存の SetWVP と同じ cbuffer(b0) を利用
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
    // world/view/proj だけ使う。残りの要素があっても無視される前提
};

// Sprite 専用パラメータ（b1）
cbuffer SpriteParam : register(b1)
{
    float4 offset_size;   // xy = offset(px), zw = size(px)
    float4 uvPos_uvScale; // xy = uvPos,    zw = uvScale
    float4 color;         // RGBA
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // 頂点位置（ローカル [-0.5,0.5] など）に size を掛けて offset を足す
    float2 p = vin.pos.xy * offset_size.zw + offset_size.xy;

    float4 pos = float4(p, 0.0f, 1.0f);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    vout.pos = pos;

    // UV は uvScale / uvPos で調整
    float2 uv = vin.uv * uvPos_uvScale.zw + uvPos_uvScale.xy;
    vout.uv = uv;

    vout.color = color;
    return vout;
}
)EOT";

	m_pVS[VS_SPRITE] = new VertexShader();
	m_pVS[VS_SPRITE]->Compile(code);
}
void ShaderList::MakeShardVS()
{
	const char* code = R"EOT(
struct VS_IN {
    float3 pos    : POSITION;   // 入力は通常 POSITION
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
};

struct VS_OUT {
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float4 color  : COLOR0;
    float4 wPos   : POSITION0;
};

// WVP（SetWVP から書き込まれる）
cbuffer Matrix : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// 破片ごとのランダムパラメータ
// randParam.xyz : スケール・回転用 (0-1)
// randParam.w   : ジャギー用シード
cbuffer ShardParam : register(b1)
{
    float4 randParam;
};

// 簡易ハッシュ（0-1）
float Hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // --- 形状いじり（ローカル XY 平面上） ---
    float2 p = vin.pos.xy;

    // 元メッシュ (-0.5,0), (0.5,0), (0,1) を想定して
    // おおよその重心付近を原点に移す
    float2 center = float2(0.0f, 1.0f / 3.0f);
    p -= center;

    // ランダムスケール（0.7 -- 1.3倍）
    float sx = lerp(0.7f, 1.3f, randParam.x);
    float sy = lerp(0.7f, 1.3f, randParam.y);
    p *= float2(sx, sy);

    // ランダム回転（約 -60 -- +60 度）
    float angle = (randParam.z * 2.0f - 1.0f) * (60.0f / 180.0f) * 3.14159265f;
    float s = sin(angle);
    float c = cos(angle);
    float2 r;
    r.x = p.x * c - p.y * s;
    r.y = p.x * s + p.y * c;
    p = r;

    // 先端を少しギザギザに（長さ方向にランダム伸ばす）
    float radial = length(p);
    float jaggy = 1.0f + (Hash(radial * 17.0f + randParam.w * 13.0f) - 0.5f) * 0.9f;
    p *= jaggy;

    // 中心を戻す
    p += center;

    // 厚み 0 のプレートとして Z は元の値を使用
    float3 localPos = float3(p, vin.pos.z);

    // --- ワールド・ビュー・プロジェクション ---
    float4 wp = mul(float4(localPos, 1.0f), world); // ワールド座標
    vout.wPos = wp;                                 // GlassPS が使う POSITION0

    float4 vp = mul(wp, view);
    vout.pos = mul(vp, proj);                       // クリップ座標

    // --- 法線・UV・カラー ---
    float3 nWorld = mul(vin.normal, (float3x3)world);
    vout.normal = normalize(nWorld);

    vout.uv = vin.uv;

    // 頂点カラーを使っていないので白固定
    vout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return vout;
}
)EOT";

	m_pVS[VS_SHARD] = new VertexShader();
	m_pVS[VS_SHARD]->Compile(code);
}

void ShaderList::MakePrimPS()
{
	const char* code = R"EOT(
struct PS_IN 
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};
float4 main(PS_IN pin) : SV_TARGET0 
{
	float4 color = float4(1,1,1,1);
	float2 halfGrid = floor(abs(pin.uv) * 2.0f);
	float2 quatGrid = floor(abs(pin.uv) * 8.0f);

	float half = fmod(halfGrid.x + halfGrid.y, 2.0f);
	float quat = fmod(quatGrid.x + quatGrid.y, 2.0f);

	color.rgb = ((half * 0.1f) * quat + 0.45f) + (1 - quat) * 0.05f;
	return color;
	//return float4(0.5f, 0.5f, 0.5f, 1.0f);
})EOT";
	m_pPS[PS_PRIM] = new PixelShader();
	m_pPS[PS_PRIM]->Compile(code);
}
void ShaderList::MakeLambertPS()
{
	const char* code = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};
cbuffer Light : register(b1)
{
	float4 lightDiffuse;
	float4 lightDir;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);

    // --- Albedo (Texture or White) ---
    bool hasTexture = (objAmbient.a >= 1.0f);
    float4 albedo = hasTexture ? tex.Sample(samp, pin.uv) : float4(1,1,1,1);

    // --- Lambert ---
    float ndl = saturate(dot(N, L));

    float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
    float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb * ndl;

    float3 rgb = albedo.rgb * (ambient + diffuse);

    // --- Specular (弱め) ---
    float gloss = max(0.01f, objSpecular.a);
    rgb += objSpecular.rgb * lightDiffuse.rgb * pow(ndl, gloss) * 0.15f;

    // --- Alpha (透明度) ---
    // 画像のα（あれば） × マテリアルのopacity（MakeMaterialで入れた diffuse.w）
    float a = albedo.a * saturate(objDiffuse.a);

    return float4(saturate(rgb), a);
})EOT";
	m_pPS[PS_LAMBERT] = new PixelShader();
	m_pPS[PS_LAMBERT]->Compile(code);
}
void ShaderList::MakeSpecularPS()
{
	const char* code = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 wPos : POSITION0;
};
cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};
cbuffer Light : register(b1)
{
	float4 lightDiffuse;
	float4 lightDir;
};
cbuffer Camera : register(b2)
{
	float4 cameraPos;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(objAmbient.a >= 1.0f)
		color = tex.Sample(samp, pin.uv);
	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir);
	float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);
	float3 R = reflect(-V, N);
	float dotNL = saturate((dot(N, L) + 0.5f) / 1.5f);
	float dotRL = saturate(dot(R, L));
	float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb;
	float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
	float3 specular = objSpecular.rgb * lightDiffuse.rgb;
	// Lambertの計算を参考
	color.rgb *= saturate(lerp(
		lerp(diffuse * ambient, diffuse + ambient, pow(ambient, 4.0f)),
		diffuse, dotNL));
	color.rgb += specular * saturate(pow(dotRL, max(0.01f, objSpecular.a)));
	return color;
})EOT";
	m_pPS[PS_SPECULAR] = new PixelShader();
	m_pPS[PS_SPECULAR]->Compile(code);
}
void ShaderList::MakeToonPS()
{
	const char* code = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};
cbuffer Light : register(b1)
{
	float4 lightDiffuse;
	float4 lightDir;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(objAmbient.a >= 1.0f)
		color = tex.Sample(samp, pin.uv);
	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir);
	float dotNL = dot(N, L); // マイナス込で計算
	float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb;
	float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
	float3 specular = objSpecular.rgb * lightDiffuse.rgb;
	float toonNL = saturate((dot(N, L) + 0.5f) / 1.5f * 100.0f); // 陰の境目を柔らかく
	// Lambertの計算を参考
	color.rgb *= saturate(lerp(
		lerp(diffuse * ambient, diffuse + ambient, pow(ambient, 4.0f)),
		diffuse, toonNL));
	if(objSpecular.a >= 1.0f)
		color.rgb += specular * saturate(pow(dotNL, max(0.01f, objSpecular.a)) * 100.0f);
	return color;
})EOT";
	m_pPS[PS_TOON] = new PixelShader();
	m_pPS[PS_TOON]->Compile(code);
}
void ShaderList::MakeFogPS()
{
	const char* code = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 wPos : POSITION0;
};
cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};
cbuffer Light : register(b1)
{
	float4 lightDiffuse;
	float4 lightDir;
};
cbuffer Camera : register(b2)
{
	float4 cameraPos;
};
cbuffer Param : register(b3)
{
	float4 fogColor;
	float fogStart;
	float fogRange;
	float2 dummy;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(objAmbient.a >= 1.0f)
		color = tex.Sample(samp, pin.uv);
	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir);
	float dotNL = saturate((dot(N, L) + 0.5f) / 1.5f);
	float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb;
	float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
	color.rgb *= saturate(diffuse * dotNL + ambient);
	float vLen = length(pin.wPos.xyz - cameraPos.xyz);
	color.rgb = lerp(color.rgb, fogColor.rgb, saturate((vLen - fogStart) / fogRange));
	return color;
})EOT";
	m_pPS[PS_FOG] = new PixelShader();
	m_pPS[PS_FOG]->Compile(code);
}
void ShaderList::MakeSpritePS()
{
	const char* code = R"EOT(
struct PS_IN {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};
cbuffer Material : register(b0)
{
    float4 dummy0;
};
Texture2D tex    : register(t0);
SamplerState samp: register(s0);
cbuffer Light : register(b1)
{
	float4 lightDiffuse;
	float4 lightDir;
};
// VS と同じ SpriteParam（b2）を定義（color を使う）
cbuffer SpriteParam : register(b2)
{
    float4 offset_size;   // 未使用だが VS とレイアウトを合わせておく
    float4 uvPos_uvScale;
    float4 color;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 texCol = tex.Sample(samp, pin.uv);
    // 単純なテクスチャ × 色
    return texCol * pin.color;
}
)EOT";

	m_pPS[PS_SPRITE] = new PixelShader();
	m_pPS[PS_SPRITE]->Compile(code);
}

void ShaderList::MakeGlassPS()
{
	const char* code = R"EOT(

////////////////////////////////////////////////////////////
// Glass Pixel Shader
// - テクスチャがある場合 : テクスチャを使ったガラス
// - テクスチャが無い場合 : ガラス色のみ（純ガラス）
// - 背景屈折 + Fresnel反射あり
//
// 前提（C++側）
//   objAmbient.a == 1.0 : テクスチャあり
//   objAmbient.a == 0.0 : テクスチャなし
//   ※ ShaderList::SetMaterial() が保証する
////////////////////////////////////////////////////////////

struct PS_IN
{
    float4 pos    : SV_POSITION; // 画面座標（ピクセル）
    float3 normal : NORMAL0;     // ワールド法線
    float2 uv     : TEXCOORD0;   // UV
    float4 color  : COLOR0;      // 未使用（将来用）
    float4 wPos   : POSITION0;   // ワールド座標
};

////////////////////////////////////////////////////////////
// b0 : Material
// objAmbient.a に「テクスチャ有無フラグ」が入る
////////////////////////////////////////////////////////////
cbuffer Material : register(b0)
{
    float4 objDiffuse;  // rgb=基本色, a=opacity
    float4 objAmbient;  // rgb=環境光, a=hasTextureFlag (1 or 0)
    float4 objSpecular; // rgb=鏡面色, a=gloss
};

////////////////////////////////////////////////////////////
// b1 : Light
////////////////////////////////////////////////////////////
cbuffer Light : register(b1)
{
    float4 lightDiffuse; // 光の色
    float4 lightDir;     // 光の方向（正規化済み）
};

////////////////////////////////////////////////////////////
// b2 : Camera
////////////////////////////////////////////////////////////
cbuffer Camera : register(b2)
{
    float4 cameraPos; // カメラのワールド座標
};

////////////////////////////////////////////////////////////
// b3 : GlassParam
// glassTint : ガラスの色 + ベースアルファ
// glassParams.x : Fresnel強度
// glassParams.y : 反射強度
// glassParams.w : Tint強度
////////////////////////////////////////////////////////////
cbuffer GlassParam : register(b3)
{
    float4 glassTint;
    float4 glassParams;
};

////////////////////////////////////////////////////////////
// b4 : GlassScreen
// 画面サイズと屈折量
////////////////////////////////////////////////////////////
cbuffer GlassScreen : register(b4)
{
    float2 screenSize;       // (width, height)
    float  refractStrength;  // 屈折の強さ（px感覚）
    float  chromaStrength;   // 色収差（今回は弱め）
};

////////////////////////////////////////////////////////////
// テクスチャ
////////////////////////////////////////////////////////////
Texture2D tex      : register(t0); // オブジェクトのテクスチャ（無い場合もある）
Texture2D sceneTex : register(t1); // 背景（SceneColor）
SamplerState samp  : register(s0);

////////////////////////////////////////////////////////////
// 簡易ハッシュ（低周波ノイズ用）
// モザイク感を防ぐために使う
////////////////////////////////////////////////////////////
float Hash21(float2 p)
{
    p = frac(p * float2(123.34f, 456.21f));
    p += dot(p, p + 34.345f);
    return frac(p.x * p.y);
}

////////////////////////////////////////////////////////////
// 背景サンプリング（色収差付き）
////////////////////////////////////////////////////////////
float3 SampleScene(float2 uv, float2 offset, float ca)
{
    float2 uvR = saturate(uv + offset * (1.0f + ca));
    float2 uvG = saturate(uv + offset);
    float2 uvB = saturate(uv + offset * (1.0f - ca));

    float r = sceneTex.Sample(samp, uvR).r;
    float g = sceneTex.Sample(samp, uvG).g;
    float b = sceneTex.Sample(samp, uvB).b;
    return float3(r, g, b);
}

////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////
float4 main(PS_IN pin) : SV_TARGET
{
    //////////////////////////////////////////////////////////
    // テクスチャ有無判定
    // objAmbient.a は C++ の SetMaterial() が保証する
    //////////////////////////////////////////////////////////
    float hasTex = (objAmbient.a >= 1.0f) ? 1.0f : 0.0f;

    //////////////////////////////////////////////////////////
    // テクスチャ取得
    // テクスチャ無しの場合は「白」として扱う
    //////////////////////////////////////////////////////////
    float4 texCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (hasTex > 0.5f)
    {
        texCol = tex.Sample(samp, pin.uv);
    }

    //////////////////////////////////////////////////////////
    // ベース色
    // ・テクスチャあり → texCol
    // ・テクスチャなし → glassTint（純ガラス）
    //////////////////////////////////////////////////////////
    float3 baseCol = lerp(glassTint.rgb, texCol.rgb, hasTex);

    //////////////////////////////////////////////////////////
    // 法線・視線
    //////////////////////////////////////////////////////////
    float3 N = normalize(pin.normal);
    float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

    //////////////////////////////////////////////////////////
    // Fresnel（縁を強くする）
    //////////////////////////////////////////////////////////
    float fresnelPower = max(glassParams.x, 1.0f);
    float fresnel = pow(1.0f - saturate(dot(N, V)), fresnelPower);

    //////////////////////////////////////////////////////////
    // スクリーンUV
    //////////////////////////////////////////////////////////
    float2 safeScreen = max(screenSize, float2(1.0f, 1.0f));
    float2 scrUV = saturate(pin.pos.xy / safeScreen);

    //////////////////////////////////////////////////////////
    // 屈折方向
    //////////////////////////////////////////////////////////
    float3 R = reflect(-V, N);
    float2 dir = normalize(R.xy + 1e-4f);

    //////////////////////////////////////////////////////////
    // ノイズ（モザイク防止）
    //////////////////////////////////////////////////////////
    float n = Hash21(scrUV * 60.0f + pin.wPos.xz * 0.02f);
    float2 noise = (n - 0.5f) * float2(1.0f, -1.0f);

    float2 offsetDir = dir * 0.85f + noise * 0.15f;

    //////////////////////////////////////////////////////////
    // 屈折量
    // ・中心は弱く
    // ・縁（Fresnel）は強く
    //////////////////////////////////////////////////////////
    float strength = refractStrength * (0.35f + 1.65f * fresnel);
    float2 offset = offsetDir * (strength / safeScreen);

    //////////////////////////////////////////////////////////
    // 背景取得
    //////////////////////////////////////////////////////////
    float ca = saturate(chromaStrength) * 0.01f;
    float3 sceneCol = SampleScene(scrUV, offset, ca);

    //////////////////////////////////////////////////////////
    // ガラス内部色（透過）
    //////////////////////////////////////////////////////////
    float tintStrength = saturate(glassParams.w);
    float3 tintedScene =
        lerp(sceneCol, sceneCol * baseCol, 0.35f + 0.55f * tintStrength);

    //////////////////////////////////////////////////////////
    // 透けすぎ防止（ガラスの厚み感）
    // 0.15～0.30 で調整
    //////////////////////////////////////////////////////////
    float solidity = 0.60f;
    tintedScene = lerp(tintedScene, baseCol, solidity);

    //////////////////////////////////////////////////////////
    // Fresnel反射
    //////////////////////////////////////////////////////////
    float reflStrength = saturate(glassParams.y);
	float reflectionScale = 10.0f; // ここを調整する
    float3 reflection =
        lightDiffuse.rgb * baseCol * fresnel * reflStrength * 1.2f;

    float3 finalRGB = tintedScene + reflection;

    //////////////////////////////////////////////////////////
    // わずかにオブジェクト色を戻す（存在感）
    //////////////////////////////////////////////////////////
    finalRGB = lerp(finalRGB, baseCol, 0.05f);

    //////////////////////////////////////////////////////////
    // Alpha
    // ・テクスチャ無し → 1.0 扱い
    // ・最低不透明度を保証
    //////////////////////////////////////////////////////////
    float alpha = glassTint.a * lerp(1.0f, texCol.a, hasTex);
    alpha = max(alpha, 0.60f);            // 透明すぎ防止
    alpha = saturate(alpha + fresnel * 0.2f); // 縁を少し濃く

    return float4(saturate(finalRGB), alpha);
}
)EOT";

	m_pPS[PS_GLASS] = new PixelShader();
	m_pPS[PS_GLASS]->Compile(code);
}

void ShaderList::MakeGlassTitlePS()
{
    const char* code = R"EOT(

struct PS_IN
{
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float4 color  : COLOR0;
    float4 wPos   : POSITION0;
};

cbuffer Material : register(b0)
{
    float4 objDiffuse;   // rgb=拡散色, a=opacity
    float4 objAmbient;   // rgb=環境光, a=hasTextureFlag(>=1でテクスチャ有り)
    float4 objSpecular;  // rgb=鏡面色, a=gloss
};

cbuffer Light : register(b1)
{
    float4 lightDiffuse;
    float4 lightDir;     // xyz
};

cbuffer Camera : register(b2)
{
    float4 cameraPos;
};

// ガラス固有（既存の SetGlassParam が b3 に書く想定）
cbuffer GlassParam : register(b3)
{
    float4 glassTint;    // rgb=色, a=透明度
    float4 glassParams;  // x=fresnelPower, y=reflectionStrength, z=transmissionStrength, w=tintStrength
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float3 N = normalize(pin.normal);
    float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);
    float3 L = normalize(-lightDir.xyz);

    // テクスチャ有無
    bool hasTexture = (objAmbient.a >= 1.0f);

    float4 albedo = float4(1,1,1,1);
    if (hasTexture)
    {
        albedo = tex.Sample(samp, pin.uv);
    }

    // ベース色（元の色を維持）
    float3 baseAlbedo = hasTexture ? albedo.rgb : objDiffuse.rgb;

    // 簡易ライティング（暗くしすぎない）
    float ndl = saturate(dot(N, L));
    float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
    float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb * ndl;
    float3 baseLit = baseAlbedo * (ambient + diffuse);

    // Fresnel（反射の縁を作る）
    float fresnelPower = max(glassParams.x, 1.0f);
    float dotNV = saturate(dot(N, V));
    float fresnel = pow(1.0f - dotNV, fresnelPower);

    // 反射を強める（ここが“反射のみ付けたい”の中心）
    float reflectionStrength = saturate(glassParams.y);

    // 反射最大量（上げるほど反射が強くなる）
    const float kReflMax = 0.55f;

    float refl = fresnel * reflectionStrength * kReflMax;

    // 簡易反射色（ライト色ベース）
    float3 reflectionColor = lightDiffuse.rgb;

    float3 finalRGB = baseLit + reflectionColor * refl;

    // 透明度（タイトルで破綻しにくくするため下限あり）
    float alpha = saturate(glassTint.a);
    alpha = max(alpha, 0.55f);

    // 縁を少し濃く
    alpha = saturate(alpha + fresnel * 0.15f);

    return float4(saturate(finalRGB), alpha);
}

)EOT";

    m_pPS[PS_GLASS_TITLE] = new PixelShader();
    m_pPS[PS_GLASS_TITLE]->Compile(code);

}

void ShaderList::MakeGlassShardPS()
{
	const char* code = R"EOT(
struct PS_IN {
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 wPos  : POSITION0;
};

cbuffer Material : register(b0)
{
	float4 objDiffuse;
	float4 objAmbient;
	float4 objSpecular;
};

cbuffer Light : register(b1)
{
    float4 lightDiffuse;
    float4 lightDir;
};

cbuffer Camera : register(b2)
{
    float4 cameraPos;
};

cbuffer GlassParam : register(b3)
{
    float4 glassTint;   // rgb = ガラス色, a = ベースアルファ
    float4 glassParams; // x=fresnelPower, y=reflectionStrength, z=transmissionStrength, w=tintStrength
};

cbuffer GlassScreen : register(b4)
{
    float2 screenSize;      // (width, height)
    float  refractStrength; // 屈折強度（ピクセル感覚でOK）
    float  chromaStrength;  // 色収差（0で無効）
};

Texture2D tex      : register(t0);
Texture2D sceneTex : register(t1);
SamplerState samp  : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float dummy = objDiffuse.x * 0.0;

    float4 texColor = tex.Sample(samp, pin.uv);

	

    float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);
    float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

    // ---- ベース（テクスチャ前提） ----
    float rawNL = dot(N, L);
    float dotNL = saturate(rawNL * 0.5f + 0.5f);
    const float kShadowMin = 0.7f;
    dotNL = lerp(kShadowMin, 1.0f, dotNL);

    float3 ambient = lightDiffuse.rgb * 0.3f;
    float3 diffuse = lightDiffuse.rgb;

    float3 base = texColor.rgb * (ambient + diffuse * dotNL);

    // ---- ガラス寄せ（PS_GLASS と同系統）----
    float fresnelPower         = max(glassParams.x, 1.0f);
    float reflectionStrength   = glassParams.y;
    float transmissionStrength = glassParams.z;
    float tintStrength         = glassParams.w;

    float dotNV   = saturate(dot(N, V));
    float fresnel = pow(1.0f - dotNV, fresnelPower);

    // tint（破片は色を出しやすく）
    const float kTintBase  = 0.25f;
    const float kTintExtra = 0.25f;
    float tintFactor = kTintBase + saturate(tintStrength) * kTintExtra; // 0.25 - 0.5

    // ---- 屈折（背景を歪ませて読む）----
    float2 safeScreen = max(screenSize, float2(1.0f, 1.0f));
    float2 screenUV = pin.pos.xy / safeScreen;

    // 画面端の破綻を減らすため少しクランプ（必要なら調整）
    screenUV = saturate(screenUV);

    float2 refractOffset = N.xy * (refractStrength / safeScreen);

    float3 refractedRGB;
    if (chromaStrength > 0.0f)
    {
        float2 ca = refractOffset * chromaStrength;
        float r = sceneTex.Sample(samp, screenUV + refractOffset + ca).r;
        float g = sceneTex.Sample(samp, screenUV + refractOffset).g;
        float b = sceneTex.Sample(samp, screenUV + refractOffset - ca).b;
        refractedRGB = float3(r, g, b);
    }
    else
    {
        refractedRGB = sceneTex.Sample(samp, screenUV + refractOffset).rgb;
    }

    // 透過の基礎を「屈折背景」に置き換える（ここがガラス感の本体）
    float3 transmit = lerp(refractedRGB, refractedRGB * glassTint.rgb, tintFactor);

    // ベースの明るさ（base）も少し混ぜる（破片の存在感を残す）
    transmit = lerp(transmit, base, 0.20f);

    // 透過強度（強くしすぎると“背景だけ”になるので注意）
    float transFactor = lerp(1.0f, 1.25f, saturate(transmissionStrength));
    transmit *= transFactor;

    // ---- 反射（Fresnel）----
    float upMask = saturate(N.y * 0.5f + 0.5f);
    float3 envUp   = float3(0.85f, 0.95f, 1.00f);
    float3 envDown = float3(0.05f, 0.06f, 0.08f);
    float3 reflectionColor = lerp(envDown, envUp, upMask);

    float reflFactor = saturate(fresnel * reflectionStrength);
    reflFactor = saturate(reflFactor * 1.35f);

    float3 finalRGB = lerp(transmit, reflectionColor, reflFactor);

    // ---- ハイライト（ガラスっぽく硬め）----
    float3 H = normalize(L + V);
    float spec = pow(saturate(dot(N, H)), 220.0f);
    float3 specular = lightDiffuse.rgb * spec * 0.9f;
    finalRGB += specular * (1.0f + 1.2f * fresnel);

    // ---- Alpha（中心薄く・縁濃く）----
    float baseAlpha = glassTint.a * texColor.a;

    const float kAlphaMin = 0.35f;   // 破片なので本体よりは薄め推奨（0.25-0.55で調整）
    baseAlpha = max(baseAlpha, kAlphaMin);

    float centerA = baseAlpha * 0.50f;
    float edgeA   = saturate(baseAlpha + 0.55f);
    float finalA  = lerp(centerA, edgeA, fresnel);

    return float4(saturate(finalRGB), finalA);
}
)EOT";

	m_pPS[PS_GLASS_SHARD] = new PixelShader();
	m_pPS[PS_GLASS_SHARD]->Compile(code);
}

void ShaderList::SetRainbowTime(float time)
{
	DirectX::XMFLOAT4 param;
	param.x = time;
	m_pPS[PS_RAINBOW]->WriteBuffer(2, &param);
}

void ShaderList::MakeRainbowPS()
{
	const char* code = R"EOT(
struct PS_IN {
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float4 color  : COLOR0;
};
cbuffer Material : register(b0)
{
    float4 objDiffuse;   // rgb=diffuse, a=opacity
    float4 objAmbient;   // rgb=ambient, a=hasTextureFlag(>=1)
    float4 objSpecular;  // rgb=specular, a=gloss
};
cbuffer Light : register(b1)
{
    float4 lightDiffuse; // 明るさ係数にだけ使う（不要なら未使用でもOK）
    float4 lightDir;     // 未使用
};
cbuffer Time : register(b2)
{
    float4 timeParam;    // x = timeSeconds
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
// 疑似乱数
float Hash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}
// HSV -> RGB (h:0-1)
float3 HSVtoRGB(float3 hsv)
{
    float3 p = abs(frac(hsv.x + float3(0, 2.0/3.0, 1.0/3.0)) * 6.0 - 3.0);
    float3 rgb = saturate(p - 1.0);
    rgb = rgb * rgb * (3.0 - 2.0 * rgb); // smooth
    return hsv.z * lerp(float3(1,1,1), rgb, hsv.y);
}
float4 main(PS_IN pin) : SV_TARGET
{
    float t = timeParam.x;

    // --- Albedo ---
    bool hasTexture = (objAmbient.a >= 1.0f);
    float4 albedo = hasTexture ? tex.Sample(samp, pin.uv) : float4(1,1,1,1);

    // =========================
    // Star / Rainbow (animated)
    // =========================

    // 虹帯：UV + 時間で流す
    float hue = frac(pin.uv.x * 1.2 + pin.uv.y * 0.8 + t * 5.0f);
    float3 rainbow = HSVtoRGB(float3(hue, 1.0, 1.0));

    // キラキラ：セル単位の乱数に「時間で移動する閾値」を掛けて点滅させる
    float2 cell = floor(pin.uv * 90.0);
    float n = Hash(cell);

    // 点滅速度・密度（調整用）
    float blink = frac(t * 3.0 + n);                 // 0..1
    float sparkle = smoothstep(0.92, 1.0, blink);    // たまに強く光る

	// ★虹の強さ（ここだけで調整）
	const float kRainbowTint = 0.25;   // 元画像を虹で薄く染める量（0.15～0.35目安）
	const float kGlow        = 0.18;   // 虹の発光（0.05～0.25目安）
	const float kSparkle     = 0.35;   // キラの強さ（0.2～0.6目安）

	// 元画像ベース
	float3 base = albedo.rgb;

	// 虹で薄くティント（上書きしない）
	float3 tinted = lerp(base, base * rainbow, kRainbowTint);

	// 発光とキラは控えめに加算
	float3 rgb = tinted + rainbow * kGlow + sparkle * rainbow * kSparkle;

    // 明るさ係数（使わなくてOK。使うなら全体の輝度調整に便利）
    rgb *= lerp(1.0, max(0.2, dot(lightDiffuse.rgb, float3(0.333,0.333,0.333))), 0.2);

    float a = albedo.a * saturate(objDiffuse.a);
    return float4(saturate(rgb), a);
}
)EOT";

	m_pPS[PS_RAINBOW] = new PixelShader();
	m_pPS[PS_RAINBOW]->Compile(code);
}
