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

    ID3D11Texture2D* backTex = nullptr;
    HRESULT hr = sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
    if (FAILED(hr) || !backTex) return;

    ID3D11RenderTargetView* rtv = sceneRT->GetView();
    if (!rtv)
    {
        backTex->Release();
        return;
    }

    ID3D11Resource* dstRes = nullptr;
    rtv->GetResource(&dstRes);
    if (!dstRes)
    {
        backTex->Release();
        return;
    }

    ID3D11Texture2D* dstTex = nullptr;
    hr = dstRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&dstTex);
    if (FAILED(hr) || !dstTex)
    {
        dstRes->Release();
        backTex->Release();
        return;
    }

    // MSAA check
    D3D11_TEXTURE2D_DESC srcDesc;
    D3D11_TEXTURE2D_DESC dstDesc;
    backTex->GetDesc(&srcDesc);
    dstTex->GetDesc(&dstDesc);

    // If sample count differs or src is MSAA, resolve is needed (src must be MSAA, dst must be non-MSAA)
    // We expect dst to be non-MSAA (SampleCount=1) because it will be sampled in the shader.
    if (srcDesc.SampleDesc.Count > 1 && dstDesc.SampleDesc.Count == 1)
    {
        // Resolve: MSAA -> non-MSAA
        ctx->ResolveSubresource(dstTex, 0, backTex, 0, dstDesc.Format);
    }
    else
    {
        // Normal copy (non-MSAA -> non-MSAA)
        ctx->CopyResource(dstTex, backTex);
    }

    dstTex->Release();
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
    w.DebugDrawColliders();
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
        // (1) 背景を sceneRT にコピー（屈折で読む元）
        CopyBackBufferToSceneRT(sceneRT);

        // (2) ガラスPSが読む SceneColor を t1 にセット
        ShaderList::SetGlassSceneTexture(sceneRT);

        // (3) 画面サイズ等（b4）
        const float sw = (float)bb->GetWidth();
        const float sh = (float)bb->GetHeight();
        ShaderList::SetGlassScreenParam(sw, sh, 50.0f, 0.0f);

        // (4) ガラスの見た目パラメータ（b3）
        // tint = ガラス色 + ベース透明度
        // fresnelPower = 縁の出方（大きいほど縁が強い）
        // reflectionStrength = 反射の強さ（0だと反射ゼロ）
        // transmissionStrength = 透過の強さ（あなたの簡易PSでは弱くしか使ってない）
        // v = tintStrength（色寄せ強さ）
        ShaderList::SetGlassParam(
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.60f), // ガラス色(白) + 透明度(0.0-1.0)
            3.0f,                                       // FresnelPower
            1.0f,                                       // ReflectionStrength ここが0だと反射消える
            1.0f,                                       // TransmissionStrength
            0.5f                                        // TintStrength
        );
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