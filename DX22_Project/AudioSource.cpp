#include "AudioSource.h"
#include "SoundBackend.h"
#include "Transform.h"
#include "GameObject.h"

void AudioSource::Awake()
{
    if (playOnAwake && cueId != InvalidSoundCueId)
    {
        Play();
    }
}

void AudioSource::OnDisable()
{
    // 無効化されたら停止（必要に応じて挙動は変えてよい）
    Stop();
}

void AudioSource::OnDestroy()
{
    // 破棄時に再生中なら止めておく
    Stop();
}

void AudioSource::Update()
{
    // 3D 音の位置更新
    if (!is3D) return;
    if (handle == InvalidSoundHandle) return;
    if (!gameObject) return;

    auto* t = gameObject->transform;
    if (!t) return;

    auto wp = t->GetWorldPosition();
    // 速度は現状 0 固定。必要になれば Rigidbody から取得する。
    SoundBackend::Set3DAttributes(
        handle,
        Vector3(wp.x, wp.y, wp.z),
        Vector3::zero()
    );
}

void AudioSource::Play()
{
    if (cueId == InvalidSoundCueId) return;

    // すでに再生中なら一度止める（Unity の Play() に近い挙動）
    if (handle != InvalidSoundHandle)
    {
        SoundBackend::Stop(handle, 0.0f);
        handle = InvalidSoundHandle;
    }

    if (is3D)
    {
        Vector3 pos = Vector3::zero();
        if (gameObject && gameObject->transform)
        {
            auto wp = gameObject->transform->GetWorldPosition();
            pos = Vector3(wp.x, wp.y, wp.z);
        }

        handle = SoundBackend::Play(cueId, category, loop);
        if (handle != InvalidSoundHandle)
        {
            SoundBackend::Set3DAttributes(handle, pos, Vector3::zero());
        }
    }
    else
    {
        handle = SoundBackend::Play(cueId, category, loop);
    }

    if (handle != InvalidSoundHandle)
    {
        SoundBackend::SetVolume(handle, volume);
        SoundBackend::SetPitch(handle, pitch);
    }
}

void AudioSource::Stop()
{
    if (handle == InvalidSoundHandle) return;

    // フェード時間はひとまず固定で 0.1。必要ならメンバ化してもよい。
    SoundBackend::Stop(handle, 0.1f);
    handle = InvalidSoundHandle;
}

bool AudioSource::IsPlaying() const
{
    if (handle == InvalidSoundHandle) return false;
    return SoundBackend::IsPlaying(handle);
}

void AudioSource::SetVolume(float v)
{
    volume = v;
    if (handle != InvalidSoundHandle)
    {
        SoundBackend::SetVolume(handle, volume);
    }
}

void AudioSource::SetPitch(float p)
{
    pitch = p;
    if (handle != InvalidSoundHandle)
    {
        SoundBackend::SetPitch(handle, pitch);
    }
}
