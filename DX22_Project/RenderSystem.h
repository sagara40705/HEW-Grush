#pragma once
#include "World.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "ModelComponent.h"
#include "ModelRenderer.h"
#include "LineManager.h"
#include "CanvasUI.h"
#include "Frustum.h"

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
        // 破壊予定をスキップ
        auto isPending = [&w](ObjectId id)
        {
            return std::find(w.toDestroy.begin(), w.toDestroy.end(), id) != w.toDestroy.end();
        };

        // === 追加: カメラがあるときだけフラスタムを計算 ===
        Frustum fr;
        Camera* cam = Camera::Main();
        if (cam)
        {
            auto vp = cam->GetViewProjection();
            fr.BuildFromMatrix(vp);
        }

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
                    auto* tf = o.TryGetComponent<Transform>();
                    auto* mc = o.TryGetComponent<MeshFilter>();
                    if (!(tf && mc)) return;

                    DirectX::XMFLOAT3 center = tf->GetWorldPosition();
                    float radius = mc->boundsRadius;

                    if (!fr.IntersectsSphere(center, radius)) continue;
                    mr->Draw();
                }
                if (auto* mr = dynamic_cast<ModelRenderer*>(c))
                {
                    auto* tf = o.TryGetComponent<Transform>();
                    auto* mc = o.TryGetComponent<ModelComponent>();
                    if (!(tf && mc)) return;

                    DirectX::XMFLOAT3 center = tf->GetWorldPosition();
                    float radius = mc->boundsRadius;

                    if (!fr.IntersectsSphere(center, radius)) continue;

                    mr->Draw();
                }
            }
        }
       
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
