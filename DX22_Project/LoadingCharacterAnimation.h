#pragma once
#include "Component.h"

class LoadingCharacterAnimation : public MonoBehaviour
{
private:
	float animTime_ = 0.0f;
	Vector3 originalPos_;
	
public:
	void Start() override;
	void Update() override;
};