#pragma once
#include "Component.h" // MonoBehaviour
#include "CanvasUI.h"

class PlayerIntroSequence : public MonoBehaviour
{
private:
	Rigidbody* rb_ = nullptr;
	bool isIntro = true;
private:
	const float kIntroTime = 3.0f;
	float introTimer = 0.f;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	void FinishIntro();

	
};