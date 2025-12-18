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
}

void Sprite::Create3dPS()
{
}
