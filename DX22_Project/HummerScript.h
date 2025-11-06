#pragma once
#include "CollisionListener.h"

class Transform;

class  HummerScript :public MonoBehaviour, public TriggerListener
{
public:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	float rotate = 0.0f;

	HummerScript(Transform* _o): owner(_o)  {}

	void Start() override;
	//void Update() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& c) override;


};