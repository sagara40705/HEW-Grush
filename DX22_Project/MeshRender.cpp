#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "GameObject.h"
#include "DirectX.h"
#include "CameraShake.h"
//MeshRender.cpp
struct MatrixCB
{
	DirectX::XMMATRIX world{};
	DirectX::XMMATRIX view{};
	DirectX::XMMATRIX proj{};
};

void MeshRender::Start()
{
	t = gameObject->TryGetComponent<Transform>();
	c = Camera::Main();
	mf = gameObject->TryGetComponent<MeshFilter>();
	SetDefaultVS();
	if (PStype) SetWhitePS();
	else SetDefaultPS();
}

void MeshRender::Draw()
{
	if (!(t && c && mf && mf->GetMeshBuffer()))
	{
		return;
	}

	MatrixCB WVP;
	// === s—ñ‚ÌŽæ“¾ ===
	using namespace DirectX;
	XMFLOAT4X4 wMat = t->GetMatrix();
	CameraShake::Shake(gameObject, wMat);
	XMMATRIX world = XMLoadFloat4x4(&wMat);
	XMMATRIX view = XMLoadFloat4x4(&c->GetView());
	XMMATRIX proj = XMLoadFloat4x4(&c->GetProjection());
	// === “]’u ===
	WVP.world = XMMatrixTranspose(world);
	WVP.view = XMMatrixTranspose(view);
	WVP. proj = XMMatrixTranspose(proj);
	m_pVS->WriteBuffer(0, &WVP);
	m_pVS->Bind();
	m_pPS->Bind();

	mf->GetMeshBuffer()->Draw();
}

void MeshRender::OnDestroy()
{
	t = nullptr;
	c = nullptr;
	mf = nullptr;
	m_pVS.reset();
	m_pPS.reset();
}

void MeshRender::SetDefaultVS()
{
	const char* VSCode = R"EOT(
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

	m_pVS = std::make_shared<VertexShader>();
	m_pVS->Compile(VSCode);
}

void MeshRender::SetDefaultPS()
{
	const char* PSCode = R"EOT(
struct PS_IN 
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
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
	m_pPS = std::make_shared<PixelShader>();
	m_pPS->Compile(PSCode);
}

void MeshRender::SetWhitePS()
{
	const char* PSCode = R"EOT(
struct PS_IN 
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
float4 main(PS_IN pin) : SV_TARGET0 
{
	float4 color = float4(1,1,1,1);
	float2 halfGrid = floor(abs(pin.uv) * 2.0f);
	float2 quatGrid = floor(abs(pin.uv) * 8.0f);

	float half = fmod(halfGrid.x + halfGrid.y, 2.0f);
	float quat = fmod(quatGrid.x + quatGrid.y, 2.0f);

	color.rgb = ((half * 0.1f) * quat + 0.6f) + (1 - quat) * 0.05f;
	return color;
})EOT";
	m_pPS = std::make_shared<PixelShader>();
	m_pPS->Compile(PSCode);
}
