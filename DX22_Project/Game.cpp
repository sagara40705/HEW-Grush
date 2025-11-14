// Game.cpp
#include "Game.h"

// static メンバ定義
World Game::world{};
Scene* Game::s_currentScene = nullptr;
Scene* Game::s_nextScene = nullptr;

void Game::Init(Scene* firstScene)
{
    // World は一旦クリーンな状態にしておく
    world.ClearAll();

    // 現在シーンをセット
    s_currentScene = firstScene;
    s_nextScene = nullptr;
}

void Game::Uninit()
{
    // 現在シーンを破棄
    if (s_currentScene)
    {
        delete s_currentScene;
        s_currentScene = nullptr;
    }

    // 予約済みシーンが残っていたら破棄
    if (s_nextScene)
    {
        delete s_nextScene;
        s_nextScene = nullptr;
    }

    // World 上のオブジェクトもすべて削除
    world.ClearAll();
}

void Game::Update()
{
    if (s_currentScene)
    {
        // Scene 内部で World::Update() などを呼ぶ想定
        s_currentScene->RootUpdate();
    }

    // フレーム末尾でシーン切り替えを反映
    ApplySceneChange();
}

void Game::Draw()
{
    if (s_currentScene)
    {
        s_currentScene->RootDraw();
    }
}

void Game::RequestSceneChange(Scene* nextScene)
{
    // すでに同じポインタが予約されているなら何もしない
    if (nextScene == s_nextScene)
    {
        return;
    }

    // すでに別の予約があった場合は破棄（リーク防止）
    if (s_nextScene && s_nextScene != nextScene)
    {
        delete s_nextScene;
    }

    s_nextScene = nextScene;
}

void Game::ApplySceneChange()
{
    if (!s_nextScene)
    {
        return; // 予約なし
    }

    // まず World の中身をクリーンにする
    world.ClearAll();

    // 現在シーンを破棄
    if (s_currentScene)
    {
        delete s_currentScene;
    }

    // 新しいシーンに切り替え
    s_currentScene = s_nextScene;
    s_nextScene = nullptr;
}
