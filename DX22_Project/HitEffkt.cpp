#include "HitEffkt.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>

void HitEffkt::Awake()
{
	GameObject& spinMono = gameObject->CreateChild();
	//spinMono.AddComponent<UiSpriteRenderer>(TextureTag::, Vector2(0,0), Vector2(100, 100)); //←コンストラクタが不適だった

	//GameObject&hit = gameObject->CreateChild();
	//HIt_ = &hit.AddComponent<UiSpriteRenderer>(TextureTag::Spin, 1, 1, 1, 1);
}

void HitEffkt::Start()
{
}

void HitEffkt::Update()
{
}

void HitEffkt::OnDestroy()
{
}
