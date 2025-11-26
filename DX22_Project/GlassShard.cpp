#include "GlassShard.h"
#include "Rigidbody.h"
#include "GameObject.h"

void GlassShard::Start()
{
    // Rigidbody �̎擾
    rb = gameObject->TryGetComponent<Rigidbody>();
}

void GlassShard::Update()
{
    const float dt = 1.f / 60.f;
    elapsed += dt;

    if (elapsed >= lifetime)
    {
        gameObject->Destroy();
    }
}
