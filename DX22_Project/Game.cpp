// Game.cpp
#include "Game.h"
#include "SceneManager.h"
#include "SceneGame.h"   // 最初のシーン

bool Game::hasPlayedIntroMovie;

void Game::Init()
{
    hasPlayedIntroMovie = false;


    SceneManager::Initialize();

    // 最初のシーンを予約
    SceneManager::ChangeSceneAsync<SceneGame>();
}

void Game::Uninit()
{
    SceneManager::Finalize();
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
