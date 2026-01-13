#include "SoundBackend.h"

#include <vector>
#include <algorithm>

//=========================================================
// ダミー実装について
// - 現在は「音は実際には鳴らない」簡易実装。
// - ゲームロジック側がハンドルの生存/停止をテストできる程度の
//   最低限の挙動のみを持つ。
// - 実際にミドルウェア(CRI など)と接続するときは、このファイルの
//   中身を丸ごと差し替える想定。
//=========================================================

namespace
{
    struct Voice
    {
        SoundHandle   handle = InvalidSoundHandle;
        SoundCueId    cueId = InvalidSoundCueId;
        SoundCategory category = SoundCategory::SE;
        bool          loop = false;
        bool          playing = false;

        float         volume = 1.0f;
        float         pitch = 1.0f;

        Vector3       pos = Vector3::zero();
        Vector3       vel = Vector3::zero();
    };

    bool g_initialized = false;
    SoundHandle g_nextHandle = 1;
    std::vector<Voice> g_voices;

    float g_busVolumes[static_cast<int>(SoundCategory::Count)] = {};

    Vector3 g_listenerPos = Vector3::zero();
    Vector3 g_listenerForward = Vector3(0, 0, 1);
    Vector3 g_listenerUp = Vector3(0, 1, 0);

    Voice* FindVoice(SoundHandle h)
    {
        if (h == InvalidSoundHandle) return nullptr;
        for (auto& v : g_voices)
        {
            if (v.handle == h) return &v;
        }
        return nullptr;
    }
}

//====================== 基本ライフサイクル ======================

bool SoundBackend::Init()
{
    if (g_initialized) return true;
    g_initialized = true;

    g_voices.clear();
    g_nextHandle = 1;

    for (int i = 0; i < static_cast<int>(SoundCategory::Count); ++i)
    {
        g_busVolumes[i] = 1.0f;
    }

    // TODO: 実際のミドルウェア初期化処理をここに追加
    // 例）CRI Atom: criAtom_Initialize(...);

    return true;
}

void SoundBackend::Update()
{
    if (!g_initialized) return;

    // TODO: 実際のミドルウェアの更新処理をここに追加
    // 例）CRI Atom: criAtom_ExecuteMain();
}

void SoundBackend::Shutdown()
{
    if (!g_initialized) return;

    // TODO: 実際のミドルウェア終了処理
    // 例）CRI Atom: criAtom_Finalize();

    g_voices.clear();
    g_initialized = false;
}

//====================== キューシート ======================

bool SoundBackend::LoadCueSheet(const char* acfFile,
    const char* acbFile,
    const char* awbFile)
{
    if (!g_initialized) return false;

    // TODO: ミドルウェア側のプロジェクト / キューシートロード処理
    // acfFile / acbFile / awbFile のパスをそのまま渡す形で問題ない。

    (void)acfFile;
    (void)acbFile;
    (void)awbFile;

    return true;
}

void SoundBackend::UnloadAll()
{
    if (!g_initialized) return;

    // TODO: キューシート / バンクの解放処理

    g_voices.clear();
}

//====================== 再生 ======================

SoundHandle SoundBackend::Play(SoundCueId cueId, SoundCategory cat, bool loop)
{
    if (!g_initialized) return InvalidSoundHandle;
    if (cueId == InvalidSoundCueId) return InvalidSoundHandle;

    Voice v;
    v.handle = g_nextHandle++;
    v.cueId = cueId;
    v.category = cat;
    v.loop = loop;
    v.playing = true;
    v.volume = 1.0f;
    v.pitch = 1.0f;
    v.pos = g_listenerPos;
    v.vel = Vector3::zero();

    g_voices.push_back(v);

    // TODO: 実際にはミドルウェアの「再生開始 API」を呼び、
    // そこから得たハンドルを v.handle に保存する。

    return v.handle;
}

SoundHandle SoundBackend::PlayOneShot(SoundCueId cueId, SoundCategory cat)
{
    return Play(cueId, cat, false);
}

void SoundBackend::Stop(SoundHandle handle, float /*fadeTime*/)
{
    if (!g_initialized) return;

    if (auto* v = FindVoice(handle))
    {
        v->playing = false;
        // TODO: ミドルウェア側の Stop API 呼び出し（fadeTime を渡す）
    }
}

bool SoundBackend::IsPlaying(SoundHandle handle)
{
    if (!g_initialized) return false;

    if (auto* v = FindVoice(handle))
    {
        return v->playing;
    }
    return false;
}

//====================== パラメータ ======================

void SoundBackend::SetVolume(SoundHandle handle, float vol)
{
    if (!g_initialized) return;

    if (auto* v = FindVoice(handle))
    {
        v->volume = vol;
        // TODO: ミドルウェアへボリューム設定
    }
}

void SoundBackend::SetPitch(SoundHandle handle, float pitch)
{
    if (!g_initialized) return;

    if (auto* v = FindVoice(handle))
    {
        v->pitch = pitch;
        // TODO: ミドルウェアへピッチ設定
    }
}

void SoundBackend::Set3DAttributes(SoundHandle handle, const Vector3& pos, const Vector3& vel)
{
    if (!g_initialized) return;

    if (auto* v = FindVoice(handle))
    {
        v->pos = pos;
        v->vel = vel;
        // TODO: ミドルウェアへ 3D 座標 / 速度を設定
    }
}

void SoundBackend::SetBusVolume(SoundCategory cat, float vol)
{
    if (!g_initialized) return;

    int idx = static_cast<int>(cat);
    if (idx < 0 || idx >= static_cast<int>(SoundCategory::Count)) return;

    g_busVolumes[idx] = vol;
    // TODO: ミドルウェアのバス / カテゴリ音量に反映
}

void SoundBackend::SetListenerAttributes(const Vector3& pos,
    const Vector3& forward,
    const Vector3& up)
{
    if (!g_initialized) return;

    g_listenerPos = pos;
    g_listenerForward = forward;
    g_listenerUp = up;

    // TODO: ミドルウェアの 3D リスナー属性に反映
}
