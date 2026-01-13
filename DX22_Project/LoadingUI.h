#pragma once
#include "Component.h"

class LoadingUI : public Component
{
public:
	void Start() override;
	void OnDestroy() override;
};