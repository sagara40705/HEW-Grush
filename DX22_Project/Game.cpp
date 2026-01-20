// Game.cpp
#include "Game.h"
#include "SceneManager.h"
#include "SceneGame.h"  
#include "SceneTitle.h"// 最初のシーン
#include "SceneSample.h"// 最初のシーン
#include "SceneResult.h"
#include "LoadingScene.h"
#include <memory>

void Game::Init()
{
    OriginalInit();

    SceneManager::Initialize();

	SceneManager::ChangeScene<Result>();
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
