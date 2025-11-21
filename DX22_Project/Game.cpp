// Game.cpp
#include "Game.h"
#include "AllComponet.h"
#include "SceneGame.h"
#include <cassert>

std::vector<std::unique_ptr<Scene>> Game::s_scenes;
std::unique_ptr<Scene>              Game::s_nextScene;

void Game::Init()
{
	// クリーンアップ
	Uninit();

	// 最初のシーンを設定して切り替え
	SetNext<SceneGame>();
	LoadScene();
}

void Game::Uninit()
{
	SceneStackClear();

	// 次シーンが残っていれば Unload
	if (s_nextScene)
	{
		s_nextScene->RootUnload();
		s_nextScene.reset();
	}

    MeshFilter::ReleaseAll();
}

void Game::Update()
{
	if (s_scenes.empty()) return;

	// 一番上のシーンだけを更新
	s_scenes.back()->RootUpdate();
}

void Game::Draw()
{
	// 下から順に描画（下のシーン → 上のシーン）
	for (auto& scene : s_scenes)
	{
		if (!scene) continue;
		scene->RootDraw();
	}
}


World& Game::GetWorld()
{
	return s_scenes.front()->GetWorld();
}

void Game::LoadScene()
{
	if (!s_nextScene) return;

	SceneStackClear();

	s_scenes.emplace_back(std::move(s_nextScene));

	s_scenes.front()->RootLoad();
}

void Game::PreLoadNext()
{
	if (!s_nextScene) return;

	s_nextScene->RootLoad();
}

void Game::StackScene()
{
	if (!s_nextScene) return;

	s_nextScene->RootLoad();

	s_scenes.emplace_back(std::move(s_nextScene));
}

void Game::PopScene()
{
	if (s_scenes.empty()) return;

	auto& top = s_scenes.back();
	if (top)
	{
		top->RootUnload();
	}
	s_scenes.pop_back();
}

void Game::SceneStackClear()
{
	// 既存スタックのシーンをすべて Unload
	for (auto& scene : s_scenes)
	{
		if (scene)
		{
			scene->RootUnload();
		}
	}
	s_scenes.clear();
}

