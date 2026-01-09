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

	//壊れ追加
	MakeBreakUiPS();

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
	SetLight(DirectX::XMFLOAT4(1.f, 1.1f, 1.f, 1.f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f));
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
	if (ps == PS_SPRITE && m_spriteOverridePS)
	{
		return m_spriteOverridePS;
	}
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
		if (!m_pPS[i]) continue;  // ★ 追加

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
		if (!m_pPS[i]) continue;  // ★ 追加

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
	param[1] = DirectX::XMFLOAT4(fresnelPower, reflectionStrength, transmissionStrength, 0.0f);

	// b3 に書き込む
	m_pPS[PS_GLASS]->WriteBuffer(3, param);
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
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(objAmbient.a >= 1.0f)
		color = tex.Sample(samp, pin.uv);
	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir);
	float dotNL = saturate((dot(N, L) + 0.5f) / 1.5f);
	float3 diffuse = objDiffuse.rgb * lightDiffuse.rgb;
	float3 ambient = objAmbient.rgb * lightDiffuse.rgb;
	float3 specular = objSpecular.rgb * lightDiffuse.rgb;
	// 本来のLambert拡散反射（思った表現が出来なかったので採用せず
	// color.rgb *= saturate(diffuse * dotNL + ambient);
	// 環境光で拡散反射部分の色が変わらないようにlerp(環境光,diffuse,dotNL)で計算
	// 環境光が弱ければ黒(乗算)、強ければ白(加算)となるように、各計算を線形で補間
	diffuse *= color.rgb;
	color.rgb = saturate(lerp(
		lerp(diffuse * ambient, diffuse + ambient, pow(ambient, 4.0f)),
		diffuse, dotNL));
	// 本来なら必要ない鏡面反射、Lambert向けに若干だけ適用
	color.rgb += specular * pow(saturate(dotNL), max(0.01f, objSpecular.a) * 0.5f) * 0.5f;
	return color;
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

cbuffer SpriteParam : register(b1)
{
    float4 offset_size;
    float4 uvPos_uvScale;
    float4 color;
};

// ★ 追加：BreakUiRenderer が送る値（b2）
cbuffer BreakParam : register(b2)
{
    float breakT;     // 0 なら通常、>0 なら割れ中
    float3 pad;       // 16byte境界合わせ
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 texCol = tex.Sample(samp, pin.uv);
    float4 outCol = texCol * pin.color;

    // ★ 動作確認用：breakT が 0 より大きい時だけ赤
    if (breakT > 0.001f)
    {
        outCol = float4(1, 0, 0, 1);
    }

    return outCol;
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
//壊れシェーダー作成
//壊れシェーダー作成
void ShaderList::MakeBreakUiPS()
{
	const char* code = R"EOT(
struct PS_IN {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex0 : register(t0);
SamplerState samp : register(s0);

// UiSpriteRenderer が毎フレーム書いてくるので b1 は残しておく
cbuffer SpriteParam : register(b1)
{
    float4 offset_size;
    float4 uvPos_uvScale;
    float4 color;
};

// BreakUiRenderer が書く（b0）
cbuffer BreakParam : register(b0)
{
    float time;
    float3 pad;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 baseCol = tex0.Sample(samp, pin.uv) * pin.color;

    // time が 0 より大きい間は赤くする（動作確認用）
    if (time > 0.0f)
    {
        return float4(1,0,0,1);
    }

    return baseCol;
}
)EOT";

	m_pPS[PS_BREAKUI] = new PixelShader();
	m_pPS[PS_BREAKUI]->Compile(code);
}

PixelShader* ShaderList::m_spriteOverridePS = nullptr;

void ShaderList::SetSpritePsOverride(PixelShader* ps)
{
	m_spriteOverridePS = ps;
}
