#include "stageLevel.h"
#include "GameState.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"

void stageLevel::Awake()
{
	GameObject& leve = gameObject->CreateChild();
	level = &leve.AddComponent<UiSpriteRenderer>(TextureTag::stagelevel_Easy, Vector2(135.f, 150.f), Vector2(170.f, 100.f));


	GameObject& Stage = gameObject->CreateChild();
	StageLevel = &Stage.AddComponent<UiSpriteRenderer>(TextureTag::stageLv1, Vector2(70.f, 100.f), Vector2(300.f, 200.f));

	GameObject& leve2 = gameObject->CreateChild();
	level2 = &leve2.AddComponent<UiSpriteRenderer>(TextureTag::stagelevel_Normal, Vector2(415.f, 450.f), Vector2(170.f, 100.f));


	GameObject& Stage2 = gameObject->CreateChild();
	StageLevel2 = &Stage2.AddComponent<UiSpriteRenderer>(TextureTag::stageLv2, Vector2(350.f, 400.f), Vector2(300.f, 200.f));

}

void stageLevel::Update()
{
	
	//->SetVisible(false);
}
