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
	MakeGlassShardPS();

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
		color
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
// randParam.xyz : スケール・回転用 (0〜1)
// randParam.w   : ジャギー用シード
cbuffer ShardParam : register(b1)
{
    float4 randParam;
};

// 簡易ハッシュ（0〜1）
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

    // ランダムスケール（0.7〜1.3倍）
    float sx = lerp(0.7f, 1.3f, randParam.x);
    float sy = lerp(0.7f, 1.3f, randParam.y);
    p *= float2(sx, sy);

    // ランダム回転（約 -60〜+60 度）
    float angle = (randParam.z * 2.0f - 1.0f) * (60.0f / 180.0f) * 3.14159265f;
    float s = sin(angle);
    float c = cos(angle);
    float2 r;
    r.x = p.x * c - p.y * s;
    r.y = p.x * s + p.y * c;
    p = r;

    // 先端を少しギザギザに（長さ方向にランダム伸ばす）
    float radial = length(p);
    float jaggy = 1.0f + (Hash(radial * 17.0f + randParam.w * 13.0f) - 0.5f) * 0.4f;
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

Texture2D tex    : register(t0);
SamplerState samp: register(s0);

// VS と同じ SpriteParam（b1）を定義（color を使う）
cbuffer SpriteParam : register(b1)
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

// ガラス固有パラメータ
cbuffer GlassParam : register(b3)
{
    float4 glassTint;   // rgb = ガラス色, a = ベースアルファ
    // x = fresnelPower, y = reflectionStrength, z = transmissionStrength, w = tintStrength
    float4 glassParams;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	 //return float4(1,0,1,1);

    float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);
    float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

    // ==========================
    // 1) テクスチャ＋シンプル Lambert
    // ==========================
    float4 texColor = float4(1, 1, 1, 1);
	// objAmbient.a を「テクスチャあり／なし」のフラグとして利用
	bool hasTexture = (objAmbient.a >= 1.0f);
	if (hasTexture)
	{
		texColor = tex.Sample(samp, pin.uv);
	}

    float3 diffuseColor = objDiffuse.rgb * lightDiffuse.rgb;
    float3 ambientColor = objAmbient.rgb * lightDiffuse.rgb;
    float3 specColor    = objSpecular.rgb * lightDiffuse.rgb;

	//影の調整
	float rawNL = dot(N, L);
	float dotNL = saturate(rawNL * 0.5f + 0.5f);
	const float kShadowMin = 0.7f;   
	dotNL = lerp(kShadowMin, 1.0f, dotNL);

    // ベース色（元モデルの見た目にかなり近い形）
	float3 baseColor = (hasTexture || glassParams.w != 1) ? texColor.rgb : glassTint.rgb;
    float3 base = baseColor * (ambientColor + diffuseColor * dotNL);

    // 鏡面はかなり弱め
    float3 R = reflect(-L, N);
    float dotRV = saturate(dot(R, V));
    float gloss = max(0.01f, objSpecular.a);
    float3 specular = specColor * pow(dotRV, gloss * 4.0f);
    base += specular * 0.2f;   // ← ここで 0.2 とかなり弱めに

    // ==========================
    // 2) ガラス用パラメータ（弱く使う）
    // ==========================
    float fresnelPower         = max(glassParams.x, 1.0f);
    float reflectionStrength   = glassParams.y;
    float transmissionStrength = glassParams.z;
    float tintStrength         = glassParams.w;

    float dotNV   = saturate(dot(N, V));
    float fresnel = pow(1.0f - dotNV, fresnelPower);

	// ==========================
	// 3) ガラス色・反射成分（控えめ）
	// ==========================

	// ほんのり色を寄せたいので、最低でも少しは Tint を効かせる
	const float kTintBase  = 0.25f;      // 常に 15% くらいは色味を寄せる
	const float kTintExtra = 0.25f;      // tintStrength で増える分
	float tintFactor = kTintBase + saturate(tintStrength) * kTintExtra; // 最大 ~0.4

	// base → glassTint.rgb に寄せる（0.6 くらい glassTint 寄りの目標色）
	float3 targetTint = lerp(base, glassTint.rgb, 0.6f);

	// 最終的な着色（base から targetTint へ tintFactor 分だけ寄せる）
	float3 tintedBase = lerp(base, targetTint, tintFactor);

	// 透過パラメータは 10% だけ反映（ほぼ見た目は維持）
	float transFactor = lerp(1.0f, transmissionStrength, 0.1f);
	tintedBase *= transFactor;

	// 反射色もかなり弱く、Fresnel 依存で少しだけ足す
	float3 reflectionColor = lightDiffuse.rgb * glassTint.rgb;
	const float kReflMax = 0.2f;
	float reflFactor = kReflMax * reflectionStrength * fresnel;

	float3 finalRGB = tintedBase + reflectionColor * reflFactor;

    // ==========================
    // 4) アルファ（透明度も弱め）
    // ==========================
    float baseAlpha = glassTint.a;

    // Fresnel でほんの少しだけ縁を濃くするが、ほぼ一定
    float fresnelAlpha = lerp(0.9f, 1.0f, fresnel); // 0.9〜1.0 の範囲

    float finalA = saturate(baseAlpha * fresnelAlpha);

    return float4(finalRGB, finalA);
})EOT";
	m_pPS[PS_GLASS] = new PixelShader();
	m_pPS[PS_GLASS]->Compile(code);

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
    // x = fresnelPower, y = reflectionStrength, z = transmissionStrength, w = tintStrength
    float4 glassParams;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
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

    // ---- 弱い鏡面 ----
    float3 R = reflect(-L, N);
    float dotRV = saturate(dot(R, V));
    float3 specular = lightDiffuse.rgb * pow(dotRV, 64.0f) * 0.05f;
    base += specular;

    // ---- ガラス寄せ（PS_GLASS と同系統）----
    float fresnelPower         = max(glassParams.x, 1.0f);
    float reflectionStrength   = glassParams.y;
    float transmissionStrength = glassParams.z;
    float tintStrength         = glassParams.w;

    float dotNV   = saturate(dot(N, V));
    float fresnel = pow(1.0f - dotNV, fresnelPower);

    const float kTintBase  = 0.25f;
    const float kTintExtra = 0.25f;
    float tintFactor = kTintBase + saturate(tintStrength) * kTintExtra; // ~0.25-0.5

    float3 targetTint = lerp(base, glassTint.rgb, 0.6f);
    float3 tintedBase = lerp(base, targetTint, tintFactor);

    float transFactor = lerp(1.0f, transmissionStrength, 0.1f);
    tintedBase *= transFactor;

    float3 reflectionColor = lightDiffuse.rgb * glassTint.rgb;
    const float kReflMax = 0.2f;
    float reflFactor = kReflMax * reflectionStrength * fresnel;

    float3 finalRGB = tintedBase + reflectionColor * reflFactor;

    // ---- Alpha（テクスチャ前提：tex.a を活かす）----
    float baseAlpha = glassTint.a * texColor.a;
    float fresnelAlpha = lerp(0.9f, 1.0f, fresnel);
    float finalA = saturate(baseAlpha * fresnelAlpha);

    return float4(finalRGB, finalA);
}
)EOT";

	m_pPS[PS_GLASS_SHARD] = new PixelShader();
	m_pPS[PS_GLASS_SHARD]->Compile(code);
}
