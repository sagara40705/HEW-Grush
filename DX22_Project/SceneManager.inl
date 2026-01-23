#pragma once

// 非同期ロード用の ChangeSceneAsync で LoadingScene を使う前提
// まだ LoadingScene を作っていない場合は、いったんこの include と
// ChangeSceneAsync の中身をコメントアウトしても良いです。
#include "LoadingScene.h"

//==================================================
// 同期: スタック全消去 + 新しいシーンに切り替え
//==================================================
template<class TScene, class... Args>
TScene* SceneManager::ChangeScene(Args&&... args)
{
    if (s_state != Idle) return nullptr;
    if (s_nextScene) return nullptr;

    static_assert(std::is_base_of<Scene, TScene>::value,
        "TScene must derive from Scene.");

    auto ptr = std::make_unique<TScene>(std::forward<Args>(args)...);
    TScene* raw = ptr.get();

    s_nextScene = std::move(ptr);
    s_pendingOp = PendingOp::Change;  // 次のフレームで ClearStack + RootLoad

    return raw;
}

//==================================================
// 同期: スタックの上に新しいシーンを積む
//==================================================
template<class TScene, class... Args>
TScene* SceneManager::PushScene(Args&&... args)
{
    if (s_state != Idle) return nullptr;
    if (s_nextScene) return nullptr;
    static_assert(std::is_base_of<Scene, TScene>::value,
        "TScene must derive from Scene.");

    auto ptr = std::make_unique<TScene>(std::forward<Args>(args)...);
    TScene* raw = ptr.get();

    s_nextScene = std::move(ptr);
    s_pendingOp = PendingOp::Push;    // 次のフレームで RootLoad + push_back

    return raw;
}

//==================================================
// 非同期ロード: LoadingScene を挟んで切り替え
//==================================================
// 使い方:
//   SceneManager::ChangeSceneAsync<SceneGame>();
//
// LoadingScene 側で
//   - SetNextScene で受け取ったシーンの BuildLoadTasks/StepLoad を回す
//   - ロード完了後に SceneManager::ChangeSceneAlreadyLoaded(...)
// のような流れを想定。
template<class TNextScene, class... Args>
void SceneManager::ChangeSceneAsync(Args&&... args)
{
    if (s_state != Idle) return;
	if (s_nextScene) return;
    
    static_assert(std::is_base_of<Scene, TNextScene>::value,
        "TNextScene must derive from Scene.");

    // 先に「次の本命シーン」を作成
    auto nextScene = std::make_unique<TNextScene>(std::forward<Args>(args)...);

    // ローディング画面用シーンを作成
    auto loadingScene = std::make_unique<LoadingScene>();
    loadingScene->SetNextScene(std::move(nextScene));

    // SceneManager 的には「ChangeScene(LoadingScene)」と同じ扱い
    s_nextScene = std::move(loadingScene);
    s_pendingOp = PendingOp::Change;
}
