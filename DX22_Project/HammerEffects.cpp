#include "HammerEffects.h"
#include "GameObject.h"
#include "Sprite3DRenderer.h"
#include "SpriteAnimation.h"
#include "Camera.h"
#include "DebugLog.h"

void HammerEffects::Awake()
{
	CreateGradeUpEffect();
	CreateGradeDownEffect();
	CreateHitEffect();
}

void HammerEffects::Start()
{
	camTf_ = Camera::Main()->gameObject->transform;
}

void HammerEffects::OnDestroy()
{
	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		gradeUp_[i] = nullptr;
	}
	for (int i = 0; i < MAX_EFFECTS[Type::GradeDown]; ++i)
	{
		gradeDown_[i] = nullptr;
	}
	for (int i = 0; i < MAX_EFFECTS[Type::Hit]; ++i)
	{
		hit_[i] = nullptr;
	}
}

void HammerEffects::Play(Type type, const Vector3& pos)
{
	GameObject* effect = nullptr;

	switch (type)
	{
	case HammerEffects::GradeUp:  effect = GetFreeGradeUpEffect();
		break;
	case HammerEffects::GradeDown:  effect = GetFreeGradeDownEffect();
		break;
	case HammerEffects::Hit:  effect = GetFreeHitEffect();
	default:
		break;
	}

	if (!effect) return;
	DebugLog::OutputValue("Hammer Effect Play", static_cast<int>(type));
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
	// 位置設定
	Transform* efTf = effect->transform;
	constexpr Vector3 OFFSET = Vector3(0.f, 0.5f, 0.f);
	efTf->SetPosition(pos + OFFSET);

	// ビルボード設定
	Vector3 toCam = camTf_->GetWorldPosition() - pos;	// カメラ方向ベクトル
	toCam.y = 0.f;	// Y軸回転のみ
	toCam.Normalize();
	float angle = atan2f(toCam.x, toCam.z);	// ラジアン角度取得
	angle = angle * (180.f / 3.14159265f);	// 度変換
	angle += 180.f; // 向きを合わせる
	DebugLog::OutputValue("Hammer Effect Angle", angle);
	efTf->SetRotation(Vector3(0.f, angle, 0.f));
}

void HammerEffects::CreateGradeUpEffect()
{
	TextureManager::Load(TextureTag::GradeUpEffect);

	World* world = gameObject->world;

	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		GameObject& effect = world->CreateGameObject();
		effect.AddComponent<Sprite3DRenderer>(TextureTag::GradeUpEffect, Vector2::zero(), Vector2(2.f, 2.f));
		effect.AddComponent<SpriteAnimation>(5, 5, 2, 25, false);
		effect.SetActive(false);
		gradeUp_[i] = &effect;
	}
}

GameObject* HammerEffects::GetFreeGradeUpEffect()
{
	for (int i = 0; i < MAX_EFFECTS[Type::GradeUp]; ++i)
	{
		if (gradeUp_[i]->IsActiveSelf()) continue;

		return gradeUp_[i];
	}
	return nullptr;
}

void HammerEffects::CreateGradeDownEffect()
{
	TextureManager::Load(TextureTag::GradeDownEffect);

	World* world = gameObject->world;

	for (int i = 0; i < MAX_EFFECTS[Type::GradeDown]; ++i)
	{
		GameObject& effect = world->CreateGameObject();
		effect.AddComponent<Sprite3DRenderer>(TextureTag::GradeDownEffect, Vector2::zero(), Vector2(2.f, 2.f));
		effect.AddComponent<SpriteAnimation>(5, 5, 2, 25, false);
		effect.SetActive(false);
		gradeDown_[i] = &effect;
	}
}

GameObject* HammerEffects::GetFreeGradeDownEffect()
{
	for (int i = 0; i < MAX_EFFECTS[Type::GradeDown]; ++i)
	{
		if (gradeDown_[i]->IsActiveSelf()) continue;

		return gradeDown_[i];
	}
	return nullptr;
}

void HammerEffects::CreateHitEffect()
{
	TextureManager::Load(TextureTag::HitEffect);

	World* world = gameObject->world;

	for (int i = 0; i < MAX_EFFECTS[Type::Hit]; ++i)
	{
		GameObject& effect = world->CreateGameObject();
		effect.AddComponent<Sprite3DRenderer>(TextureTag::HitEffect, Vector2::zero(), Vector2(2.f, 2.f));
		effect.AddComponent<SpriteAnimation>(5, 5, 1, 25, false);
		effect.SetActive(false);
		hit_[i] = &effect;
	}
}

GameObject* HammerEffects::GetFreeHitEffect()
{
	for (int i = 0; i < MAX_EFFECTS[Type::Hit]; ++i)
	{
		if (hit_[i]->IsActiveSelf()) continue;

		return hit_[i];
	}
	return nullptr;
}
