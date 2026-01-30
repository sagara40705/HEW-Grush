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