// SceneTitle.cpp
#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"


void SceneTitle::Load()
{
	//テクスチャーロード


	World& w = GetWorld();

	//背景生成

	//プレイヤー画像生成

	//タイトルマネージャー生成
}

void SceneTitle::Unload()
{
	TextureManager::Clear();
}

void SceneTitle::Update()
{
}

void SceneTitle::Draw()
{
}


