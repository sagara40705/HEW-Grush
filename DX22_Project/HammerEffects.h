#pragma once
#include "Component.h"
#include "Vector3.h"

class HammerEffects : public Component
{
public:
	enum Type
	{
		GradeUp,
		Count,
	};
private:
	static constexpr int MAX_EFFECTS[Type::Count] = { 2 };
private:
	GameObject* gardeUp_[MAX_EFFECTS[GradeUp]]{};
public:
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
public:
	void Play(Type type, const Vector3& pos);
private:
	Transform* camTf_ = nullptr;
	// 対象エフェクトの位置を設定、ビルボード処理
	void SetEffectTransform(GameObject* effect, const Vector3& pos);
private:
	void CreateGradeUpEffect();
	GameObject* GetFreeGradeUpEffect();
};