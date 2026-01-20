#pragma once
#include "Component.h"

class PlayerState;

class ActionUI : public MonoBehaviour
{
private: //éQè∆
	PlayerState* state_ = nullptr;
	UiSpriteRenderer* spin_ = nullptr;
	UiSpriteRenderer* dash_ = nullptr;
	UiSpriteRenderer* spFlash_ = nullptr;
	UiSpriteRenderer* spFront_  = nullptr;
	UiSpriteRenderer* spinMoji_ = nullptr;
	UiSpriteRenderer* dashMoji_ = nullptr;
	UiSpriteRenderer* daFlash_ = nullptr;
	UiSpriteRenderer* daFront_ = nullptr;
	
	float glowTimerDash = 0.f;
	float glowSpinTimer = 0.f;
	bool wasSpinReady = false;
	bool wasDashReady = false;

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:


};