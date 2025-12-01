#pragma once
#include "World.h"
#include "DirectX.h"
#include "Component.h"
#include "GameObject.h"
#include "LineManager.h"


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