// Game.cpp
#include "Game.h"
#include "SceneManager.h"
#include "SceneGame.h"  
#include "SceneTitle.h"// 最初のシーン
#include "SceneSample.h"// 最初のシーン
#include "InputKeyboard.h"
#include "SceneResult.h"


void Game::Init()
{
  
    OriginalInit();

    SceneManager::Initialize();

    // 最初のシーンを予約
   // SceneManager::ChangeScene<SceneSample>();
  // SceneManager::ChangeSceneAsync<SceneTitle>();
    //SceneManager::ChangeScene<SceneSample>();
    SceneManager::ChangeSceneAsync<Result>();
   
   
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
