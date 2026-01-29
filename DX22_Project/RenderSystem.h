#pragma once
#include "World.h"
#include "DirectX.h"
#include "Component.h"
#include "GameObject.h"
#include "LineManager.h"
#include "UiSpriteRenderer.h"
#include "Camera.h"

#include <algorithm>
#include <vector>
#include "Sprite3DRenderer.h" 
#include "MeshRenderer.h"
#include "ShaderList.h"

// scene color RT holder
inline RenderTarget*& GetSceneColorRTRef()
{
    static RenderTarget* s_sceneColorRT = nullptr;
    return s_sceneColorRT;
}

inline void SetSceneColorRT(RenderTarget* rt)
{
    GetSceneColorRTRef() = rt;
}

// backbuffer -> sceneRT copy
inline void CopyBackBufferToSceneRT(RenderTarget* sceneRT)
{
    if (!sceneRT) return;

    ID3D11DeviceContext* ctx = GetContext();
    IDXGISwapChain* sc = GetSwapChain();
    if (!ctx || !sc) return;

    // backbuffer resource
    ID3D11Texture2D* backTex = nullptr;
    HRESULT hr = sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
    if (FAILED(hr) || !backTex) return;

    // sceneRT must have SRV (Texture::GetResource())
    ID3D11ShaderResourceView* srv = sceneRT->GetResource();
    if (!srv)
    {
        backTex->Release();
        return;
    }

    // SRV -> underlying resource
    ID3D11Resource* dstRes = nullptr;
    srv->GetResource(&dstRes);
    if (!dstRes)
    {
        backTex->Release();
        return;
    }

    // copy: backbuffer -> sceneRT texture
    ctx->CopyResource(dstRes, backTex);

    dstRes->Release();
    backTex->Release();
}

inline void Render(const World& w)
{
    auto isPending = [&w](ObjectId id)
        {
            return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
        };

    SetBlendMode(BLEND_ALPHA);

    // --- 1) Opaque パス（ZWrite ON） ---
    SetDepthTest(true);
	SetCullingMode(D3D11_CULL_NONE);

    std::vector<Sprite3DRenderer*> transparentSprites;

    //=======================
    //追加
    //=======================
    std::vector<MeshRenderer*> glassMeshes; // add

    for (auto* r : w.GetRenderers())
    {
        if (!r) continue;
        if (dynamic_cast<UiSpriteRenderer*>(r)) continue;

        if (auto* spr = dynamic_cast<Sprite3DRenderer*>(r))
        {
            transparentSprites.push_back(spr);
            continue;
        }

        auto* c = dynamic_cast<Component*>(r);
        if (!c || !c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        // add: glass mesh is rendered later (needs sceneTex)
        if (auto* mr = dynamic_cast<MeshRenderer*>(r))
        {
            if (mr->psKind == ShaderList::PS_GLASS || mr->psKind == ShaderList::PS_GLASS_SHARD)
            {
                glassMeshes.push_back(mr);
                continue;
            }
        }

        r->Render();
    }

    // --- 2) デバッグ線など ---
    //w.DebugDrawColliders();
    //if(Camera::Main()) Camera::Main()->DebugDrawFrustum();
    //w.DebugDrawBounds();
    //LineManager::Draw(LineManager::LineGroup::General);

    // --- 3) Transparent パス（ZWrite OFF、奥→手前ソート推奨） ---
    SetDepthTest(true);
    // ソート（カメラから遠い順）
    //
    //Transform* camTf = nullptr;
    //if (auto* cam = Camera::Main()) camTf = cam->gameObject ? cam->gameObject->GetComponent<Transform>() : nullptr;
    //
    //if (camTf)
    //{
    //    const auto camPos = camTf->GetWorldPosition();
    //    std::sort(transparentSprites.begin(), transparentSprites.end(),
    //        [&](Sprite3DRenderer* a, Sprite3DRenderer* b)
    //        {
    //            auto* ta = a->gameObject->GetComponent<Transform>();
    //            auto* tb = b->gameObject->GetComponent<Transform>();
    //            const auto pa = ta ? ta->GetWorldPosition() : Vector3( 0,0,0 );
    //            const auto pb = tb ? tb->GetWorldPosition() : Vector3(0, 0, 0);
    //
    //            auto dxA = pa.x - camPos.x; auto dyA = pa.y - camPos.y; auto dzA = pa.z - camPos.z;
    //            auto dxB = pb.x - camPos.x; auto dyB = pb.y - camPos.y; auto dzB = pb.z - camPos.z;
    //            const float d2A = dxA * dxA + dyA * dyA + dzA * dzA;
    //            const float d2B = dxB * dxB + dyB * dyB + dzB * dzB;
    //            return d2A > d2B; // 遠い順
    //        });
    //}
    
    for (auto* spr : transparentSprites)
    {
        if (!spr) continue;
        if (!spr->IsVisible()) continue;

        auto* c = static_cast<Component*>(spr);
        if (!c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        spr->Render();
    }


    // add: copy backbuffer -> sceneRT for refraction sampling
    RenderTarget* sceneRT = GetSceneColorRTRef();
    RenderTarget* bb = GetDefaultRTV();
    if (sceneRT && bb)
    {
        CopyBackBufferToSceneRT(sceneRT);

        // set refraction inputs
        ShaderList::SetGlassSceneTexture(sceneRT);

        const float sw = (float)bb->GetWidth();
        const float sh = (float)bb->GetHeight();
        ShaderList::SetGlassScreenParam(sw, sh, 50.0f, 0.0f);
    }

    SetDepthTest(true); // これを追加

    // add: render glass after sceneRT is ready
    for (auto* mr : glassMeshes)
    {
        if (!mr) continue;

        auto* c = static_cast<Component*>(mr);
        if (!c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        mr->Render();
    }


    // --- 4) UI（Depth OFF） ---
    SetDepthTest(false);
	SetCullingMode(D3D11_CULL_NONE);
    for (auto* r : w.GetRenderers())
    {
        auto* ui = dynamic_cast<UiSpriteRenderer*>(r);
        if (!ui) continue;
        if (!ui->IsVisible()) continue;

        auto* c = static_cast<Component*>(ui);
        if (!c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        ui->Render();
    }


}

/*
inline void Render(const World& w)
{
    auto isPending = [&w](ObjectId id)
        {
            return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
        };

    // 1) 3D / 通常描画
    SetDepthTest(true);
    SetBlendMode(BLEND_ALPHA);
    SetCullingMode(D3D11_CULL_BACK);

    for (auto* r : w.GetRenderers())
    {
        if (!r) continue;
        if (dynamic_cast<UiSpriteRenderer*>(r) ) continue; // UI はここでは描かない

        auto* c = dynamic_cast<Component*>(r);
        if (!c || !c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        r->Render();
    }

    // 2) デバッグ線など
    w.DebugDrawColliders();
    //if(Camera::Main()) Camera::Main()->DebugDrawFrustum();
    //w.DebugDrawBounds();
    //LineManager::Draw(LineManager::LineGroup::General);

    // 3) UI のみ（深度 OFF）
    SetDepthTest(false);
    SetCullingMode(D3D11_CULL_NONE);
    for (auto* r : w.GetRenderers())
    {
        auto* ui = dynamic_cast<UiSpriteRenderer*>(r);
        if (!ui) continue;
        if (!ui->IsVisible()) continue;

        auto* c = static_cast<Component*>(ui);
        if (!c->IsEnabled()) continue;
        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        ui->Render();
    }

}
*/