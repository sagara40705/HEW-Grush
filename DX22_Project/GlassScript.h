#pragma once
#include "Component.h"

enum GlassLevel
{
	Level_1,
	Level_2,
	Level_3,
	Level_Count,
};

class GlassScript : public Component
{
public:
	void Start() override;
	void OnDestroy() override;
	int level = 1;
};