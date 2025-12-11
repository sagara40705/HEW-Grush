#pragma once
#include "Component.h"

class PlayerState;

class ActionUI : public MonoBehaviour
{
private: //éQè∆
	PlayerState* state_ = nullptr;
	UiSpriteRenderer* spin_ = nullptr;
	UiSpriteRenderer* dash_ = nullptr;
	UiSpriteRenderer* spinMoji_ = nullptr;
	UiSpriteRenderer* dashMoji_ = nullptr;


public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:

};