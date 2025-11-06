#pragma once
#include "World.h"
#include "GameObject.h"
#include "MeshRender.h"

namespace RenderSystem
{
    inline void Render(const World& w)
    {
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
            }
        }
    }
}
