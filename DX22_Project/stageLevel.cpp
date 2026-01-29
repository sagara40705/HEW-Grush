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

	GameObject& leve3_a = gameObject->CreateChild();
	level3_a = &leve3_a.AddComponent<UiSpriteRenderer>(TextureTag::Stagelevel_Hard_a, Vector2(715.f, 60.f), Vector2(170.f, 100.f));


	GameObject& Stage3_a = gameObject->CreateChild();
	StageLevel3_a = &Stage3_a.AddComponent<UiSpriteRenderer>(TextureTag::stageLv3, Vector2(650.f, 10.f), Vector2(300.f, 200.f));

	GameObject& leve3_b = gameObject->CreateChild();
	level3_b = &leve3_b.AddComponent<UiSpriteRenderer>(TextureTag::Stagelevel_Hard_b, Vector2(1065.f, 200.f), Vector2(170.f, 100.f));


	GameObject& Stage3_b = gameObject->CreateChild();
	StageLevel3_c = &Stage3_b.AddComponent<UiSpriteRenderer>(TextureTag::stageLv3, Vector2(1000.f, 150.f), Vector2(300.f, 200.f));

	GameObject& leve3_c = gameObject->CreateChild();
	level3_c = &leve3_c.AddComponent<UiSpriteRenderer>(TextureTag::Stagelevel_Hard_c, Vector2(765.f, 600.f), Vector2(170.f, 100.f));


	GameObject& Stage3_c = gameObject->CreateChild();
	StageLevel3_c = &Stage3_c.AddComponent<UiSpriteRenderer>(TextureTag::stageLv3, Vector2(700.f, 550.f), Vector2(300.f, 200.f));

	

}

void stageLevel::Update()
{
	
	//->SetVisible(false);
}
