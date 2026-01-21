#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"
#include <cassert>

// 静的メンバ定義
std::vector<std::unique_ptr<Scene>> SceneManager::s_scenes;
std::unique_ptr<Scene>              SceneManager::s_nextScene;
SceneManager::PendingOp             SceneManager::s_pendingOp = SceneManager::PendingOp::None;

SceneManager::State 			    SceneManager::s_state = SceneManager::State::Idle;
Timer SceneManager::s_timer;
SpriteAnimation* SceneManager::s_fadeOutAnime = nullptr;
SpriteAnimation* SceneManager::s_fadeInAnime = nullptr;


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
    // アクティブシーンを更新
    if (Scene* active = GetActiveScene())
    {
        active->RootUpdate();
    }

	// フェード状態に応じた更新
    switch (s_state)
    {
    case SceneManager::FadeOut: UpdateFadeOut();
        break;
	case SceneManager::Idle:    UpdateIdle();
        break;
	case SceneManager::FadeIn:  UpdateFadeIn();
        break;
    default:
        break;
    }
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

    assert(scene && "SceneManager::ChangeSceneAlreadyLoaded() scene is null.");
    if (scene)
    {
        s_scenes.emplace_back(std::move(scene));
        //s_state = State::FadeIn;
        //s_timer.Start();
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
		// FadeIn 開始
		StartFadeIn();
  
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

    // Push のときに使い終わった nextScene を念のためクリア
	if (s_pendingOp == PendingOp::Push)
	{
		s_nextScene.reset();
	}

    // 操作が終わったのでクリア
    s_pendingOp = PendingOp::None;
    //// Change / Push のときに使い終わった nextScene を念のためクリア
    //if (s_pendingOp != PendingOp::Change && s_pendingOp != PendingOp::Push)
    //{
    //    s_nextScene.reset();
    //}
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

void SceneManager::StartFadeOut()
{
	Scene* active = GetActiveScene();
	assert(active && "SceneManager::StartFadeOut() active scene is null.");

	s_state = State::FadeOut;
	s_timer.Start();

	assert(s_fadeOutAnime && "SceneManager::StartFadeOut() fadeOutAnime is null.");
    s_fadeOutAnime->Play();

}

void SceneManager::StartFadeIn()
{
	Scene* active = GetActiveScene();

	// active が null の場合はフェードインアニメーションを作成するシーンが無いため, そのままロードシーンへ遷移する
	if (!active)
	{
        //ロードsceneへ遷移する
        ClearStack();
        if (s_nextScene)
        {
            s_nextScene->RootLoad();
            s_scenes.emplace_back(std::move(s_nextScene));
        }
        s_state = State::Idle;
		return;
	}

	s_state = State::FadeIn;
	s_timer.Start();

	if (s_fadeInAnime) s_fadeInAnime->Play();
}

void SceneManager::UpdateIdle()
{
    // まず前フレームで予約された操作を反映
    //ApplyNextScene();

    // Update 中に新たに ChangeScene / PushScene / PopScene が呼ばれていたら、
    // その結果もこのフレームの終わりで反映しておく
    ApplyNextScene();
}

void SceneManager::UpdateFadeOut()
{
	assert(s_state == State::FadeOut && " SceneManager::UpdateFadeOut()");

	if (!s_timer.IsRunning()) assert(false && "FadeOut timer is not running.");

	if (s_timer.ElapsedSeconds() >= FadeDuration)
	{
		// フェードアウト完了
		s_timer.Stop();
        s_state = State::Idle;
	}
}

void SceneManager::UpdateFadeIn()
{
	assert(s_state == State::FadeIn && " SceneManager::UpdateFadeIn()");

	if (!s_timer.IsRunning()) assert(false && "FadeIn timer is not running.");

	if (s_timer.ElapsedSeconds() >= FadeDuration)
	{
		// フェードイン完了
		s_timer.Stop();

        //ロードsceneへ遷移する
        ClearStack();
        if (s_nextScene)
        {
            s_nextScene->RootLoad();
            s_scenes.emplace_back(std::move(s_nextScene));
        }

        s_state = State::Idle;
	}   
}

void SceneManager::CreateFadeAnimations(Scene* scene)
{
    TextureManager::Load(TextureTag::FadeOut);
    TextureManager::Load(TextureTag::FadeIn);

	World& world = scene->GetWorld();

 //   GameObject* fadeOutObj = &world.CreateGameObject();
 //   fadeOutObj->AddComponent<UiSpriteRenderer>(TextureTag::FadeOut, Vector2::zero(), Vector2(128.f, 72.f));
 //   s_fadeOutAnime = &fadeOutObj->AddComponent<SpriteAnimation>(6, 8, 1, 48, false);
	//fadeOutObj->SetActive(false); // 最初は非表示にしておく

    GameObject* fadeInObj = &world.CreateGameObject();
    fadeInObj->AddComponent<UiSpriteRenderer>(TextureTag::FadeIn, Vector2::zero(), Vector2(1280.f, 720.f));
    s_fadeInAnime = &fadeInObj->AddComponent<SpriteAnimation>(6, 8, 3, 48, false);
}

void SceneManager::ReleaseFadeAnimations()
{
	s_fadeOutAnime = nullptr;
	s_fadeInAnime = nullptr;
}
