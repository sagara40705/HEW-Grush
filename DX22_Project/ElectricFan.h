#pragma once
#include "Component.h"
#include "Vector3.h"

/*
* 扇風機のギミック・風を出す
*/
class ElectricFan : public Component
{
public:
	void Awake() override;
	void OnDestroy() override;
};

/*
* 風本体・プレイヤーに力を加える
*/
class Wind : public Component, public TriggerListener
{
private: 
	//風の向き
	Vector3 dir_ = Vector3::zero();
public:
	void Start() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& info) override;
};

