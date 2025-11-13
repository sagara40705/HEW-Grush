#include "CanvasUI.h"
#include "Camera.h"
#include "Defines.h"
using namespace DirectX;

std::shared_ptr<VertexShader> CanvasUI::s_defVS;
std::shared_ptr<PixelShader>  CanvasUI::s_defPS;

CanvasUI::CanvasUI(const char* texPass, Vector2 pos, Vector2 size, Vector2 uvPos, Vector2 uvSize)
{
	m_data.texture = new Texture();
	m_data.texture->Create(texPass);
	SetOffset(pos);
	SetSize(size);
	SetUVPos(uvPos);
	SetUVScale(uvSize);
	m_data.param[2] = { 1, 1, 1, 1 };
}

void CanvasUI::Start()
{
	CreateMesh();
	ensureDefaultShaders();
	SetParam();

	m_data.vs = s_defVS.get();
	m_data.ps = s_defPS.get();
}

void CanvasUI::OnDestroy()
{
	delete m_data.texture; m_data.texture = nullptr;
	m_data.vs = nullptr;
	m_data.ps = nullptr;
}

void CanvasUI::Draw()
{
	if (!m_data.mesh || !m_data.vs || !m_data.ps || !m_data.texture) return;


	SetMatrix();
	m_data.vs->WriteBuffer(0, m_data.matrix);
	m_data.vs->WriteBuffer(1, m_data.param);
	m_data.vs->Bind();
	m_data.ps->SetTexture(0, m_data.texture);
	m_data.ps->Bind();
	m_data.mesh->Draw();
}

void CanvasUI::SetOffset(Vector2  offset)
{
	m_data.param[0].x = offset.x;
	m_data.param[0].y = offset.y;
}

void CanvasUI::SetSize(Vector2 size)
{
	m_data.param[0].z = size.x;
	m_data.param[0].w = size.y;
	SetParam();
}

void CanvasUI::SetUVPos(Vector2 pos)
{
	m_data.param[1].x = pos.x;
	m_data.param[1].y = pos.y;
}

void CanvasUI::SetUVScale(Vector2 scale)
{
	m_data.param[1].z = scale.x;
	m_data.param[1].w = scale.y;
}

void CanvasUI::SetColor(DirectX::XMFLOAT4 color)
{
	m_data.param[2] = color;
}

void CanvasUI::SetTexture(Texture* tex)
{
}

void CanvasUI::SetVertexShader(Shader* vs)
{

}

void CanvasUI::SetPixelShader(Shader* ps)
{
}

void CanvasUI::CreateMesh()
{
	struct Vertex
	{
		float pos[3];
		float uv[2];
	} vtx[] = {
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
	};

	// メッシュ
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = _countof(vtx);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_data.mesh = std::make_shared<MeshBuffer>();
	m_data.mesh->Create(desc);
}

void CanvasUI::ensureDefaultShaders()
{
	if (s_defVS && s_defPS) return;

	const char* VS = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Param : register(b1) {
	float2 offset;
	float2 size;
	float2 uvPos;
	float2 uvScale;
	float4 color;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos.xy *= size;
	vout.pos.xy += offset;
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.uv *= uvScale;
	vout.uv += uvPos;
	vout.color = color;
	return vout;
})EOT";
	const char* PS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
	return tex.Sample(samp, pin.uv) * pin.color;
})EOT";
	s_defVS = std::make_shared<VertexShader>(); s_defVS->Compile(VS);
	s_defPS = std::make_shared<PixelShader>();  s_defPS->Compile(PS);
}

void CanvasUI::SetMatrix()
{
	const float W = SCREEN_WIDTH;
	const float H = SCREEN_HEIGHT;
	auto I = XMMatrixIdentity();
	auto P = XMMatrixOrthographicOffCenterLH(0.f, W, H, 0.f, 0.f, 1.f);
	const XMFLOAT4X4 view = Camera::Main()->GetView();
	const XMFLOAT4X4 proj = Camera::Main()->GetProjection();

	XMStoreFloat4x4(&m_data.matrix[0], XMMatrixTranspose(I)); // world=I
	XMStoreFloat4x4(&m_data.matrix[1], XMMatrixTranspose(I)); // view=I
	XMStoreFloat4x4(&m_data.matrix[2], XMMatrixTranspose(P)); // proj=ortho
}

void CanvasUI::SetParam()
{
	// 元画像サイズ取得
	float texW = (float)m_data.texture->GetWidth();
	float texH = (float)m_data.texture->GetHeight();

	// param[0] = (offsetX, offsetY, width, height)
	m_data.param[0].z = texW * m_data.param[0].z;
	m_data.param[0].w = texW * m_data.param[0].w;
}
