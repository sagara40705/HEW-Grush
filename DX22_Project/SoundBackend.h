#pragma once
#include "SoundType.h"
#include "Vector3.h"

//=========================================================
// SoundBackend
//   - 実サウンドミドルウェア(CRI ADX2 / FMOD / Wwise 等)に
//     直接アクセスする層。
//   - ゲーム側 / コンポーネント側からはこのヘッダだけ見える。
//   - 現在の cpp 実装は「ダミー実装」なので、実際に音を
//     出したいときは SoundBackend.cpp 内部をミドルウェア用に
//     差し替える想定。
//=========================================================
class SoundBackend
{
public:
    // 初期化・更新・終了
    static bool Init();
    static void Update();
    static void Shutdown();

    // キューシート / バンクロード
    // acf/acb/awb のパス等を渡す想定（ミドルウェア依存）。
    static bool LoadCueSheet(const char* acfFile,
        const char* acbFile,
        const char* awbFile);
    static void UnloadAll();

    //====================== 再生 ======================

    // ループ有無を指定して再生
    static SoundHandle Play(SoundCueId cueId, SoundCategory cat, bool loop);

    // ワンショット再生（内部的には Play(..., loop=false)）
    static SoundHandle PlayOneShot(SoundCueId cueId, SoundCategory cat);

    // 停止（fadeTime はミドルウェアに任せる。ダミー実装では無視）
    static void Stop(SoundHandle handle, float fadeTime = 0.0f);

    static bool IsPlaying(SoundHandle handle);

    //====================== パラメータ ======================

    static void SetVolume(SoundHandle handle, float vol);   // 0.0〜1.0
    static void SetPitch(SoundHandle handle, float pitch);  // 0.5〜2.0 など

    // 3D 音用：位置・速度
    static void Set3DAttributes(SoundHandle handle, const Vector3& pos, const Vector3& vel);

    // カテゴリ / バス単位の音量（ダッキングなどのベース）
    static void SetBusVolume(SoundCategory cat, float vol); // 0.0〜1.0

    // リスナー（カメラ）の姿勢
    static void SetListenerAttributes(const Vector3& pos,
        const Vector3& forward,
        const Vector3& up);
};
