#pragma once
#include "Component.h"

/*
* ジャンプ台のギミック
*/
class JampSki : public Component, public TriggerListener
{
public:
	void Start() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& info) override;
};