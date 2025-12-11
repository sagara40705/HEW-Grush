#include "LoadingScene.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Effekseer.h"

void LoadingScene::OriginalLoad()
{
	TextureManager::Load(TextureTag::LoadGauge);

	World& w = GetWorld();

	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	//îwåiê∂ê¨
	GameObject& loadGauge = w.CreateGameObject();
	loadGauge.AddComponent<UiSpriteRenderer>(TextureTag::LoadGauge, Vector2(0.f, 0.f), Vector2(500.f, 50.f));

	


}
::Effekseer::EffectRef EffekseerSystem::LoadEffect(const char16_t* path)
{

}