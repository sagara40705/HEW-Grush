#pragma once
#include "Component.h"

class PlayerState;

class ActionUI : public MonoBehaviour
{
private: //参照
	PlayerState* state_ = nullptr;
	UiSpriteRenderer* spin_ = nullptr;
	UiSpriteRenderer* dash_ = nullptr;
	UiSpriteRenderer* spFlash_ = nullptr;
	UiSpriteRenderer* spFront_  = nullptr;
	UiSpriteRenderer* spinMoji_ = nullptr;
	UiSpriteRenderer* dashMoji_ = nullptr;
	UiSpriteRenderer* daFlash_ = nullptr;
	UiSpriteRenderer* daFront_ = nullptr;
	
	float glowTimerSpin = 0.f;
	float glowTimerDash = 0.f;
	bool wasSpinReady = false;//回転攻撃の準備完了前フレーム状態
	bool wasDashReady = false;//ダッシュ攻撃の準備完了前フレーム状態

	
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:


};