#pragma once
#include "Component.h"

class PlayerState;

class PlayerAnimation : public MonoBehaviour
{
private:
	PlayerState* state = nullptr;
	Animator* anime_ = nullptr;
public:
	void Start() override;
	void OnDestroy() override;
	void Update()  override;
};