#include "SceneManager.h"
#include "Scene.h"

// 静的メンバ定義
std::vector<std::unique_ptr<Scene>> SceneManager::s_scenes;
std::unique_ptr<Scene>              SceneManager::s_nextScene;
SceneManager::PendingOp             SceneManager::s_pendingOp = SceneManager::PendingOp::None;

//--------------------------------------------------
// 初期化 / 終了
//--------------------------------------------------
void SceneManager::Initialize()
{
    ClearStack();
    s_nextScene.reset();
    s_pendingOp = PendingOp::None;
}

void SceneManager::Finalize()
{
    ClearStack();
    s_nextScene.reset();
    s_pendingOp = PendingOp::None;
}

//--------------------------------------------------
// 毎フレーム更新 / 描画
//--------------------------------------------------
void SceneManager::Update()
{
    // まず前フレームで予約された操作を反映
    ApplyNextScene();

    // アクティブシーンを更新
    if (Scene* active = GetActiveScene())
    {
        active->RootUpdate();
    }

    // Update 中に新たに ChangeScene / PushScene / PopScene が呼ばれていたら、
    // その結果もこのフレームの終わりで反映しておく
    ApplyNextScene();
}

void SceneManager::Draw()
{
    if (Scene* active = GetActiveScene())
    {
        active->RootDraw();
    }
}

//--------------------------------------------------
// シーンスタック操作
//--------------------------------------------------
void SceneManager::PopScene()
{
    // いきなり消すと現在のフレーム中に使われている可能性があるので、
    // 次のフレーム頭で実際に Pop する。
    if (!s_scenes.empty())
    {
        s_pendingOp = PendingOp::Pop;
    }
}

void SceneManager::ChangeSceneAlreadyLoaded(std::unique_ptr<Scene> scene)
{
    ClearStack();                 // 既存シーンは RootUnload して破棄

    if (scene)
    {

        s_scenes.emplace_back(std::move(scene));
    }
}

Scene* SceneManager::GetActiveScene()
{
    if (s_scenes.empty()) return nullptr;
    return s_scenes.back().get();
}

const Scene* SceneManager::GetActiveSceneConst()
{
    if (s_scenes.empty()) return nullptr;
    return s_scenes.back().get();
}

//--------------------------------------------------
// 内部ヘルパ
//--------------------------------------------------
void SceneManager::ApplyNextScene()
{
    switch (s_pendingOp)
    {
    case PendingOp::None:
        // 何もしない
        break;

    case PendingOp::Change:
        // 既存スタックを全て破棄して、新しいシーンだけ積む
        ClearStack();
        if (s_nextScene)
        {
            s_nextScene->RootLoad();                // World.Load() + Start() まで
            s_scenes.emplace_back(std::move(s_nextScene));
        }
        break;

    case PendingOp::Push:
        // 上に新しいシーンを積む
        if (s_nextScene)
        {
            s_nextScene->RootLoad();
            s_scenes.emplace_back(std::move(s_nextScene));
        }
        break;

    case PendingOp::Pop:
        // 一番上のシーンだけを削除
        if (!s_scenes.empty())
        {
            s_scenes.back()->RootUnload();          // Unload() + World.ClearAll()
            s_scenes.pop_back();
        }
        break;
    }

    // 操作が終わったのでクリア
    s_pendingOp = PendingOp::None;
    // Change / Push のときに使い終わった nextScene を念のためクリア
    if (s_pendingOp != PendingOp::Change && s_pendingOp != PendingOp::Push)
    {
        s_nextScene.reset();
    }
}

void SceneManager::ClearStack()
{
    // 後ろから順に Unload + 破棄
    while (!s_scenes.empty())
    {
        s_scenes.back()->RootUnload();
        s_scenes.pop_back();
    }
}