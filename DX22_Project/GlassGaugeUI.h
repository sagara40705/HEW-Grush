#pragma once
#include "Component.h"

class GlassGaugeManager;

class GlassGaugeUI : public MonoBehaviour
{
private: //éQè∆
	GlassGaugeManager* manager_ = nullptr;
	UiSpriteRenderer* empty_ = nullptr;
	UiSpriteRenderer* max_ = nullptr;
	SpriteAnimation* effect_ = nullptr;
	
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:

};