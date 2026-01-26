#include "LoadingScene.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include <iostream>  // デバッグ用
#include "Camera.h"

LoadingScene::LoadingScene()
    : nextScene()
    , started(false)
    , progress(0.0f)
    , shouldChangeScene(false)
{
	
}

LoadingScene::~LoadingScene() = default;

void LoadingScene::SetNextScene(std::unique_ptr<Scene> next)
{
    nextScene = std::move(next);
    started = false;
    progress = 0.0f;
    shouldChangeScene = false;
}

// このシーン自身のロード
void LoadingScene::Load()
{
    //std::cout << "LoadingScene::Load() - START" << std::endl;
    
    OriginalLoad();
    
    //std::cout << "LoadingScene::Load() - Before World::Start()" << std::endl;
    GetWorld().Start();
   
}

// 毎フレーム呼ばれる
void LoadingScene::Update()
{
    // ★重要: シーン切り替えフラグが立っている場合は何もしない
    if (shouldChangeScene)
    {
        return;
    }

    if (!nextScene)
    {
        return;
    }

    if (!started)
    {
        started = true;
    }

    bool finished = false;

    // 1 フレームに TasksPerFrame 個だけロードタスクを進める
    for (int i = 0; i < TasksPerFrame && !finished; ++i)
    {
        finished = nextScene->StepLoad();
    }

    progress = nextScene->GetLoadProgress();

    if (finished)
    {
        shouldChangeScene = true;
    }

    if (shouldChangeScene && nextScene)
    {
        // unique_ptr から shared_ptr へ変換して渡す
        SceneManager::ChangeSceneAlreadyLoaded(std::move(nextScene));
        return;
    }
}

void LoadingScene::Draw()
{
	if (shouldChangeScene) return;
	if (!nextScene) return;

    GetWorld().Draw();
}

void LoadingScene::Unload()
{
    nextScene.reset();
}
