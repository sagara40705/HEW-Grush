#pragma once
#include "Component.h"

class Hammer : public MonoBehaviour, public TriggerListener
{
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& info) override;
};