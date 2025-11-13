#pragma once
#include "Component.h"

class PlayerAction : public MonoBehaviour
{
public:
	void Start() override;
	void OnDestroy() override;
	void Update() override;

	bool isDash = false;
};