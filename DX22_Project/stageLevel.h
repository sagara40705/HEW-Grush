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
	UiSpriteRenderer* StageLevel3_a = nullptr;
	UiSpriteRenderer* StageLevel3_b = nullptr;
	UiSpriteRenderer* StageLevel3_c = nullptr;
	UiSpriteRenderer* level3_a = nullptr;
	UiSpriteRenderer* level3_b = nullptr;
	UiSpriteRenderer* level3_c = nullptr;
	UiSpriteRenderer* bou = nullptr;
	UiSpriteRenderer* bou2 = nullptr;
	UiSpriteRenderer* Image = nullptr;


};
	

