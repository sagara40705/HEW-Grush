// Game.cpp
#include "Game.h"
#include "SceneManager.h"
#include "SceneGame.h"  
#include "SceneTitle.h"// 最初のシーン
#include "SceneSample.h"// 最初のシーン
#include "LoadingScene.h"
#include <memory>

void Game::Init()
{
    OriginalInit();

    SceneManager::Initialize();

    // ★修正: LoadingScene を直接作成してロード
    auto loadingScene = std::make_unique<LoadingScene>();

    // 次のシーン（SceneGame）を作成
    auto gameScene = std::make_unique<SceneGame>();

    // LoadingScene に次のシーンを設定
    loadingScene->SetNextScene(std::move(gameScene));

    // LoadingScene を手動でロード
    loadingScene->RootLoad();

    // SceneManager に登録（ChangeSceneAlreadyLoaded を使用）
    SceneManager::ChangeSceneAlreadyLoaded(std::move(loadingScene));
}

void Game::Uninit()
{
    SceneManager::Finalize();
    OriginalUninit();
}

void Game::Update()
{
    // 入力更新・サウンド更新などがあればここで

    SceneManager::Update();
}

void Game::Draw()
{
    // 描画開始 (Clear / BeginScene 等) が別にあるなら先に呼ぶ

    SceneManager::Draw();

    // 描画終了 (Present 等) が別にあるならここで呼ぶ
}
