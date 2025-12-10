#pragma once
#include "World.h"
#include "DirectX.h"
#include "Component.h"
#include "GameObject.h"
#include "LineManager.h"
#include "UiSpriteRenderer.h"
#include "Camera.h"

inline void Render(const World& w)
{
    auto isPending = [&w](ObjectId id)
        {
            return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
        };

    // 1) 3D / 通常描画
    SetDepthTest(true);
    //SetCullingMode(D3D11_CULL_BACK);

    for (auto* r : w.GetRenderers())
    {
        if (!r) continue;
        if (dynamic_cast<UiSpriteRenderer*>(r)) continue; // UI はここでは描かない

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


/*
namespace RenderSystem
{

    inline void Render(const World& w)
    {
        SetDepthTest(true);
        // �j��\����X�L�b�v
        auto isPending = [&w](ObjectId id)
            {
                return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
            };

        for (auto* r : w.GetRenderers())
        {
            if (!r) continue;
            auto* c = dynamic_cast<Component*>(r);
            if (!c || !c->IsEnabled()) continue;

            GameObject* go = c->gameObject;
            if (!go || !go->active) continue;
            if (isPending(go->id))  continue;

            r->Render();
        }

        SetDepthTest(true);
        w.DebugDrawColliders();
        LineManager::Draw(LineManager::LineGroup::General);

        SetDepthTest(false);


    }
}
*/