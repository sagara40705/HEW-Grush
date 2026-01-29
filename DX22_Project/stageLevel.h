#pragma once
#include "Component.h"
#include "TextureManager.h"
//#include "StagePoint.h"
#include <array>
class stageLevel : public MonoBehaviour
{
public:
	void Awake();
	void Update() override;
	UiSpriteRenderer* StageLevel = nullptr;
	UiSpriteRenderer* level = nullptr;
	UiSpriteRenderer* StageLevel2 = nullptr;
	UiSpriteRenderer* level2 = nullptr;
};

