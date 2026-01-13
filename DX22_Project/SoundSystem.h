#pragma once
#include "SoundType.h"
#include "Vector3.h"

//=========================================================
// SoundSystem
//   - エンジン全体のサウンド管理クラス。
//   - Game / Engine の Init/Update/Shutdown から呼び出される想定。
//   - 実際のサウンド I/O は SoundBackend に委譲する。
//=========================================================
class SoundSystem
{
public:
    // 初期化 / フレーム更新 / 終了
    static bool Init();
    static void Update();       // 毎フレーム一度呼ぶ
    static void Shutdown();

    // プロジェクト / キューシートのロード
    // （引数は実際のミドルウェア構成に合わせて増減させても良い）
    static bool LoadProject(const char* acfFile,
        const char* acbFile,
        const char* awbFile);

    //====================== 再生系 高レベル API ======================

    // 効果音（2D）
    static SoundHandle PlaySE(SoundCueId id);

    // 効果音（3D）: pos はワールド座標
    static SoundHandle PlaySE3D(SoundCueId id, const Vector3& pos);

    // BGM 再生（ループ前提）
    // 既に BGM が再生中なら、fadeTime 秒かけて停止 → 新しい BGM 再生。
    // ※ 実フェードはミドルウェア側の fadeTime 依存。
    static void PlayBGM(SoundCueId id, float fadeTime = 0.5f);

    // BGM 停止
    static void StopBGM(float fadeTime = 0.5f);

    //====================== カテゴリ / ボリューム ======================

    static void SetCategoryVolume(SoundCategory cat, float volume); // 0.0〜1.0
    static float GetCategoryVolume(SoundCategory cat);

    //====================== リスナー（カメラ） ======================

    static void SetListenerTransform(const Vector3& pos,
        const Vector3& forward,
        const Vector3& up);

private:
    // BGM の状態
    static SoundHandle s_bgmHandle;
    static SoundCueId  s_bgmCueId;

    static float s_categoryVolumes[static_cast<int>(SoundCategory::Count)];
};

