// SceneTitle.cpp
#include "SceneSmaple.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"

void SceneSmaple::Load()
{
	ModelManager::Load(ModelTag::Player);

	World& w = GetWorld();

}

void SceneSmaple::Unload()
{
	TextureManager::Clear();
}

void SceneSmaple::Update()
{
}

void SceneSmaple::Draw()
{
}


