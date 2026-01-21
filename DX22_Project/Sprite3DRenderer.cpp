#include "Sprite3DRenderer.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "DirectX.h"
#include <cassert>
using namespace DirectX;

void Sprite3DRenderer::Start()
{
    tf_ = gameObject->GetComponent<Transform>();
    cam_ = Camera::Main();
    texture_ = TextureManager::Get(texTag);

    assert(tf_ && cam_ && "Sprite3DRendererの参照がnullptr");
}

void Sprite3DRenderer::OnDestroy()
{
    tf_ = nullptr;
    cam_ = nullptr;
}

void Sprite3DRenderer::Render()
{
    // 0) VS / PS を取得
    VertexShader* vs = Sprite::Get3dVS();
    PixelShader* ps = Sprite::Get3dPS();
	if (!vs || !ps) return;

    assert(tf_ && "Transfromがnullptr");

    // 1) WVP を構築
    XMFLOAT4X4 fWVP[3];
    XMMATRIX W = XMLoadFloat4x4(&tf_->GetMatrix());
    XMMATRIX V = XMLoadFloat4x4(&Camera::Main()->GetView());
    XMMATRIX P = XMLoadFloat4x4(&Camera::Main()->GetProjection());
    XMStoreFloat4x4(&fWVP[0], XMMatrixTranspose(W));
    XMStoreFloat4x4(&fWVP[1], XMMatrixTranspose(V));
    XMStoreFloat4x4(&fWVP[2], XMMatrixTranspose(P));
    vs->WriteBuffer(0, &fWVP);

    // 2) SpriteParam を構築
    Sprite::ParamCB  param;
    param.offset_size = { offset.x, offset.y, size.x, size.y };
    param.uvPos_uvScale = { uvPos.x, uvPos.y ,uvScale.x, uvScale.y };
    param.color = color;
    vs->WriteBuffer(1, &param);
    ps->WriteBuffer(2, &param);

    // 3) テクスチャ・サンプラ・ステート
    ps->SetTexture(0, texture_);

	// 4) Sharderをバインド / 描画
    vs->Bind();
    ps->Bind();
	Sprite::Draw();
}
