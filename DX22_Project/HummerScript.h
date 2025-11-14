#pragma once
#include "CollisionListener.h"
#include "GameObject.h"

constexpr float kGaugeUpValue[3] = { 1.f, 3.f, 5.f };
constexpr float kMaxGaugeValue[3] = { 10.f, 20.f, 30.f };

class Transform;
class  HummerScript :public MonoBehaviour, public TriggerListener
{
public:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	float rotate = 0.0f;
	int level = 1;
	float gauge = 0.0f;

	HummerScript(Transform* _o): owner(_o)  {}

	void Start() override;
	//void Update() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& c) override;
	bool CheckLevel(GameObject* glass);
	void GaugeUp(GameObject* glass);
	void LevelUp();
};