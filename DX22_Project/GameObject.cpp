#include "GameObject.h"
#include "World.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "DebugLog.h"

GameObject::~GameObject()
{
    if (!world) return;

    // World ‚ÌƒŠƒXƒg‚©‚ç“o˜^‰ğœ
    for (auto& kv : components)
    {
        Component* c = kv.second.get();
        if (!c) continue;

        if (auto* mb = dynamic_cast<MonoBehaviour*>(c))
            world->Unregister(mb);

        if (auto* fu = dynamic_cast<IFixedUpdate*>(c))
            world->Unregister(fu);

        if (auto* lu = dynamic_cast<ILateUpdate*>(c))
            world->Unregister(lu);
    }
}

void GameObject::Destroy(float /*time*/)
{
    if (!world)
    {
        Debug::Log("GameObject::Destroy called but world is null");
        return;
    }

    world->RequestDestroy(id);
    Debug::LogValue("GameObject::Destroy id", id);
}
