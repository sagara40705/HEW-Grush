#include "AudioListener.h"
#include "SoundSystem.h"
#include "Transform.h"
#include "GameObject.h"

AudioListener* AudioListener::s_active = nullptr;

void AudioListener::OnEnable()
{
    // 後から有効化されたものを優先してリスナーにする
    s_active = this;
}

void AudioListener::OnDisable()
{
    if (s_active == this)
    {
        s_active = nullptr;
    }
}

void AudioListener::LateUpdate()
{
    if (s_active != this) return;
    if (!gameObject)       return;

    auto* t = gameObject->transform;
    if (!t) return;

    auto wp = t->GetWorldPosition();
    auto fwd = t->Forward();
    auto up = t->Up();

    SoundSystem::SetListenerTransform(
        Vector3(wp.x, wp.y, wp.z),
        Vector3(fwd.x, fwd.y, fwd.z),
        Vector3(up.x, up.y, up.z)
    );
}
