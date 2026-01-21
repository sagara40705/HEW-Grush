#include "HammerEffects.h"
#include "GameObject.h"
#include "Sprite3DRenderer.h"
#include "SpriteAnimation.h"
#include "Camera.h"

void HammerEffects::Awake()
{
	CreateGradeUpEffect();
}

void HammerEffects::Start()
{
	camTf_ = Camera::Main()->gameObject->transform;
}

void HammerEffects::OnDestroy()
{
	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		gardeUp_[i] = nullptr;
	}
}

void HammerEffects::Play(Type type, const Vector3& pos)
{
	GameObject* effect = nullptr;

	switch (type)
	{
	case HammerEffects::GradeUp:  effect = GetFreeGradeUpEffect();
		break;
	default:
		break;
	}

	if (!effect) return;
	effect->SetActive(true);
	SetEffectTransform(effect, pos);	// 位置設定 + ビルボード
	// アニメーション再生
	auto anim = effect->GetComponent<SpriteAnimation>();
	if (anim)
	{
		anim->Play();
	}
}

void HammerEffects::SetEffectTransform(GameObject* effect, const Vector3& pos)
{
	Transform* efTf = effect->transform;
}

void HammerEffects::CreateGradeUpEffect()
{
	TextureManager::Load(TextureTag::GradeUpEffect);

	World* world = gameObject->world;

	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		GameObject& effect = world->CreateGameObject();
		effect.AddComponent<Sprite3DRenderer>(TextureTag::GradeUpEffect, Vector2::zero(), Vector2::one());
		effect.AddComponent<SpriteAnimation>(5, 5, 1, 25, false);
		effect.SetActive(false);
		gardeUp_[i] = &effect;
	}
}

GameObject* HammerEffects::GetFreeGradeUpEffect()
{
	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		if (gardeUp_[i]->IsActiveSelf()) continue;

		return gardeUp_[i];
	}
	return nullptr;
}
