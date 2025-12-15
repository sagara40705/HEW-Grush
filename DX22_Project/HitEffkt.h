#pragma once
#include "Component.h"

class PlayerState;

class HitEffkt : public MonoBehaviour
{
private: //éQè∆
	PlayerState* state_ = nullptr;
	UiSpriteRenderer* HIt_ = nullptr;

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:

};


