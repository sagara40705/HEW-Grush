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
	MakeWorldVS();
	MakeAnimeVS();
	MakeExplodeVS();

	MakeLambertPS();
	MakeSpecularPS();
	MakeToonPS();
	MakeFogPS();
	MakeGlassPS();

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
	SetLight(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f));
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
void ShaderList::SetExplodeParam(float amount, float time)
{
	DirectX::XMFLOAT4 param(amount, time, 0.0f, 0.0f);

	if (m_pVS[VS_EXPLODE])
	{
		m_pVS[VS_EXPLODE]->WriteBuffer(1, &param);
	}
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

void ShaderList::SetGlassParam(const DirectX::XMFLOAT4& tint, float fresnelPower, float reflectionStrength, float transmissionStrength)
{
	DirectX::XMFLOAT4 param[2];

	// tint
	param[0] = tint;  // tint = { r, g, b, a } (a にベースアルファ)

	// glassParams = { fresnelPower, reflectionStrength, transmissionStrength, 0 }
	param[1] = DirectX::XMFLOAT4(
		fresnelPower,
		reflectionStrength,
		transmissionStrength,
		0.0f);

	// b3 に書き込む
	m_pPS[PS_GLASS]->WriteBuffer(3, param);
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
void ShaderList::MakeExplodeVS()
{
	const char* code = R"EOT(

//======================================================================
// 頂点ベース爆散 Vertex Shader (VS_EXPLODE)
//  - グリッド破片
//  - 重力＋空気抵抗
//  - カメラに近いほど破片をさらに縮小（motion を上げても巨大化しづらくする）
//
// explodeParam.x : tNorm  … 破壊からの正規化時間 [0,1]
// explodeParam.y : motion … 飛び散り距離スケール（初速倍率）
//======================================================================

struct VS_IN
{
    float3 pos    : POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float4 color  : COLOR0;
};

struct VS_OUT
{
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float4 color  : COLOR0;
    float4 wPos   : POSITION0;
};

//------------------------------------------------------------------
// WVP
//------------------------------------------------------------------
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

//------------------------------------------------------------------
// 爆散パラメータ
//------------------------------------------------------------------
cbuffer ExplodeParam : register(b1)
{
    float4 explodeParam; // x=tNorm, y=motion, z,w unused
};

//------------------------------------------------------------------
// 調整用定数
//------------------------------------------------------------------

// 破片グリッドのセルサイズ（ローカル空間）
static const float3 CELL_SIZE      = float3(0.2f, 0.2f, 0.2f);

// 破片の基本縮小率（1.0 = 元サイズ）
static const float   SHRINK_BASE   = 0.3f;

// シミュレーション時間（秒）
static const float   MAX_SIM_TIME  = 0.7f;

// motion=1.0 のときの基本初速
static const float   BASE_SPEED    = 4.0f;

// 水平方向の空気抵抗
static const float   DRAG_COEFF    = 2.0f;

// 重力
static const float3  GRAVITY       = float3(0.0f, -9.8f * 2.f, 0.0f);

// 距離ベース縮小用のパラメータ（ビュー空間 Z を使う）
// depth <= NEAR_DIST  → DIST_SCALE_NEAR 倍
// depth >= FAR_DIST   → DIST_SCALE_FAR 倍
static const float   NEAR_DIST       = 1.5f;
static const float   FAR_DIST        = 10.0f;
static const float   DIST_SCALE_NEAR = 0.1f;  // 手前ほどさらに縮める
static const float   DIST_SCALE_FAR  = 1.0f;  // 遠くはそのまま

//------------------------------------------------------------------
// セル単位ランダム方向
//------------------------------------------------------------------
float3 HashDirCell(int3 cellId)
{
    float3 p = (float3)cellId;
    float n = dot(p, float3(12.9898f, 78.233f, 37.719f));

    float s = frac(sin(n)        * 43758.5453f);
    float t = frac(sin(n * 1.31f)* 12345.6789f);
    float u = frac(sin(n * 2.17f)* 98765.4321f);

    float3 v = float3(s, t, u) * 2.0f - 1.0f;

    float len2 = dot(v, v);
    if (len2 < 1e-4f)
    {
        v = float3(0.0f, 1.0f, 0.0f);
    }
    return normalize(v);
}

//------------------------------------------------------------------
// 重力＋空気抵抗オフセット
//------------------------------------------------------------------
float3 ComputeBallisticOffset(float3 dir, float tNorm, float motion)
{
    float time = saturate(tNorm) * MAX_SIM_TIME;

    float speed = BASE_SPEED * motion;
    float3 v0   = dir * speed;

    float drag       = DRAG_COEFF;
    float dragFactor = exp(-drag * time);

    float3 velHoriz0 = float3(v0.x, 0.0f, v0.z);
    float3 posHoriz  = velHoriz0 * (1.0f - dragFactor) / drag;

    float vY0  = v0.y;
    float posY = vY0 * time + 0.5f * GRAVITY.y * time * time;

    return float3(posHoriz.x, posY, posHoriz.z);
}

//------------------------------------------------------------------
// main
//------------------------------------------------------------------
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    float3 localPos = vin.pos;
    float3 nrm      = normalize(vin.normal);

    float tNorm  = saturate(explodeParam.x);
    float motion = explodeParam.y;

    //------------------------------------------------------------------
    // 1. グリッドで「破片セル」を定義
    //------------------------------------------------------------------
    int3  cellId      = int3(floor(localPos / CELL_SIZE));
    float3 cellCenter = (float3)cellId * CELL_SIZE + CELL_SIZE * 0.5f;

    float3 fromCenter = localPos - cellCenter;

    // 基本縮小を適用
    float3 shardLocal = cellCenter + fromCenter * SHRINK_BASE;

    //------------------------------------------------------------------
    // 2. 現在の（まだ飛ばす前の）破片位置から、ビュー空間距離を求めて距離スケール決定
    //------------------------------------------------------------------
    float4 wposPre = mul(float4(shardLocal, 1.0f), world);
    float4 vposPre = mul(wposPre, view);  // ビュー空間

    float depth = vposPre.z;              // LH系なら前方が +Z
    depth = max(depth, 0.001f);           // 負値・ゼロを防ぐ

    // depth を [NEAR_DIST, FAR_DIST] → [0,1] へ正規化
    float depth01 = (depth - NEAR_DIST) / (FAR_DIST - NEAR_DIST);
    depth01 = saturate(depth01);

    // 距離に応じた追加スケール（手前ほど小さく）
    float distScale = lerp(DIST_SCALE_NEAR, DIST_SCALE_FAR, depth01);

    // セル中心基準で再縮小
    float3 shardRel      = shardLocal - cellCenter;
    float3 shardLocalDS  = cellCenter + shardRel * distScale;

    //------------------------------------------------------------------
    // 3. 破片ごとの飛び散り方向
    //------------------------------------------------------------------
    float3 dirCell = HashDirCell(cellId);
    float3 dir     = dirCell; // 完全ランダム（法線バイアスなし）

    //------------------------------------------------------------------
    // 4. 重力＋空気抵抗オフセットを加算
    //------------------------------------------------------------------
    float3 offset    = ComputeBallisticOffset(dir, tNorm, motion);
    float3 displaced = shardLocalDS + offset;

    //------------------------------------------------------------------
    // 5. WVP 変換
    //------------------------------------------------------------------
    float4 wpos = mul(float4(displaced, 1.0f), world);
    vout.wPos   = wpos;

    float4 vpos = mul(wpos, view);
    vpos        = mul(vpos, proj);
    vout.pos    = vpos;

    float3x3 w3 = (float3x3)world;
    vout.normal = normalize(mul(nrm, w3));

    vout.uv     = vin.uv;
    vout.color  = vin.color;

    return vout;
}


)EOT";
	m_pVS[VS_EXPLODE] = new VertexShader();
	m_pVS[VS_EXPLODE]->Compile(code);
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
	 // 基本色とアルファを分離して扱う
    float3 baseColor = float3(1.0f, 1.0f, 1.0f);
    float  alpha     = 0.8f;

    // ★ テクスチャを使うマテリアルのときだけサンプル
    //   objAmbient.a は「テクスチャ有効フラグ」としてそのまま使用
    if (objAmbient.a >= 1.0f)
    {
        float4 texc = tex.Sample(samp, pin.uv);
        baseColor = texc.rgb;  // 拡散反射のベース色
        alpha     = texc.a;    // 透明 PNG のアルファを保持
    }

    // ランバート系ライティング
    float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);
    float  dotNL = saturate((dot(N, L) + 0.5f) / 1.5f);

    float3 diffuse  = objDiffuse.rgb * lightDiffuse.rgb;
    float3 ambient  = objAmbient.rgb * lightDiffuse.rgb;
    float3 specular = objSpecular.rgb * lightDiffuse.rgb;

    // 元の処理：diffuse にベースカラーを乗算
    diffuse *= baseColor;

    // 元コメント通りの処理を baseColor ベースで
    float3 litColor = saturate(
        lerp(
            lerp(diffuse * ambient,
                 diffuse + ambient,
                 pow(ambient, 4.0f)),
            diffuse,
            dotNL));

    // 鏡面成分（元の式を維持）
    litColor += specular
        * pow(saturate(dotNL), max(0.01f, objSpecular.a) * 0.5f)
        * 0.5f;

    // ★ アルファはテクスチャありなら tex.a, なければ 1.0
    return float4(litColor, alpha);
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
	float4 color = float4(1.0f, 1.0f, 1.0f, 0.5f);
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
    float4 glassTint;        // rgb = ガラスの色, a = ベースアルファ
    float4 glassParams;  
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);
    float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

	float dotNL = saturate(dot(N, L));
    float minNL = 0.3;
    dotNL = lerp(minNL, 1.0f, dotNL);

	float3 baseColor    = objDiffuse.rgb * lightDiffuse.rgb;
    float3 ambientColor = objAmbient.rgb * lightDiffuse.rgb;
    float3 specColor    = objSpecular.rgb * lightDiffuse.rgb;

	float4 texColor = float4(1,1,1,1);
    if (objAmbient.a >= 1.0f)
    {
        texColor = tex.Sample(samp, pin.uv);
    }

	float3 lambert = ambientColor + baseColor * dotNL;
    lambert *= texColor.rgb;

    float3 R = reflect(-L, N);
    float  dotRV = saturate(dot(R, V));
    float  gloss = max(0.01f, objSpecular.a);
    float3 specular = specColor * pow(dotRV, gloss * 4.0f) * 0.5f;

    float3 litColor = lambert + specular;

    float fresnelPower        = glassParams.x;
    float reflectionStrength  = glassParams.y;
    float transmissionStrength= glassParams.z;

	float dotNV   = saturate(dot(N, V));
    float fresnel = pow(1.0f - dotNV, fresnelPower);

    float3 reflectionColor = lightDiffuse.rgb * glassTint.rgb;
    float3 transmission    = litColor * glassTint.rgb;

    float3 finalRGB = lerp(
        transmission * transmissionStrength,
        reflectionColor,
        saturate(reflectionStrength * fresnel));

	float baseAlpha    = glassTint.a;
    float alphaFresnel = 0.3 + 0.7 * fresnel;
    float finalA = saturate(baseAlpha * alphaFresnel);

	return float4(finalRGB, finalA);

})EOT";
	m_pPS[PS_GLASS] = new PixelShader();
	m_pPS[PS_GLASS]->Compile(code);
}
