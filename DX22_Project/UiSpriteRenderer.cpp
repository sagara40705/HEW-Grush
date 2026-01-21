#include "UiSpriteRenderer.h"
#include "ShaderList.h"
#include "Canvas.h"
#include "DirectX.h"
#include <cassert>
using namespace DirectX;

void UiSpriteRenderer::Start()
{
    canvas = Canvas::GetMain();
    texture_ = TextureManager::Get(texTag);

}

void UiSpriteRenderer::OnDestroy()
{
    canvas = nullptr;
    texture_ = nullptr;
}

void UiSpriteRenderer::SetTexture(TextureTag tag)
{
    texTag = tag;
    texture_ = TextureManager::Get(texTag);
}

void UiSpriteRenderer::Render()
{ 
    //assert(canvas && texture && "UiSpriteRenderer::Render()でnullptr");
    // 0) ガード
    if (!IsVisible()) return;
    if (!canvas)
    {
        canvas = Canvas::GetMain();
        if (!canvas) return;
    }
    if (!texture_)
    {
        texture_ = TextureManager::Get(texTag);
        return;
    }

    // 1) WVP を構築
    XMMATRIX W = XMMatrixIdentity();
    XMMATRIX V = XMMatrixIdentity();
    XMMATRIX P = XMLoadFloat4x4(&canvas->GetProjMatrix());
    W = XMMatrixTranspose(W);
    V = XMMatrixTranspose(V);
    P = XMMatrixTranspose(P);

    struct WVP_CB
    {
        XMFLOAT4X4 world;
        XMFLOAT4X4 view;
        XMFLOAT4X4 proj;
    } wvp;

    XMStoreFloat4x4(&wvp.world, W);
    XMStoreFloat4x4(&wvp.view, V);
    XMStoreFloat4x4(&wvp.proj, P);

    // 2) SpriteParam を構築
    Sprite::ParamCB param = GetSpriteParamCB();
    
    // 3) VS / PS を ShaderList から取得
    auto* vs = ShaderList::GetVS(ShaderList::VS_SPRITE);
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!vs || !ps) return;

    // 4) cbuffer 書き込み
    // b1 : WVP
    vs->WriteBuffer(0, &wvp);
    // b1 : SpriteParam
    vs->WriteBuffer(1, &param);
    ps->WriteBuffer(1, &param);

    // 5) テクスチャ・サンプラ・ステート
    ps->SetTexture(0, texture_);

    // 6) シェーダを Bind して Quad 描画
    vs->Bind();
    ps->Bind();

    Sprite::Draw();
}

Sprite::ParamCB UiSpriteRenderer::GetSpriteParamCB()
{
    Sprite::ParamCB  param;

    param.offset_size = { offset.x, offset.y, size.x, size.y };
    param.uvPos_uvScale = { uvPos.x, uvPos.y ,uvScale.x, uvScale.y };
    param.color = color;

    return param;
}
