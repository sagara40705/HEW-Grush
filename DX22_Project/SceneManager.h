#pragma once
#include "Scene.h"
#include <vector>
#include <memory>
#include <type_traits>
#include "Time/Timer.h"
#include "Texture.h"

class SpriteAnimation;

// UnityEngine.SceneManagement.SceneManager に相当するクラス
class SceneManager
{
public:
    SceneManager() = delete;

    // 全シーンの初期化/破棄
    static void Initialize();
    static void Finalize();

    // 現在のアクティブシーン（スタックのトップ）を更新・描画
    static void Update();
    static void Draw();

    //===== シーン切り替え系（同期）====================================

    // スタックを全消去して、指定シーンだけに切り替える
    // 例: SceneManager::ChangeScene<SceneGame>();
    template<class TScene, class... Args>
    static TScene* ChangeScene(Args&&... args);

    // スタックの上に新しいシーンを積む（下のシーンは維持）
    // 例: SceneManager::PushScene<ScenePause>();
    template<class TScene, class... Args>
    static TScene* PushScene(Args&&... args);

    // 一番上のシーンを Pop（RootUnload してから削除）
    static void PopScene();

    //===== 非同期ロード用の入口 =======================================
    // LoadingScene を挟んで次のシーンに遷移したいときに使う想定。
    // ※ LoadingScene 側で std::function タスクを回す。
    template<class TNextScene, class... Args>
    static void ChangeSceneAsync(Args&&... args);

    // すでに StepLoad / BuildLoadTasks でロード済みのシーンをそのまま採用する
    static void ChangeSceneAlreadyLoaded(std::unique_ptr<Scene> next);

    // アクティブシーン（スタックのトップ）
    static Scene* GetActiveScene();
    static const Scene* GetActiveSceneConst();

private:
    enum class PendingOp
    {
        None,
		Change, // フェードインしてシーンを入れ替える
		LoadedChange, // すでにロード済みのシーンに切り替える(フェードなし)
        Push,   // スタックの上に積む
        Pop,    // スタックの上を 1 枚はがす
    };

    // シーンスタック
    static std::vector<std::unique_ptr<Scene>> s_scenes;

    // 次フレームに適用する予定のシーン（Change / Push 用）
    static std::unique_ptr<Scene> s_nextScene;

    // 次フレームに行う操作
    static PendingOp s_pendingOp;

    // s_pendingOp / s_nextScene の内容を反映する
    static void ApplyNextScene();

    // 全シーン Unload + 破棄
    static void ClearStack();

private:
	// シーン遷移時のフェード管理
    enum State
    {
		FadeOut,
		Idle,
		FadeIn,
    };
	static State s_state;   // 現在のフェード状態
	static Timer s_timer;   // フェードタイマー(FadeOut/in共通)
	static constexpr float FadeDuration = 48.f / 60.f * 3.0f; // フェード時間(秒)(FadeOut/in共通)(48フレーム * 2)

	static void StartFadeOut();
	static void StartFadeIn();

	static void UpdateIdle();
	static void UpdateFadeOut();
    static void UpdateFadeIn();

	static SpriteAnimation* s_fadeOutAnime;     // フェードアウト用アニメーション
	static SpriteAnimation* s_fadeInAnime; ;    // フェードイン用アニメーション
public:
    static void CreateFadeAnimations(Scene* scene);         // フェード用アニメーション作成
	static void ReleaseFadeAnimations();        // フェード用アニメーション解放
};

// テンプレート関数の実装
#include "SceneManager.inl"
