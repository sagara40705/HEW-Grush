#include "SoundSystem.h"
#include "SoundBackend.h"

//=========================================================
// 静的メンバ定義
//=========================================================

SoundHandle SoundSystem::s_bgmHandle = InvalidSoundHandle;
SoundCueId  SoundSystem::s_bgmCueId = InvalidSoundCueId;
float       SoundSystem::s_categoryVolumes[static_cast<int>(SoundCategory::Count)] = {};

//====================== ライフサイクル ======================

bool SoundSystem::Init()
{
    if (!SoundBackend::Init())
    {
        return false;
    }

    s_bgmHandle = InvalidSoundHandle;
    s_bgmCueId = InvalidSoundCueId;

    for (int i = 0; i < static_cast<int>(SoundCategory::Count); ++i)
    {
        s_categoryVolumes[i] = 1.0f;
        SoundBackend::SetBusVolume(static_cast<SoundCategory>(i), 1.0f);
    }

    return true;
}

void SoundSystem::Update()
{
    // 現状は Backend の Update を呼ぶだけ。
    // 必要に応じて BGM の手動フェードなどを追加しても良い。
    SoundBackend::Update();
}

void SoundSystem::Shutdown()
{
    SoundBackend::Shutdown();
}

//====================== プロジェクト / キューシート ======================

bool SoundSystem::LoadProject(const char* acfFile,
    const char* acbFile,
    const char* awbFile)
{
    return SoundBackend::LoadCueSheet(acfFile, acbFile, awbFile);
}

//====================== 再生系 ======================

SoundHandle SoundSystem::PlaySE(SoundCueId id)
{
    if (id == InvalidSoundCueId) return InvalidSoundHandle;
    return SoundBackend::PlayOneShot(id, SoundCategory::SE);
}

SoundHandle SoundSystem::PlaySE3D(SoundCueId id, const Vector3& pos)
{
    if (id == InvalidSoundCueId) return InvalidSoundHandle;

    SoundHandle h = SoundBackend::Play(id, SoundCategory::SE, false);
    if (h != InvalidSoundHandle)
    {
        SoundBackend::Set3DAttributes(h, pos, Vector3::zero());
    }
    return h;
}

void SoundSystem::PlayBGM(SoundCueId id, float fadeTime)
{
    if (id == InvalidSoundCueId) return;

    // 同じ BGM なら何もしない
    if (s_bgmCueId == id && s_bgmHandle != InvalidSoundHandle)
    {
        return;
    }

    // 既存 BGM をフェードアウトして停止
    if (s_bgmHandle != InvalidSoundHandle)
    {
        SoundBackend::Stop(s_bgmHandle, fadeTime);
        s_bgmHandle = InvalidSoundHandle;
        s_bgmCueId = InvalidSoundCueId;
    }

    // 新しい BGM を再生（ループ前提）
    s_bgmHandle = SoundBackend::Play(id, SoundCategory::BGM, true);
    s_bgmCueId = id;

    // カテゴリボリュームを反映（必要であれば BGM 個別ボリュームも）
    float bgmVol = s_categoryVolumes[static_cast<int>(SoundCategory::BGM)];
    if (s_bgmHandle != InvalidSoundHandle)
    {
        SoundBackend::SetVolume(s_bgmHandle, bgmVol);
    }
}

void SoundSystem::StopBGM(float fadeTime)
{
    if (s_bgmHandle == InvalidSoundHandle) return;

    SoundBackend::Stop(s_bgmHandle, fadeTime);
    s_bgmHandle = InvalidSoundHandle;
    s_bgmCueId = InvalidSoundCueId;
}

//====================== カテゴリ / ボリューム ======================

void SoundSystem::SetCategoryVolume(SoundCategory cat, float volume)
{
    int idx = static_cast<int>(cat);
    if (idx < 0 || idx >= static_cast<int>(SoundCategory::Count)) return;

    s_categoryVolumes[idx] = volume;
    SoundBackend::SetBusVolume(cat, volume);
}

float SoundSystem::GetCategoryVolume(SoundCategory cat)
{
    int idx = static_cast<int>(cat);
    if (idx < 0 || idx >= static_cast<int>(SoundCategory::Count)) return 0.0f;
    return s_categoryVolumes[idx];
}

//====================== リスナー ======================

void SoundSystem::SetListenerTransform(const Vector3& pos,
    const Vector3& forward,
    const Vector3& up)
{
    SoundBackend::SetListenerAttributes(pos, forward, up);
}
