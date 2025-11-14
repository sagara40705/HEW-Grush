#pragma once
#include "World.h"
#include "GameObject.h"
#include "MeshRender.h"
#include "ModelRenderer.h"
#include "LineManager.h"
#include "CanvasUI.h"
#include "Camera.h"
#include "DebugLog.h"

enum RenderMode
{
    Normal,
    Debug,
    Max
};

namespace RenderSystem
{
    static RenderMode s_mode = Normal;

    inline void Render(const World& w)
    {
        SetDepthTest(true);
        // ”j‰ó—\’è‚ðƒXƒLƒbƒv
        auto isPending = [&w](ObjectId id)
            {
                return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
            };

        for (auto& up : w.objects)
        {
            if (!up) continue;
            auto& o = *up;
            if (!o.active || isPending(o.id)) continue;

            for (auto& kv : o.components)
            {
                Component* c = kv.second.get();
                if (!c || !c->IsEnabled()) continue;

                if (auto* mr = dynamic_cast<MeshRender*>(c))
                {
                    mr->Draw();
                }
                if (auto* mr = dynamic_cast<ModelRenderer*>(c))
                {
                    mr->Draw();
                }
            }
        }

        Debug::LogValue("RednderSystem camera", (Camera::Main() == nullptr));
       
        LineManager::Draw();

        SetDepthTest(false);
        for (auto& up : w.objects)
        {
            if (!up) continue;
            auto& o = *up;
            if (!o.active || isPending(o.id)) continue;

            for (auto& kv : o.components)
            {
                Component* c = kv.second.get();
                if (!c || !c->IsEnabled()) continue;

                if (auto* mr = dynamic_cast<CanvasUI*>(c))
                {
                    mr->Draw();
                }
            }
        }

    }
    inline void SetMode(RenderMode mode) { s_mode = mode; }
}
