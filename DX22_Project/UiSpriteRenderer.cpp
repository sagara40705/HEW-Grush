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
        if (!texture_) return;
    }

    // 1) VS/PS 取得（ここで1回だけ）
    auto* vs = ShaderList::GetVS(ShaderList::VS_SPRITE);
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!vs || !ps) return;

    // 2) WVP
    XMMATRIX W = XMMatrixTranspose(XMMatrixIdentity());
    XMMATRIX V = XMMatrixTranspose(XMMatrixIdentity());
    XMMATRIX P = XMMatrixTranspose(XMLoadFloat4x4(&canvas->GetProjMatrix()));

    struct WVP_CB
    {
        XMFLOAT4X4 world;
        XMFLOAT4X4 view;
        XMFLOAT4X4 proj;
    } wvp;

    XMStoreFloat4x4(&wvp.world, W);
    XMStoreFloat4x4(&wvp.view, V);
    XMStoreFloat4x4(&wvp.proj, P);

    // 3) SpriteParam
    Sprite::ParamCB param = GetSpriteParamCB();

    // 4) cbuffer 書き込み
    vs->WriteBuffer(0, &wvp);     // b0
    vs->WriteBuffer(1, &param);   // b1
    ps->WriteBuffer(1, &param);   // b1

    // 5) ★ BreakParam(b2) を書く（毎フレーム）
    struct CB_BreakParam2
    {
        float breakParam[4]; // x = breakT
    };

    CB_BreakParam2 cb{};
    cb.breakParam[0] = ShaderList::GetSpriteBreakT(); // 0～1

    ps->WriteBuffer(2, &cb);      // b2

    // 6) テクスチャ等
    ps->SetTexture(0, texture_);
    SetDepthTest(false);
    SetCullingMode(D3D11_CULL_NONE);

    // 7) Bind & Draw
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
