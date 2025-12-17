#pragma once
#include "Component.h"

class GlassGaugeManager;

class GlassGaugeUI : public MonoBehaviour
{
private: //QÆ
	GlassGaugeManager* manager_ = nullptr;
	UiSpriteRenderer* em_ = nullptr;
	UiSpriteRenderer* empty_ = nullptr;
	UiSpriteRenderer* max_ = nullptr;
	SpriteAnimation* effect_ = nullptr;

	float gDecayDuration = 0.3f; // 0.3•b‚¾‚¯Œõ‚é
	float decayTimer = 0.f;

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:

};