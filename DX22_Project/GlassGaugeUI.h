#pragma once
#include "Component.h"

class GlassGaugeManager;

class GlassGaugeUI : public MonoBehaviour
{
private: //éQè∆
	GlassGaugeManager* ggm_ = nullptr;
	UiSpriteRenderer* ui_ = nullptr;
	GameObject* effect_ = nullptr;
	SpriteAnimation* efAnime_ = nullptr;
public:
	GlassGaugeUI(GameObject* effect) : effect_(effect) {}
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};